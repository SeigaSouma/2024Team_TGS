//=============================================================================
// 
// 判定ゾーン [judgezone.cpp]
//  Author : 石原颯馬
// 
//=============================================================================
#include "judgezone.h"
#include "judge.h"
#include "baggage.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
}

//**************************************************************************
// 
// 判定ゾーンクラス
// 
//**************************************************************************
//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeZone::CJudgeZone()
{
	m_isEnable = true;
	m_pJudge = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudgeZone::~CJudgeZone()
{
	
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CJudgeZone::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudgeZone::Uninit()
{
	if (m_pJudge != nullptr)
	{
		m_pJudge->Uninit();
		m_pJudge = nullptr;
	}

	m_isEnable = false;
}

//==========================================================================
// 生成処理
//==========================================================================
CJudgeZone* CJudgeZone::Create(const float start, const float end)
{
	// メモリの確保
	CJudgeZone* pObj = DEBUG_NEW CJudgeZone;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
		pObj->SetZone(start, end);
		pObj->m_aJudgeInfo[0] = { TYPE_NONE,CJudge::JUDGE::JUDGE_AAA,0 };
		pObj->m_aJudgeInfo[1] = { TYPE_HITNUM,CJudge::JUDGE::JUDGE_AAA,10 };
	}

	return pObj;
}

//==========================================================================
// 確認処理
//==========================================================================
void CJudgeZone::Check()
{
	if (m_pJudge != nullptr)
	{// どっち行くか決まってる
		m_pJudge->Check();
	}
	else
	{// 決まってない
		if (true)
		{// どっち行くか決まった
			std::map<CJudge::JUDGE, int> hitnum;
			hitnum[CJudge::JUDGE::JUDGE_AAA] = 2;
			hitnum[CJudge::JUDGE::JUDGE_BBB] = 4;
			hitnum[CJudge::JUDGE::JUDGE_CCC] = 6;
			hitnum[CJudge::JUDGE::JUDGE_DDD] = 8;

			m_pJudge = CJudge::Create(new CJudgeConditional_HitNum(hitnum));
			m_pJudge->Check();
		}
	}
}

//==========================================================================
// 結果処理
//==========================================================================
CJudge::JUDGE CJudgeZone::Judge()
{
	CJudge::JUDGE judge = CJudge::JUDGE::JUDGE_AAA;
	if (m_pJudge != nullptr)
	{
		judge = m_pJudge->Judge();
	}

	return judge;
}

//==========================================================================
// 判定ゾーン設定処理
//==========================================================================
void CJudgeZone::SetZone(const float start, const float end)
{
	m_zone.start = start;
	m_zone.end = end;
}
