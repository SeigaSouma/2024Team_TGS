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
	m_borderHeight = 0.0f;
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
CJudgeZone* CJudgeZone::Create(const float start, const float end, const float borderHeight)
{
	// メモリの確保
	CJudgeZone* pObj = DEBUG_NEW CJudgeZone;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
		pObj->SetZone(start, end);
		pObj->SetBorder(borderHeight);
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
		// 荷物取得
		CListManager<CBaggage> baggageList = CBaggage::GetListObj();
		CBaggage* pBaggage = nullptr;
		baggageList.ListLoop(&pBaggage);

		// 上か下か決める
		CJudge::BORDER border = (pBaggage->GetPosition().y > m_borderHeight) ? CJudge::BORDER::UP : CJudge::BORDER::DOWN;

		// 生成
		if (m_aJudgeInfo[border].type == CJudge::JUDGETYPE::TYPE_NONE)
		{
			m_pJudge = CJudge::Create(new CJudgeConditional_None(m_aJudgeInfo[border].judgeParam));
		}
		else if (m_aJudgeInfo[border].type == CJudge::JUDGETYPE::TYPE_HITNUM)
		{
			m_pJudge = CJudge::Create(new CJudgeConditional_HitNum(m_aJudgeInfo[border].judgeParam));
		}
		
		m_pJudge->Check();
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
