//=============================================================================
// 
//  ゴールフラグ処理 [goalflag.cpp]
//  Author : 日野澤匠泉のプログラム(お借りしてません)
// 
//=============================================================================
#include "checkpoint.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "spline.h"
#include "course.h"
#include "timer.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\checkpoint\\flag.x";
	const float ROTATE_TIMER = (1.0f / 30.0f);
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CCheckpoint> CCheckpoint::m_List = {};	// リスト
int CCheckpoint::m_nAll = 0;
int CCheckpoint::m_nSaveID = 0;

//==========================================================================
// コンストラクタ
//==========================================================================
CCheckpoint::CCheckpoint(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;	// 状態カウンター
	m_fLength = 0.0f;
	m_fPassedTime = 0.0f;
	m_bIsPassed = false;
	m_fRotateTime = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
CCheckpoint::~CCheckpoint()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCheckpoint* CCheckpoint::Create(const float length)
{
	// メモリの確保
	CCheckpoint* pObj = DEBUG_NEW CCheckpoint;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();

		// 距離設定
		pObj->SetLength(length);

		// 総数加算
		m_nAll++;
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCheckpoint::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// ID設定
	m_MyIndex = m_nAll;

	// 初期化処理
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCheckpoint::Uninit()
{
	// 総数減らす
	if (m_nAll > 0)
	{
		m_nAll--;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CCheckpoint::Kill()
{
	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCheckpoint::Update()
{
	// 通過済みなら処理しない
	if (m_bIsPassed)
	{
		// 回転時間加算
		m_fRotateTime += CManager::GetInstance()->GetDeltaTime();

		MyLib::Vector3 rot = GetRotation();
		rot.z = UtilFunc::Correction::EasingEaseIn(0.0f, -D3DX_PI, 0.0f, 0.75f, m_fRotateTime);
		SetRotation(rot);
		return;
	}

	// 位置情報取得
	float playerlen = 0.0f;

	// リストループ
	CListManager<CPlayer> PlayerList = CPlayer::GetListObj();
	CPlayer* pObj = nullptr;
	while (PlayerList.ListLoop(&pObj))
	{
		// プレイヤーの位置情報取得
		playerlen = pObj->GetMoveLength();
	}

	if (playerlen >= m_fLength)
	{// チェックポイント通過したら

		if (m_nSaveID < m_MyIndex)
		{
			// ID保存
			m_nSaveID = m_MyIndex;

			// 通過した時間を保存
			m_fRotateTime = 0.0f;
			m_fPassedTime = CTimer::GetInstance()->GetTime();
			m_bIsPassed = true;

			// SE再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_KARAKURI);
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CCheckpoint::Draw()
{
	// ボックスコライダーの描画
	if (!CManager::IsDisp_CheckPoint()) return;

	// 描画
	CObjectX::Draw();
}

//==========================================================================
// 距離設定
//==========================================================================
void CCheckpoint::SetLength(const float length)
{
	// 距離を設定
	m_fLength = length;

	// 座標を設定
	MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), m_fLength);
	SetPosition(pos);
}

//==========================================================================
// チェックポイントファイル読み込み
//==========================================================================
void CCheckpoint::Load(const std::string filename)
{
	// ファイルを開く
	std::ifstream File(filename);
	if (!File.is_open()) {
		return;
	}

	float length = 0.0f;

	// コメント用
	std::string hoge;

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

		if (line.find("SET_LENGTH") != std::string::npos)
		{// TYPEで配置物の種類

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>	// ＝
				length;	// 配置物の種類

			CCheckpoint::Create(length);

			continue;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
}