//=============================================================================
// 
//  �����Ԋ֐� [judgestatefunc.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "judgestatefunc.h"
#include "judgeobj.h"
#include "manager.h"
#include "game.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float TIME_APPEAR = 1.0f;		// �o������
	const float TIME_WAIT = 0.4f;		// �҂�����
	const float TIME_FADEOUT = 0.6f;	// �t�F�[�h�A�E�g����
	const MyLib::Vector3 DESTROTATION_GETTOGETHER = MyLib::Vector3(0.0f, 0.0f, D3DX_PI * 0.75f);
	const MyLib::Vector3 DESTPOSITION_GETTOGETHER = MyLib::Vector3(130.0f, 230.0f, 0.0f);
	const MyLib::Vector3 DESTPOSITION_STINGS = MyLib::Vector3(0.0f, 360.0f, 0.0f);
}
//==========================================================================
// �֐��|�C���^
//==========================================================================
CJudgeStateFunc::STATE_FUNC CJudgeStateFunc::m_StateFuncList[] =
{
	&CJudgeStateFunc::StateAppear,
	&CJudgeStateFunc::StateWait,
	&CJudgeStateFunc::StateFadeOut,
};

//**************************************************************************
// ����Ԋ֐�
//**************************************************************************
//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeStateFunc::CJudgeStateFunc(CJudgeObj* pObj)
{
	m_pObj = pObj;
	m_state = CJudgeStateFunc::STATE_APPEAR;
	m_fStateTime = 0.0f;
}

//**************************************************************************
// AAA�����N�ʏ�Ԋ֐�
//**************************************************************************
//==========================================================================
// �o�����
//==========================================================================
void CJudgeStateFunc_AAA::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// �T�C�Y�ݒ�
	CJudgeObj* pObj = GetObj();
	float ratio = stateTime / TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.x * 1.6f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.y * 1.6f, ratio);
	pObj->SetSize(size);
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CJudgeStateFunc_AAA::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
}

//==========================================================================
// ��������
//==========================================================================
void CJudgeStateFunc_AAA::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// �퓬�J�n�ɑJ��
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		return;
	}

	// �s�����x�ݒ�
	pObj->SetAlpha(1.0f - stateTime / TIME_FADEOUT);
}

//**************************************************************************
// BBB�����N�ʏ�Ԋ֐�
//**************************************************************************
//==========================================================================
// �o�����
//==========================================================================
void CJudgeStateFunc_BBB::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// �T�C�Y�ݒ�
	CJudgeObj* pObj = GetObj();
	float ratio = stateTime / TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.x * 1.6f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.y * 1.6f, ratio);
	pObj->SetSize(size);
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CJudgeStateFunc_BBB::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
}

//==========================================================================
// ��������
//==========================================================================
void CJudgeStateFunc_BBB::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// �퓬�J�n�ɑJ��
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		return;
	}

	// �s�����x�ݒ�
	pObj->SetAlpha(1.0f - stateTime / TIME_FADEOUT);
}

//**************************************************************************
// CCC�����N�ʏ�Ԋ֐�
//**************************************************************************
//==========================================================================
// �o�����
//==========================================================================
void CJudgeStateFunc_CCC::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// �T�C�Y�ݒ�
	CJudgeObj* pObj = GetObj();
	float ratio = stateTime / TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.x * 1.6f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.y * 1.6f, ratio);
	pObj->SetSize(size);
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CJudgeStateFunc_CCC::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
}

//==========================================================================
// ��������
//==========================================================================
void CJudgeStateFunc_CCC::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// �퓬�J�n�ɑJ��
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		return;
	}

	// �s�����x�ݒ�
	pObj->SetAlpha(1.0f - stateTime / TIME_FADEOUT);
}

//**************************************************************************
// DDD�����N�ʏ�Ԋ֐�
//**************************************************************************
//==========================================================================
// �o�����
//==========================================================================
void CJudgeStateFunc_DDD::StateAppear()
{
	float stateTime = GetStateTime();
	if (stateTime >= TIME_APPEAR)
	{
		SetStateTime(0.0f);
		SetState(STATE_WAIT);
		return;
	}

	// �T�C�Y�ݒ�
	CJudgeObj* pObj = GetObj();
	float ratio = stateTime / TIME_APPEAR;
	D3DXVECTOR2 size = pObj->GetSize();
	D3DXVECTOR2 sizeOrigin = pObj->GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.x * 1.6f, ratio);
	size.y = UtilFunc::Correction::EasingEaseIn(0.0f, sizeOrigin.y * 1.6f, ratio);
	pObj->SetSize(size);
}

//==========================================================================
// �ҋ@���
//==========================================================================
void CJudgeStateFunc_DDD::StateWait()
{
	float stateTime = GetStateTime();
	if (stateTime >= TIME_WAIT)
	{
		SetStateTime(0.0f);
		SetState(STATE_FADEOUT);
		return;
	}
}

//==========================================================================
// ��������
//==========================================================================
void CJudgeStateFunc_DDD::StateFadeOut()
{
	CJudgeObj* pObj = GetObj();
	float stateTime = GetStateTime();
	if (stateTime >= TIME_FADEOUT)
	{
		SetStateTime(0.0f);
		pObj->Uninit();

		// �퓬�J�n�ɑJ��
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAIN);
		return;
	}

	// �s�����x�ݒ�
	pObj->SetAlpha(1.0f - stateTime / TIME_FADEOUT);
}