//=============================================================================
// 
//  タイマーヘッダー [timer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TIMER_H_
#define _TIMER_H_	// 二重インクルード防止

#include "main.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CMultiNumber;

//==========================================================================
// クラス定義
//==========================================================================
// タイマークラス
class CTimer
{
public:

	enum eState
	{
		STATE_WAIT = 0,		// 待機
		STATE_APPEARANCE,	// 出現
		STATE_ADD_LITTLE,	// 少し加算
		STATE_ADJ,			// 調整
		STATE_BEFORECONTROLL,	// 操作前
		STATE_AFTERCONTROLL,	// 操作後
		STATE_MAX
	};

	CTimer(int nPriority = 6);
	~CTimer();

	static CTimer *Create();
	static CTimer *GetInstance() { return m_pTimer; }

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	float GetTime() { return m_fTime; }

	CTimer::eState GetState() { return m_state; }
	void SetState(eState state) { m_state = state; }

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CTimer::*STATE_FUNC)();
	static STATE_FUNC m_StateFuncList[];	// 状態関数リスト

	//=============================
	// メンバ関数
	//=============================
	void StateWait();
	void StatAppearance();
	void StatAddLittle();
	void StateAdjustment();
	void StateBeforeControll();
	void StateAfterControll();

	//=============================
	// メンバ変数
	//=============================
	MyLib::Vector3 m_pos;			// 位置
	MyLib::Vector3 m_posOrigin;		// 元の位置
	eState m_state;					// 状態
	float m_fStateTime;				// 状態時間
	float m_fTime;					// 時間
	bool m_bAddTime;	// タイマー加算のフラグ
	CMultiNumber* m_pClearTime[3];		// 種類ごとの数字
	static CTimer *m_pTimer;	// 自身のポインタ
};



#endif