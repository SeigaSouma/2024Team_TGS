//======================================================
//
//ランキングシーンのヘッダ[ranking.h]
//Author:石原颯馬
//
//======================================================
#ifndef _RANKING_H_
#define _RANKING_H_
#include "..\..\manager\manager.h"

//前方宣言
class CFade;

//タイトルクラス
class CRanking : public CScene
{
public:
	//静的const
	static const int MAX_RANK;

	//コンストラクタ・デストラクタ
	CRanking();
	~CRanking();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定
	static void Set(const int nScore);

private:
	static void Load(int* pRanking);
	static void Save(int* pRanking);

	//オブジェ（いったんスコア類すべて投げっぱで）
	CFade* m_pFade;
};

#endif // !_RANKING_H_
