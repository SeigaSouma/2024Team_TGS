//=============================================================================
// 
// タイトル選択 [title_select.h]
// Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _TITLE_SELECT_H_
#define _TITLE_SELECT_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"

class CObject2D;

//==========================================================================
// クラス定義
//==========================================================================
// タイトルエンター
class CTitle_Select
{
public:

	//=============================
	// 列挙型定義
	//=============================
	enum STATE
	{
		STATE_NONE = 0,			// なにもなし
		STATE_FADEIN,			// フェードイン
		STATE_FADEOUT,			// フェードアウト
		STATE_TUTORIAL_FADEOUT,	// チュートリアル確認のフェードアウト
		STATE_NOACTIVE,			// 反応しない
		STATE_SETTING,			// 設定中
		STATE_MAX
	};

	enum SELECT
	{
		SELECT_START,	// 開始
		SELECT_OPTION,	// 設定
		SELECT_MAX
	};

	CTitle_Select(float fadetime);
	~CTitle_Select() {}

	//  オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void SetDraw(const bool bDraw = true);

	void SetState(STATE state);
	STATE GetState() { return m_state; }

	static CTitle_Select* Create(float fadetime);	// 生成処理

private:

	//=============================
	// 関数ポインタ
	//=============================
	typedef void(CTitle_Select::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	void StateNone();			// なにもなし
	void StateFadeIn();			// フェードイン
	void StateFadeOut();			// フェードアウト
	void StateTutorial_FadeOut();		// チュートリアル確認のフェードアウト
	void StateNoActive();		// 反応しない
	void StateSetting();		// 反応しない

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;				// 状態
	float m_fStateTime;			// 状態カウンター
	const float m_fFadeOutTime;	// フェードにかかる時間
	CObject2D* m_ap2D[SELECT_MAX];	// 選択肢
	CObject2D* m_pSelect;			// 背景筆
	int m_nSelect;				// 
	bool m_bPress;
};

#endif