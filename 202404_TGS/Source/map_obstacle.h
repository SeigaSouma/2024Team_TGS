//=============================================================================
// 
//  マップの障害物ヘッダー [map_obstacle.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MAP_OBSTACLE_H_
#define _MAP_OBSTACLE_H_		// 二重インクルード防止

#include "objectX.h"
#include "listmanager.h"
#include "map_obstacleManager.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物クラス
class CMap_Obstacle : public CObjectX
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
	CMap_ObstacleManager::SObstacleInfo GetObstacleInfo() { return m_ObstacleInfo; }	// 障害物情報取得
	void SetObstacleInfo(const CMap_ObstacleManager::SObstacleInfo& info) { m_ObstacleInfo = info; }	// 障害物情報設定

	//=============================
	// 静的関数
	//=============================
	static CMap_Obstacle *Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange = true, const bool bSave = true);	// 生成処理
	static CListManager<CMap_Obstacle> GetListObj() { return m_List; }				// リスト取得

private:

	
	//=============================
	// メンバ関数
	//=============================
	// その他

	//=============================
	// メンバ変数
	//=============================
	TYPE m_type;			// 種類
	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// 障害物情報
	CMap_ObstacleManager::SObstacleInfo m_OriginObstacleInfo;	// 障害物情報
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// 当たり判定ボックス
	static CListManager<CMap_Obstacle> m_List;	// リスト
	bool m_bSave;			// 保存するかどうか

};


#endif