//=============================================================================
// 
//  マップの障害物マネージャヘッダー [map_obstacleManager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MAP_OBSTACLEMANAGER_H_
#define _MAP_OBSTACLEMANAGER_H_		// 二重インクルード防止

#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// マップの障害物マネージャクラス
class CMap_ObstacleManager
{
public:

	//=============================
	// 障害物情報
	//=============================
	// セットアップ情報
	struct SObstacleSetup
	{
		int isAir;		// 空気貫通フラグ
		int isMove;		// 移動フラグ

		SObstacleSetup() : isAir(0), isMove(0) {}
	};

	// 障害物情報
	struct SObstacleInfo
	{
		SObstacleSetup setup;		// セットアップ情報
		std::string textFile;		// テキストファイル
		std::string modelFile;		// モデルファイル
		std::string colliderFile;	// コライダーファイル
		std::vector<MyLib::Collider_BOX> boxcolliders;	// BOXコライダー
		
		SObstacleInfo() : setup(), modelFile(), colliderFile(), boxcolliders() {}

		// JSONからの読み込み
		void from_json(const json& j)
		{
			for (const auto& colliderData : j.at("colliders"))
			{
				MyLib::Collider_BOX collider;
				collider.from_json(colliderData);
				boxcolliders.push_back(collider);
			}
		}

		// JSONへの書き込み
		void to_json(json& j) const
		{
			j["colliders"] = json::array(); // 空の配列を作成

			for (const auto& collider : boxcolliders)
			{
				json colliderJson;
				collider.to_json(colliderJson);

				j["colliders"].emplace_back(colliderJson);
			}
		}
	};



	CMap_ObstacleManager();
	~CMap_ObstacleManager();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	//=============================
	// メンバ関数
	//=============================
	void Kill();	// 削除
	void Save();	// セーブ
	void SaveInfo(const std::string& file);	// 情報セーブ
	void Load();	// ロード
	void LoadInfo(const std::string& file);	// 情報ロード

	//=============================
	// 静的関数
	//=============================
	static CMap_ObstacleManager *Create();

private:

	//=============================
	// メンバ変数
	//=============================
	std::vector<SObstacleInfo> m_ObstacleInfo;	// 障害物情報
};


#endif