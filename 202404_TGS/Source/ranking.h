//=============================================================================
// 
//  ランキングヘッダー [ranking.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RANKING_H_
#define _RANKING_H_	// 二重インクルード防止

#include "main.h"
#include "manager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CRankingScore;
class CRankingItem;
//==========================================================================
// クラス定義
//==========================================================================
// ランキングクラス定義
class CRanking : public CScene
{
public:

	CRanking();
	~CRanking();

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static void SetEnableArrival();
private:

	static bool m_bAllArrival;		// 全て到着した判定
	static CRankingScore *m_pRankingScore;	// ランキングスコアのオブジェクト
	CRankingItem* m_pRanking[10];
};



#endif