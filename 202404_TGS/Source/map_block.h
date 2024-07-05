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

	//=============================
	// 静的関数
	//=============================
	static CListManager<CMapBlock>* Create();	// 生成
	static void Kill();	// 開放
	static CListManager<CMapBlock>* GetList() { m_List; }

private:

	//=============================
	// メンバ関数
	//=============================
	void Load(std::ifstream* pFile);	// ロード
	void ObstacleLoad(std::ifstream* pFile); // 障害物読み込み

	//=============================
	// メンバ変数
	//=============================
	static CListManager<CMapBlock> m_List;	// リスト

	// 配置情報リスト
	CListManager<CMap_Obstacle> m_ObstacleList = {};	// 障害物
	CListManager<CCheckpoint> m_CheckpointList = {};	// チェックポイント
};

#endif
