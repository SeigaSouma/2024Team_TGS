//======================================================
//
//リザルトシーンのヘッダ[result.h]
//Author:石原颯馬
//
//======================================================
#ifndef _RESULT_H_
#define _RESULT_H_
#include "..\..\manager\manager.h"

//前方宣言
class CPause;
class CFade;

//仮
class CGoal;

//タイトルクラス
class CResult : public CScene
{
public:
	//コンストラクタ・デストラクタ
	CResult();
	~CResult();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CPause* m_pPause;	//ポーズ
	CFade* m_pFade;		//フェード
};

#endif // !_GAME_H_
