//=============================================================================
// 
//  ランキング2Dヘッダー[rankingItem.h]
//  Author : 澗口将太郎
// 
//=============================================================================

#ifndef _RANKINGITEM_H_
#define _RANKINGITEM_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// 戦果クラス
class CRankingItem : public CObject
{
public:

	//列挙型
	typedef enum Items
	{
		ITEM_NUM_RANK = 0,
		ITEM_ALLRANK,
		ITEM_SCORE,
		ITEM_ALLRANK_DATA,
		ITEM_MAX
	};

	CRankingItem(int nPriority = 11);
	~CRankingItem();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// 削除
	static CRankingItem* Create(int nNumRank,int nScore,int nAllRank);	// 生成処理

private:

	
	/*void SetScoreData()*/
	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;
	CObject2D* m_pItem[ITEM_MAX];	//項目数
	CObject2D* m_pScoreItem[8];		//スコアの桁毎obj
	int m_nNumRank;					//順位
	int m_nScore;					//スコア値
	int m_nAllRank;					//総評の値
	static int m_nObjNum;			//当オブジェクトの生成数
};


#endif