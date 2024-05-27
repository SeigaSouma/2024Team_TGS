//=============================================================================
// 
//  マップの障害物処理 [map_obstacle.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "map_obstacle.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "collisionLine_Box.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CMap_Obstacle> CMap_Obstacle::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CMap_Obstacle::CMap_Obstacle(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CMap_Obstacle::~CMap_Obstacle()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CMap_Obstacle *CMap_Obstacle::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// メモリの確保
	CMap_Obstacle* pObj = DEBUG_NEW CMap_Obstacle;

	if (pObj != nullptr)
	{
		pObj->m_ObstacleInfo = info;

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMap_Obstacle::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	HRESULT hr = CObjectX::Init(m_ObstacleInfo.modelFile);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

#if _DEBUG

	for (const auto& box : m_ObstacleInfo.boxcolliders)
	{
		CCollisionLine_Box* pBox = CCollisionLine_Box::Create(MyLib::AABB(box.vtxMin, box.vtxMax), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// 当たり判定ボックス
		m_pCollisionLineBox.push_back(pBox);
	}
#endif

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMap_Obstacle::Uninit()
{
	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CMap_Obstacle::Kill()
{
	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMap_Obstacle::Update()
{
	for (auto& collider : m_ObstacleInfo.boxcolliders)
	{
		collider.TransformOffset(GetWorldMtx());
	}

#if _DEBUG
	int i = 0;
	for (const auto& box : m_pCollisionLineBox)
	{
		m_ObstacleInfo.boxcolliders[i].TransformOffset(GetWorldMtx());
		box->SetPosition(m_ObstacleInfo.boxcolliders[i].worldmtx.GetWorldPosition());
		box->SetRotation(GetRotation());
		i++;
	}

#endif
}

//==========================================================================
// 描画処理
//==========================================================================
void CMap_Obstacle::Draw()
{
	// 描画
	CObjectX::Draw();
}

