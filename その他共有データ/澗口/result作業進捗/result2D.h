//=============================================================================
// 
//  リザルト2Dヘッダー[result2D.h]
//  Author : 澗口将太郎
// 
//=============================================================================

#ifndef _RESULT2D_H_
#define _RESULT2D_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// 戦果クラス
class CResult2D : public CObject
{
public:

	//列挙型
	typedef enum Items
	{
		ITEM_RANK = 0,
		ITEM_TIME,
		ITEM_SCORE,
		ITEM_ALLRANK,
		ITEM_RANKING_UPD,
		ITEM_MAX

	};

	CResult2D(int nPriority = 11);
	~CResult2D();

	// オーバーライドされた関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// 削除
	static CResult2D* Create();	// 生成処理

private:
	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;
	CObject2D* m_pLogo;
	CObject2D* m_pBg;
	CObject2D* m_pItem[5];
};


#endif