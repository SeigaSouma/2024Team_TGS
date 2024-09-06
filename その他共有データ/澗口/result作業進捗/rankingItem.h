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
	static CRankingItem* Create();	// 生成処理

private:

	
	/*void SetScoreData()*/
	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;
	CObject2D* m_pItem[ITEM_MAX];
};


#endif