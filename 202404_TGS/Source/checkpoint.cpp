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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\koko.x";
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
			m_nSaveID = m_MyIndex;
		}
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CCheckpoint::Draw()
{
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
