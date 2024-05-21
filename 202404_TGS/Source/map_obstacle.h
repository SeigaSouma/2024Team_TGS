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
public:
	
	//=============================
	// 列挙型定義
	//=============================

	CMap_Obstacle(int nPriority = 6);
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
	void Kill();	// 削除
	void Save();	// セーブ
	void Load();	// ロード


	//=============================
	// 静的関数
	//=============================
	static CMap_Obstacle *Create(const CMap_ObstacleManager::SObstacleInfo& info);	// 生成処理
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
	static CListManager<CMap_Obstacle> m_List;	// リスト

};


#endif