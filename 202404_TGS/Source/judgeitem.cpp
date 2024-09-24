//=============================================================================
// 
//  ジャッジアイテム処理 [judgeitem.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "judgeitem.h"
#include "judgeitemManager.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "spline.h"
#include "course.h"
#include "baggage.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\coin\\coin.x";	// モデルパス
	const float TIME_ROTATION = 2.0f;	// 回転するための時間
}

namespace StateTime
{
	const float SWING = 2.0f;	// ゆらゆら
	const float ROTATE = 0.6f;	// 回転
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CJudgeItem> CJudgeItem::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeItem::CJudgeItem(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_pMgr = nullptr;	// マネージャ
	m_fLength = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudgeItem::~CJudgeItem()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CJudgeItem* CJudgeItem::Create(CJudgeItemManager* pMgr, const float length, const float height)
{
	// メモリの確保
	CJudgeItem* pObj = DEBUG_NEW CJudgeItem;

	if (pObj != nullptr)
	{
		// 引数情報
		pObj->m_pMgr = pMgr;
		pObj->m_fHeight = height;
		pObj->SetLength(length);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CJudgeItem::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

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
void CJudgeItem::Uninit()
{
	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CJudgeItem::Kill()
{
	// マネージャ内削除
	m_pMgr->DeleteJudgeItem(this);

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CJudgeItem::Update()
{
	MyLib::Vector3 rot = GetRotation();

	// 回転
	rot.y += ((D3DX_PI * 2.0f) * (1.0f / TIME_ROTATION)) * CManager::GetInstance()->GetDeltaTime();
	UtilFunc::Transformation::RotNormalize(rot.y);

	SetRotation(rot);
}

//==========================================================================
// 取得
//==========================================================================
void CJudgeItem::Get()
{
	// 削除
	Kill();

	// サウンド生成

	// エフェクト生成
}

//==========================================================================
// 距離設定
//==========================================================================
void CJudgeItem::SetLength(const float length)
{
	// 距離を設定
	m_fLength = length;

	// 座標を設定
	MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), m_fLength);
	pos.y += m_fHeight;
	SetPosition(pos);
}
