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
CJudgeZone* CJudgeZone::Create(const float start, const float end)
{
	// �������̊m��
	CJudgeZone* pObj = DEBUG_NEW CJudgeZone;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
		pObj->SetZone(start, end);
		pObj->m_aJudgeInfo[0] = { TYPE_NONE,CJudge::JUDGE::JUDGE_AAA,0 };
		pObj->m_aJudgeInfo[1] = { TYPE_HITNUM,CJudge::JUDGE::JUDGE_AAA,10 };
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
		if (true)
		{// �ǂ����s�������܂���
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
