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
#include "map_obstacle_obj.h"
#include "map_obstacle_motion.h"
#include "obstacle_fisharch.h"
#include "obstacle_birdcircle.h"

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
CMap_Obstacle::CMap_Obstacle(int nPriority,
	CObject::LAYER layer) : CObject(nPriority, layer)
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
CMap_Obstacle *CMap_Obstacle::Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange, const bool bSave)
{
	CMap_Obstacle* pObj = nullptr;
	// メモリの確保
	if (bChange)	// 集合で生成するか否か
	{
		switch (info.type)
		{
		case T::SAKANA:

			pObj = CObstacle_FishArch::Create(info);
			break;

		case T::BIRD:

			pObj = CObstacle_BirdCircle::Create(info);
			break;

		default:

			pObj = CMap_Obstacle_Object::Create(info,bChange,bSave);
			break;
		}
	}
	else {
		switch (info.type)
		{
		case T::SAKANA:

			pObj = CMap_Obstacle_Motion::Create(info);
			break;

		case T::BIRD:

			pObj = CMap_Obstacle_Motion::Create(info);
			break;

		default:

			pObj = CMap_Obstacle_Object::Create(info, bChange, bSave);
			break;
		}
	}

	if (pObj != nullptr)
	{
		pObj->m_ObstacleInfo = info;
		pObj->m_bSave = bSave;

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

	m_OriginObstacleInfo = m_ObstacleInfo;	// 障害物情報

#if _DEBUG

	for (const auto& box : m_ObstacleInfo.boxcolliders)
	{
		CCollisionLine_Box* pBox = CCollisionLine_Box::Create(MyLib::AABB(box.vtxMin, box.vtxMax), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// 当たり判定ボックス
		m_pCollisionLineBox.push_back(pBox);
	}

#endif

	// 頂点大小計算
	CalVtxMinMax();

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

	// オブジェクトの破棄
	Release();
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

	// オブジェクトの破棄
	Release();
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
		MyLib::Collider_BOX& collider = m_ObstacleInfo.boxcolliders[i];

		collider.TransformOffset(GetWorldMtx());
		box->SetPosition(collider.worldmtx.GetWorldPosition());
		box->SetRotation(GetRotation());
		box->SetAABB(MyLib::AABB(collider.vtxMin, collider.vtxMax), GetScale().x);
		i++;
	}

#endif
}

//==========================================================================
// 描画処理
//==========================================================================
void CMap_Obstacle::Draw()
{
	
}

void CMap_Obstacle::CalVtxMinMax()
{
	m_vtxMin = MyLib::Vector3();
	m_vtxMax = MyLib::Vector3();

	for (auto& obj : m_ObstacleInfo.boxcolliders)
	{
		MyLib::Vector3 vtxMin, vtxMax;
		vtxMin = obj.vtxMin + obj.offset;
		vtxMax = obj.vtxMax + obj.offset;

		// X
		if (m_vtxMin.x > vtxMin.x)
		{// 今の最小値よりも今回の値がちいさかったら

			m_vtxMin.x = vtxMin.x;
		}
		if (m_vtxMax.x < vtxMax.x)
		{// 今の最大値よりも今回の値が大きかったら

			m_vtxMax.x = vtxMax.x;
		}

		// Y
		if (m_vtxMin.y > vtxMin.y)
		{// 今の最小値よりも今回の値がちいさかったら

			m_vtxMin.y = vtxMin.y;
		}
		if (m_vtxMax.y < vtxMax.y)
		{// 今の最大値よりも今回の値が大きかったら

			m_vtxMax.y = vtxMax.y;
		}

		// Z
		if (m_vtxMin.z > vtxMin.z)
		{// 今の最小値よりも今回の値がちいさかったら

			m_vtxMin.z = vtxMin.z;
		}
		if (m_vtxMax.z < vtxMax.z)
		{// 今の最大値よりも今回の値が大きかったら

			m_vtxMax.z = vtxMax.z;
		}
	}
}