//=============================================================================
// 
//  水しぶきヘッダー [splashwater.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SPLASHWATER_H_
#define _SPLASHWATER_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水しぶきクラス
class CSplashwater : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_MAX
	};

	CSplashwater(int nPriority = 7);
	~CSplashwater();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// 静的関数
	//=============================
	static CSplashwater* Create();

private:

	//=============================
	// メンバ関数
	//=============================
	// 状態系

	// その他関数

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fLife;		// 寿命
	float m_fOriginLife;		// 寿命
};


#endif