//=============================================================================
// 
//  アーチ状の魚障害物ヘッダー [obstacle_fisharch.h]
//  Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _OBSTACLE_FISHARCH_H_
#define _OBSTACLE_FISHARCH_H_		// 二重インクルード防止

#include "map_obstacle.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物クラス
class CObstacle_FishArch : public CMap_Obstacle
{
public:

	CObstacle_FishArch(int nPriority = 5,
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CObstacle_FishArch();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Kill() override;

	//=============================
	// メンバ関数
	//=============================
	CObstacle_FishArch* Create(const CMap_ObstacleManager::SObstacleInfo& info);
	

	//=============================
	// 静的関数
	//=============================

private:


	//=============================
	// メンバ関数
	//=============================
	// その他

	//=============================
	// メンバ変数
	//=============================
	CMap_ObstacleManager::SObstacleInfo m_Info;	// 使用する魚の情報
	std::vector<CMap_Obstacle> m_FishList;	// 魚のリスト

};


#endif
