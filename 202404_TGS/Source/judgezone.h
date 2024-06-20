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
#include <map>

//==========================================================================
// クラス定義
//==========================================================================
//**************************************************************************
// 判定ゾーン全体
//**************************************************************************
class CJudgeZone
{
public:
	// 始点終点構造体
	struct SZone
	{
		float start;	// 始点
		float end;	// 終点
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
	void SetBorder(const float borderHeight) { m_borderHeight = borderHeight; }
	void SetInfo(CJudge::BORDER border, CJudge::SJudgeInfo info) { m_aJudgeInfo[border] = info; }
	bool IsEnable() { return m_isEnable; }

	//=============================
	// 静的関数
	//=============================
	static CJudgeZone* Create(const float start, const float end, const float borderHeight);	// 生成

private:
	SZone m_zone;
	CJudge* m_pJudge;
	bool m_isEnable;
	CJudge::SJudgeInfo m_aJudgeInfo[2];
	float m_borderHeight;
};

#endif