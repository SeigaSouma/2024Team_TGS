//=============================================================================
// 
//  マップのモーションする障害物 [map_obstacle_motion.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "map_obstacle_motion.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "collisionLine_Box.h"
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
CListManager<CMap_Obstacle_Motion> CMap_Obstacle_Motion::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CMap_Obstacle_Motion::CMap_Obstacle_Motion(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CMap_Obstacle_Motion::~CMap_Obstacle_Motion()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CMap_Obstacle_Motion *CMap_Obstacle_Motion::Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange, const bool bSave)
{
	CMap_Obstacle_Motion* pObj = nullptr;
	// メモリの確保
	pObj = DEBUG_NEW CMap_Obstacle_Motion;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
		pObj->SetSave(bSave);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMap_Obstacle_Motion::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// キャラ生成
	m_pChara = CObjectChara::Create(m_ObstacleInfo.modelFile);
	m_pChara->GetMotion()->Set(0);
	m_pChara->CObject::SetType(TYPE_OBJECTX);

	m_OriginObstacleInfo = m_ObstacleInfo;	// 障害物情報

	return CMap_Obstacle::Init();
}

//==========================================================================
// 終了処理
//==========================================================================
void CMap_Obstacle_Motion::Uninit()
{
	if (m_pChara != nullptr)
	{
		m_pChara = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CMap_Obstacle_Motion::Kill()
{
	if (m_pChara != nullptr)
	{
		m_pChara->Uninit();
		m_pChara = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CMap_Obstacle::Kill();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMap_Obstacle_Motion::Update()
{
	CMap_Obstacle::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CMap_Obstacle_Motion::Draw()
{
	CMap_Obstacle::Draw();
}

//==========================================================================
// 位置設定
//==========================================================================
void CMap_Obstacle_Motion::SetPosition(const MyLib::Vector3& pos)
{
	CObject::SetPosition(pos);
	m_pChara->SetPosition(pos);
}

//==========================================================================
// 角度設定
//==========================================================================
void CMap_Obstacle_Motion::SetRotation(const MyLib::Vector3& rot)
{
	CObject::SetRotation(rot);
	m_pChara->SetRotation(rot);
}