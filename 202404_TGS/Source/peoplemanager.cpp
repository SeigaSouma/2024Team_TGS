//=============================================================================
// 
//  人のマネージャ処理 [peoplemanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "peoplemanager.h"
#include "people.h"
#include "calculation.h"
#include "manager.h"
#include "debugproc.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\people\\manager.txt";
	const float SPAWN_DISTANCE = 600.0f;		// 湧き距離間隔
	const float SPAWN_ALL_LENGTH = 80000.0f;	// 出現する全ての長さ

	
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
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CPeopleManager::Update()
{

	// 評価ごとに人数増やす
	if (m_Rank != m_OldRank)
	{
		SetByRank();
	}

	// 前回のランク
	m_OldRank = m_Rank;
}

//==========================================================================
// ランクごとのセット処理
//==========================================================================
void CPeopleManager::SetByRank()
{
	// 障害物のリスト取得
	CListManager<CPeople> list = CPeople::GetListObj();

	// 先頭を保存
	std::list<CPeople*>::iterator itr = list.GetEnd();
	CPeople* pObj = nullptr;

	// 全員フェードアウト
	while (list.ListLoop(itr))
	{
		pObj = *itr;

		pObj->SetState(CPeople::STATE::STATE_FADEOUT);
	}

	if (m_Rank == CJudge::JUDGE::JUDGE_MAX)
	{// 最低ランク
		return;
	}


	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 300.0f, 500.0f);
	MyLib::Vector3 spawnpos = pos;
	MyLib::Vector3 rot = MyLib::Vector3(0.0f, D3DX_PI * 0.5f, 0.0f);
	int type = 0, patternNum = static_cast<int>(m_PatternByRank[m_Rank].size());


	for (float len = 0.0f; len <= SPAWN_ALL_LENGTH; len += SPAWN_DISTANCE)
	{
		type = rand() % patternNum;

		// 位置リセット
		spawnpos = pos;
		spawnpos.x += len;
		spawnpos.z += UtilFunc::Transformation::Random(-50, 50) * 10.0f;
		spawnpos.z += UtilFunc::Transformation::Random(-50, 50);

		SetPeople(spawnpos, rot, type);
	}
}

//==========================================================================
// 人配置
//==========================================================================
void CPeopleManager::SetPeople(MyLib::Vector3 pos, MyLib::Vector3 rot, int nPattern)
{
	SPattern NowPattern = m_PatternByRank[m_Rank][nPattern];
	int nNumSpawn = NowPattern.nNum;	// スポーンする数
	CPeople* pPeople = nullptr;

	for (const auto& data : NowPattern.data)
	{
		// スポーン時の向きを掛け合わせる
		MyLib::Vector3 spawnPos = pos;

		// スポーン位置分加算
		spawnPos += data.pos;

		// 生成
		pPeople = CPeople::Create(
			m_vecMotionFileName[data.nType],	// ファイル名
			spawnPos);							// 位置

		if (pPeople == nullptr)
		{
			delete pPeople;
			break;
		}

		// 向き設定
		pPeople->SetRotation(rot);
		pPeople->SetRotDest(rot.y);
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
