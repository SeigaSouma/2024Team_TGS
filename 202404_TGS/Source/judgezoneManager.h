//=============================================================================
// 
//  判定ゾーンマネージャヘッダー [judgezoneManager.h]
//  Author : 石原颯馬
// 
//=============================================================================
#ifndef _JUDGE_ZONE_MANAGER_H_
#define _JUDGE_ZONE_MANAGER_H_		// 二重インクルード防止
#include "listmanager.h"

// 前方宣言
class CJudgeZone;

//==========================================================================
// クラス定義
//==========================================================================
// 判定ゾーンマネージャクラス
class CJudgeZoneManager
{
public:
	CJudgeZoneManager();
	~CJudgeZoneManager();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// メンバ関数
	//=============================
	void Check(float progress);
	void Add(CJudgeZone* zone) { m_zoneList.Regist(&zone); }

	//=============================
	// 静的関数
	//=============================
	static CJudgeZoneManager* Create();
	static CJudgeZoneManager* GetInstance() { return m_ThisPtr; }	// インスタンス取得

private:
	CListManager<CJudgeZone*> m_zoneList;
	static CJudgeZoneManager* m_ThisPtr;		// 自身のポインタ
};

#endif