//=============================================================================
// 
//  サンプル_オブジェクト2Dヘッダー [sample_obj2D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SAMPLE_OBJ2D_H_
#define _SAMPLE_OBJ2D_H_	// 二重インクルード防止

#include "object2d.h"

//==========================================================================
// クラス定義
//==========================================================================
// サンプル_オブジェクト2Dクラス
class CSample_Obj2D : public CObject2D
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

	CSample_Obj2D(int nPriority = 7);
	~CSample_Obj2D();

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
	static CSample_Obj2D* Create();

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
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	// その他変数

};


#endif