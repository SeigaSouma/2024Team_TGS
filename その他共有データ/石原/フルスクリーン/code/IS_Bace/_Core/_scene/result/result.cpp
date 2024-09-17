//======================================================
//
//���U���g�V�[��[result.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "result.h"
#include "..\pause\pause.h"
#include "..\result\result.h"
#include "..\ranking\ranking.h"
#include "..\..\manager\manager.h"
#include "..\..\texture\texture.h"
#include "..\..\camera\camera.h"
#include "..\..\sound\sound.h"
#include "..\..\_object\object_empty\object.h"
#include "..\..\input\input.h"
#include "..\..\..\_Expansion\fade\fade.h"

//=================================
//�R���X�g���N�^
//=================================
CResult::CResult()
{
	m_pPause = nullptr;
	m_pFade = nullptr;
}

//=================================
//�f�X�g���N�^
//=================================
CResult::~CResult()
{
}

//=================================
//������
//=================================
HRESULT CResult::Init(void)
{
	//�J�������Z�b�g
	CManager::GetInstance()->CManager::GetInstance()->GetCamera()->ResetPos();

	return S_OK;
}

//=================================
//�I��
//=================================
void CResult::Uninit(void)
{
	//�I�u�W�F�S�j��
	for (int cnt = 0; cnt < CObject::PRIORITY_FADE; cnt++)
	{
		CObject::ReleaseAll(cnt);
	}

	//���~�߂�
	CManager::GetInstance()->GetSound()->Stop();
}

//=================================
//�X�V
//=================================
void CResult::Update(void)
{
	CManager* pIns = CManager::GetInstance();
	CInputKeyboard* pKeyboard = pIns->GetInputKeyboard();
	CInputGamePad* pResultpad = pIns->GetInputGamePad();
	bool bPush = false;

	if (pResultpad != nullptr && pResultpad->IsConnect() == true)
	{//�Q�[���p�b�h�ڑ�
		if (pResultpad->GetTrigger(XINPUT_GAMEPAD_A))
		{
			bPush = true;
		}
	}
	else
	{//���ڑ�
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			bPush = true;
		}
	}

	if (m_pFade == nullptr && bPush == true)
	{//�X�y�[�X�����ꂽ
		m_pFade = CFade::Create(CScene::MODE_TUTORIAL);
	}
}

//=================================
//�`��
//=================================
void CResult::Draw(void)
{
	//���i�͂��ׂ�CObject�N���X�p�����Ă���̂������Ă���̂Ŏ����`��
	//�|�[�Y�̒��g�����ׂ�CObject�N���X�p�����Ă�̂Ŏ����`��
	//����Ă����ł��邱�Ƃ͂Ȃ�
}