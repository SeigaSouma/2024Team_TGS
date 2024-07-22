//=============================================================================
// 
//  人のマネージャ処理 [peoplemanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "peoplemanager.h"
#include "people.h"
#include "player.h"
#include "calculation.h"
#include "manager.h"
#include "camera.h"
#include "debugproc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\people\\manager.txt";
	const float SPAWN_DISTANCE = 600.0f;		// 湧き距離間隔
	const float SPAWN_ALL_LENGTH = 80000.0f;	// 出現する全ての長さ
	const float SPAWN_MIN_LENGTH = -1000.0f;	// 出現する最低距離（プレイヤーからの距離）
	const float SPAWN_MAX_LENGTH = 6000.0f;		// 出現する最高距離（プレイヤーからの距離、これより先は後生成リスト行き）
}
CPeopleManager* CPeopleManager::m_ThisPtr = nullptr;				// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CPeopleManager::CPeopleManager()
{
	// 値のクリア
	m_Rank = CJudge::JUDGE::JUDGE_MAX;	// 現在のランク
	m_OldRank = CJudge::JUDGE::JUDGE_MAX;		// 前回のランク
	m_vecPattern.clear();			// 配置パターン
	m_vecMotionFileName.clear();	// モーションファイル名
	m_PatternByRank;			// 配置パターン
}

//==========================================================================
// デストラクタ
//==========================================================================
CPeopleManager::~CPeopleManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPeopleManager* CPeopleManager::Create()
{
	if (m_ThisPtr != nullptr) return m_ThisPtr;

	// メモリ確保
	m_ThisPtr = DEBUG_NEW CPeopleManager;

	if (m_ThisPtr != nullptr)
	{
		// 初期化処理
		HRESULT hr = m_ThisPtr->Init();
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPeopleManager::Init()
{
	// ファイル読み込み
	HRESULT hr = ReadText(FILENAME);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPeopleManager::Uninit()
{
	ResetLateSpawn();

	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPeopleManager::Update()
{
	// 範囲外の人を消す
	DespawnPeople();

	// 評価ごとに人数増やす
	if (m_Rank != m_OldRank)
	{
		SetByRank();
	}

	// 後生成の人を出す
	LateSpawn();

	// 前回のランク
	m_OldRank = m_Rank;
}

//==========================================================================
// ランクごとのセット処理
//==========================================================================
void CPeopleManager::SetByRank()
{
	// 人のリスト取得
	CListManager<CPeople> listPeople = CPeople::GetListObj();

	// プレイヤーリスト取得
	CListManager<CPlayer> listPlayer = CPlayer::GetListObj();

	// 先頭を保存
	std::list<CPeople*>::iterator itr = listPeople.GetEnd();
	CPeople* pObj = nullptr;
	CPlayer* pPlayer = (*listPlayer.GetBegin());

	// 全員フェードアウト
	while (listPeople.ListLoop(itr))
	{
		(*itr)->SetState(CPeople::STATE::STATE_FADEOUT);
	}

	if (m_Rank == CJudge::JUDGE::JUDGE_MAX)
	{// 最低ランク
		return;
	}


	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 300.0f, 1500.0f);
	MyLib::Vector3 spawnpos = pos;
	MyLib::Vector3 rot = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f);
	int type = 0, patternNum = static_cast<int>(m_PatternByRank[m_Rank].size());
	float playerLen = pPlayer->GetMoveLength();
	float playerPosX = pPlayer->GetPosition().x;

	for (float len = playerLen + SPAWN_MIN_LENGTH; len <= SPAWN_ALL_LENGTH; len += SPAWN_DISTANCE)
	{
		type = rand() % patternNum;

		// 位置リセット
		spawnpos = pos;
		spawnpos.x += len;
		spawnpos.z += UtilFunc::Transformation::Random(-50, 50) * 10.0f;
		spawnpos.z += UtilFunc::Transformation::Random(-50, 50);

		if (spawnpos.x < playerPosX + SPAWN_MAX_LENGTH)
		{// 範囲内なのですぐ出す
			SetPeople(spawnpos, rot, type);
		}
		else
		{// 範囲外なので後で出す
			m_lateSpawnPeople.push_back(CPeopleManager::SPeopleData(type, spawnpos));
		}
	}
}

//==========================================================================
// 人撤去
//==========================================================================
void CPeopleManager::DespawnPeople()
{
	std::list<CPeople*> despawnList;
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 人のリスト取得
	CListManager<CPeople> listObjPeople = CPeople::GetListObj();

	// プレイヤーリスト取得
	CListManager<CPlayer> listPlayer = CPlayer::GetListObj();
	
	// 先頭を保存
	std::list<CPeople*>::iterator itr = listObjPeople.GetEnd();
	CPlayer* pPlayer = (*listPlayer.GetBegin());

	// 距離取得・一定距離から外に出たら消すリストに追加
	float playerPosX = pPlayer->GetPosition().x;
	while (listObjPeople.ListLoop(itr))
	{
		MyLib::Vector3 pos = (*itr)->GetPosition();
		MyLib::Vector3 screenPos = pCamera->GetScreenPos(pos);

		if (pos.x < playerPosX + SPAWN_MIN_LENGTH && screenPos.x < -100.0f)
		{
			despawnList.push_back((*itr));
		}
	}

	// 人を消す
	for (const auto& peaple : despawnList)
	{
		peaple->Uninit();
	}
}

//==========================================================================
// 後生成分の生成
//==========================================================================
void CPeopleManager::LateSpawn()
{
	std::vector<SPeopleData> spawnList;

	// プレイヤーリスト取得・先頭を保存
	CListManager<CPlayer> listPlayer = CPlayer::GetListObj();
	CPlayer* pPlayer = (*listPlayer.GetBegin());

	// 出現範囲にいる人を抽出
	float playerPosX = pPlayer->GetPosition().x;
	for (const auto& latePeaple : m_lateSpawnPeople)
	{
		float peoplePosX = latePeaple.pos.x;

		if (peoplePosX <= playerPosX + SPAWN_MAX_LENGTH)
		{
			spawnList.push_back(latePeaple);
		}
	}

	// 抽出した人を出現して後生成リストから削除
	MyLib::Vector3 rot = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f);
	for (const auto& spawnPeaple : spawnList)
	{
		SetPeople(spawnPeaple.pos, rot, spawnPeaple.nType);
		m_lateSpawnPeople.remove(spawnPeaple);
	}
}

//==========================================================================
// 人配置
//==========================================================================
void CPeopleManager::SetPeople(const MyLib::Vector3& pos, const MyLib::Vector3& rot, int nPattern)
{
	const SPattern& NowPattern = m_PatternByRank[m_Rank][nPattern];
	int nNumSpawn = NowPattern.nNum;	// スポーンする数
	CPeople* pPeople = nullptr;

	MyLib::Vector3 spawnPos;

	// キャラの数
	int maxType = static_cast<int>(m_vecMotionFileName.size()) - 1;
	for (const auto& data : NowPattern.data)
	{
		// スポーン時の向きを掛け合わせる
		spawnPos = pos;

		// スポーン位置分加算
		spawnPos += data.pos;

		// 生成
		pPeople = CPeople::Create(
			m_vecMotionFileName[UtilFunc::Transformation::Random(0, maxType)],	// ファイル名
			spawnPos);							// 位置

		// 向き設定
		if (pPeople != nullptr)
		{
			pPeople->SetRotation(rot);
			pPeople->SetRotDest(rot.y);
		}
	}
	
}

//==========================================================================
// 外部ファイル読み込み処理
//==========================================================================
HRESULT CPeopleManager::ReadText(const std::string& filename)
{
	// ファイルを開く
	std::ifstream File(filename);
	if (!File.is_open()) {
		return E_FAIL;
	}

	// コメント用
	std::string hoge;
	std::string charaFile;
	m_vecMotionFileName.clear();


	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// ストリーム作成
		std::istringstream lineStream(line);


		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// MOTION_FILENAMEでモーションファイル名読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>		// ＝
				hoge >>		// ＝
				charaFile;	// モデルファイル名

			// モーションファイル追加
			m_vecMotionFileName.push_back(charaFile);
			continue;
		}

		if (line.find("PATTERNSET") != std::string::npos)
		{// PATTERNSETでパターン読み込み

			// パターン追加
			m_vecPattern.emplace_back();

			int rank = 0;

			while (line.find("END_PATTERNSET") == std::string::npos)
			{
				std::getline(File, line);


				if (line.find("RANK") != std::string::npos)
				{// キャラの種類

					// ストリーム作成
					std::istringstream lineStream(line);

					// 情報渡す
					lineStream >>
						hoge >>
						hoge >>	// ＝
						rank;	// ランク

					m_vecPattern.back().nRank = rank;
					continue;
				}


				if (line.find("PEOPLESET") != std::string::npos)
				{// PEOPLESETでパターン読み込み

					// 読み込みデータ
					SPeopleData data;
					data.nType = 0;

					while (line.find("END_PEOPLESET") == std::string::npos)
					{
						std::getline(File, line);

						if (line.find("TYPE") != std::string::npos)
						{// キャラの種類

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>
								hoge >>	// ＝
								data.nType;	// キャラの種類
							continue;
						}

						if (line.find("POS") != std::string::npos)
						{// POSで位置

							// ストリーム作成
							std::istringstream lineStream(line);

							// 情報渡す
							lineStream >>
								hoge >>
								hoge >>						// ＝
								data.pos.x >> data.pos.y >> data.pos.z;	// 位置
							continue;
						}
					}

					// データ追加
					m_vecPattern.back().data.push_back(data);

				}// END_PEOPLESET
			}

			// 人の数
			m_vecPattern.back().nNum = m_vecPattern.back().data.size();

		}// END_PATTERNSET

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();

	// ランク別パターン
	for (const auto& pattern : m_vecPattern)
	{
		m_PatternByRank[pattern.nRank].push_back(pattern);
	}


	return S_OK;
}
