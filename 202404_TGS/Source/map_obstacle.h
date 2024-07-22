//=============================================================================
// 
//  マップの障害物ヘッダー [map_obstacle.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MAP_OBSTACLE_H_
#define _MAP_OBSTACLE_H_		// 二重インクルード防止

#include "object.h"
#include "objectX.h"
#include "listmanager.h"
#include "map_obstacleManager.h"
#include "collisionLine_Box.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物クラス
class CMap_Obstacle : public CObject
{
private:

	enum T
	{
		SAKANA = 12,	// 魚
		BIRD = 13,		// 鳥
	};

public:

	CMap_Obstacle(int nPriority = 5, 
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CMap_Obstacle();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	virtual void Kill();	// 削除
	void Save();	// セーブ
	void Load();	// ロード
	bool GetSave() { return m_bSave; }
	void SetSave(const bool bSave) { m_bSave = bSave; }
	CMap_ObstacleManager::SObstacleInfo GetObstacleInfo() { return m_ObstacleInfo; }	// 障害物情報取得
	void SetObstacleInfo(const CMap_ObstacleManager::SObstacleInfo& info) { m_ObstacleInfo = info; }	// 障害物情報設定
	virtual MyLib::Matrix GetWorldMtx() { return MyLib::Matrix(); }
	virtual MyLib::Vector3 GetScale() = 0;
	virtual void CalWorldMtx() {}
	virtual void SetPosition(const MyLib::Vector3& pos) { CObject::SetPosition(pos); }		// 位置設定
	virtual void SetRotation(const MyLib::Vector3& rot) { CObject::SetRotation(rot); }		// 向き設定
	virtual void SetScale(const MyLib::Vector3& scale) = 0;
	virtual MyLib::Vector3 GetVtxMin() { return m_vtxMin; };			// 頂点の最小値取得
	virtual MyLib::Vector3 GetVtxMax() { return m_vtxMax; };			// 頂点の最大値取得
	virtual void SetState(CObjectX::STATE state){}
	std::vector<CCollisionLine_Box*> GetCollisionLineBox() { return m_pCollisionLineBox; }

	//=============================
	// 静的関数
	//=============================
	static CMap_Obstacle *Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange = true, const bool bSave = true);	// 生成処理
	static CListManager<CMap_Obstacle> GetListObj() { return m_List; }				// リスト取得
	static void ListRegist(CMap_Obstacle* pObstacle) { m_List.Regist(pObstacle); }

private:

	
	//=============================
	// メンバ関数
	//=============================
	// その他
	void CalVtxMinMax();

	//=============================
	// メンバ変数
	//=============================
	TYPE m_type;			// 種類
	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// 障害物情報
	CMap_ObstacleManager::SObstacleInfo m_OriginObstacleInfo;	// 障害物情報
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// 当たり判定ボックス
	MyLib::Vector3 m_vtxMin, m_vtxMax;
	static CListManager<CMap_Obstacle> m_List;	// リスト
	bool m_bSave;			// 保存するかどうか

};


#endif