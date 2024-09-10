//=============================================================================
//
// キーコンフィグ設定ヘッダー [keyconfig_setting.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_SETTING_H_
#define _KEYCONFIG_SETTING_H_	// 二重インクルード防止

#include "keyconfig.h"

// 前方宣言
class CControlKeyDisp;
class CScroll;

//==========================================================================
// クラス定義
//==========================================================================
// キーコンフィグ
class CKeyConfigSetting
{
private:

	// キー情報
	struct KEYINFO
	{
		CObject2D* s_p2D;				// 2Dポリゴン
		CObject2D* s_p2DFront;			// 表面
		CControlKeyDisp* s_pKeyDisp;	// キーコンフィグ表示

		// コンストラクタ
		KEYINFO() { s_p2D = nullptr; s_pKeyDisp = nullptr; s_p2DFront = nullptr; }
	};

	// 切り替え情報
	struct CHENGEINFO
	{
		CObject2D* s_p2D;				// 2Dポリゴン
		CObject2D* s_p2DFront;			// 表面
		CControlKeyDisp* s_pKeyDispOK;	// キーコンフィグ表示
		CControlKeyDisp* s_pKeyDispNO;	// キーコンフィグ表示

		// コンストラクタ
		CHENGEINFO() { s_p2D = nullptr; s_pKeyDispOK = nullptr; s_pKeyDispNO = nullptr; s_p2DFront = nullptr; }
	};

public:

	// コンストラクタ
	CKeyConfigSetting();
	~CKeyConfigSetting() {}

	//=============================
	// メンバ関数
	//============================= 
	static CKeyConfigSetting* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool IsDeath(void) { return m_bDeath; }
	bool IsChange(void) { return m_bNowChange; }

private:

	// メンバ関数
	void Chenge();
	void SetAlpha();

	// メンバ変数
	float m_Alpha;
	float m_fTime;
	int m_SelectKey;	// 選択中
	bool m_bNowChange;	// 変更中か否か
	bool m_bDeath;		// 死亡フラグ
	KEYINFO m_aKeyConfig[INGAME::ACT_MAX];	// キーコンフィグ用
	CHENGEINFO m_checkconfig;	// 決定戻る変更用
	CObject2D* m_pTitle2D;		// 説明用ポリゴン
	CScroll* m_pScroll;
};

#endif