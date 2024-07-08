//=============================================================================
// 
//  マップブロック管理ヘッダー [map_block.h]
//  Author : Ibuki Okusada
// 
//=============================================================================

#ifndef _MAPBLOCK_H_
#define _MAPBLOCK_H_		// 二重インクルード防止

#include "listmanager.h"

// 前方宣言
class CCheckpoint;
class CMap_Obstacle;

//==========================================================================
// クラス定義
//==========================================================================
// マップブロック配置情報まとめクラス
class CMapBlockInfo
{
public:
	// 構造体情報
	struct SObsacleInfo
	{
		MyLib::Vector3 pos;		// 座標
		MyLib::Vector3 rot;		// 向き
		MyLib::Vector3 scale;	// スケール
		int nType;				// モデル種類

		// コンストラクタ
		SObsacleInfo() : pos(0.0f), rot(0.0f), scale(1.0f), nType(0) {}
	};

	CMapBlockInfo();
	~CMapBlockInfo();

	HRESULT Init();
	void Uninit();
	void Load(std::ifstream* pFile);	// ロード
	void ObstacleLoad(std::ifstream* pFile); // 障害物読み込み
	std::vector<SObsacleInfo> GetObstacleInfo() { return m_ObstacleList; }
	std::vector<float> GetCheckpointInfo() { return m_CheckpointList; }

private:

	// 配置情報リスト
	std::vector<SObsacleInfo> m_ObstacleList;	// 障害物
	std::vector<float> m_CheckpointList;	// チェックポイント
	int m_nLevel;
};

// マップブロック管理クラス
class CMapBlock
{
public:

	CMapBlock();	// コンストラクタ
	~CMapBlock();	// デストラクタ

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	// 入出力
	void Save();	// セーブ
	
	void Set(const int Idx, const MyLib::Vector3& startpos, float startlength);
	CListManager<CMap_Obstacle> GetObstacleList() { return m_ObstacleList; }
	CListManager<CCheckpoint> GetCheckpointList() { return m_CheckpointList; }


	//=============================
	// 静的関数
	//=============================
	static CListManager<CMapBlockInfo>* Load();	// 読み込み
	static void Kill();	// 開放
	static CListManager<CMapBlock> GetList() { return m_List; }
	static CListManager<CMapBlockInfo> GetInfoList() { return m_InfoList; }	// 配置情報リスト取得

private:

	//=============================
	// メンバ関数
	//=============================

	//=============================
	// メンバ変数
	//=============================
	static CListManager<CMapBlock> m_List;	// リスト
	static CListManager<CMapBlockInfo> m_InfoList;	// 配置情報リスト

	// 配置情報リスト
	CListManager<CMap_Obstacle> m_ObstacleList = {};	// 障害物
	CListManager<CCheckpoint> m_CheckpointList = {};	// チェックポイント
};

#endif
