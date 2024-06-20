//=============================================================================
// 
// ����]�[�� [judgezone.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "judgezone.h"
#include "judge.h"
#include "baggage.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

//**************************************************************************
// 
// ����]�[���N���X
// 
//**************************************************************************
//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeZone::CJudgeZone()
{
	m_isEnable = true;
	m_pJudge = nullptr;
	m_borderHeight = 0.0f;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CJudgeZone::~CJudgeZone()
{
	
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CJudgeZone::Init()
{
	return S_OK;
}

//==========================================================================
// �I������
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
// ��������
//==========================================================================
CJudgeZone* CJudgeZone::Create(const float start, const float end, const float borderHeight)
{
	// �������̊m��
	CJudgeZone* pObj = DEBUG_NEW CJudgeZone;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
		pObj->SetZone(start, end);
		pObj->SetBorder(borderHeight);
	}

	return pObj;
}

//==========================================================================
// �m�F����
//==========================================================================
void CJudgeZone::Check()
{
	if (m_pJudge != nullptr)
	{// �ǂ����s�������܂��Ă�
		m_pJudge->Check();
	}
	else
	{// ���܂��ĂȂ�
		// �ו��擾
		CListManager<CBaggage> baggageList = CBaggage::GetListObj();
		CBaggage* pBaggage = nullptr;
		baggageList.ListLoop(&pBaggage);

		// �ォ�������߂�
		CJudge::BORDER border = (pBaggage->GetPosition().y > m_borderHeight) ? CJudge::BORDER::UP : CJudge::BORDER::DOWN;

		// ����
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
// ���ʏ���
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
// ����]�[���ݒ菈��
//==========================================================================
void CJudgeZone::SetZone(const float start, const float end)
{
	m_zone.start = start;
	m_zone.end = end;
}
