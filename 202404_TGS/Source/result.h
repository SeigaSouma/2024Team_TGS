//=============================================================================
// 
//  リザルトヘッダー [result.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RESULT_H_
#define _RESULT_H_	// 二重インクルード防止

#include "main.h"
#include "manager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CResultScore;
class CTimer;

//==========================================================================
// クラス定義
//==========================================================================
// リザルトクラス定義
class CResult : public CScene
{
public:

	CResult();
	~CResult();

	// オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

	static CResultScore *GetResultScore();
	static void SetEnableArrival();

private:

	// メンバ変数
	bool m_clear; // クリア判定

	CTimer* m_pTimer;	// タイマーのオブジェクト
	static bool m_bAllArrival;		// 全て到着した判定
	static CResultScore *m_pResultScore;
};



#endif