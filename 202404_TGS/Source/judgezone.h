//=============================================================================
// 
//  判定ゾーンヘッダー [judgezone.h]
//  Author : 石原颯馬
// 
//=============================================================================
#ifndef _JUDGE_ZONE_H_
#define _JUDGE_ZONE_H_		// 二重インクルード防止
#include "listmanager.h"
#include "judge.h"

//==========================================================================
// クラス定義
//==========================================================================
//**************************************************************************
// 判定ゾーン全体
//**************************************************************************
class CJudgeZone
{
public:
	// 判定種類列挙
	enum JUDGETYPE
	{
		TYPE_NONE = 0,
		TYPE_HITNUM,
	};

	// 始点終点構造体
	struct SZone
	{
		float start;	// 始点
		float end;	// 終点
	};

	// 判定情報構造体
	struct SJudgeInfo
	{
		JUDGETYPE type;
		CJudge::JUDGE judge;
		int hitnum;
	};

	CJudgeZone();
	virtual ~CJudgeZone();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// メンバ関数
	//=============================
	virtual void Check();
	virtual CJudge::JUDGE Judge();

	// 取得・設定
	SZone GetZone() { return m_zone; }
	void SetZone(const SZone zone) { m_zone = zone; }
	void SetZone(const float start, const float end);
	bool IsEnable() { return m_isEnable; }

	//=============================
	// 静的関数
	//=============================
	static CJudgeZone* Create(const float start, const float end);	// 生成

private:
	SZone m_zone;
	CJudge* m_pJudge;
	bool m_isEnable;
	SJudgeInfo m_aJudgeInfo[2];
};

#endif