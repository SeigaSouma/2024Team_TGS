//=============================================================================
// 
//  タイマーヘッダー [timer_result.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TIMER_RESULT_H_
#define _TIMER_RESULT_H_	// 二重インクルード防止

#include "timer.h"

//==========================================================================
// 前方宣言
//==========================================================================

//==========================================================================
// クラス定義
//==========================================================================
// タイマークラス
class CTimer_Result : public CTimer
{
public:

	CTimer_Result(int nPriority = 6);
	~CTimer_Result();

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

private:

	void CreateText();	// 文字生成

	CObject2D* m_pText;		// 文字

};



#endif