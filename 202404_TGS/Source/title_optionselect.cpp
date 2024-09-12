//=============================================================================
// 
//  �^�C�g���̃I�v�V�����I�������� [title_optionselect.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "title_optionselect.h"
#include "title_select.h"
#include "title_pressenter.h"
#include "title.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "object2D.h"
#include "scroll.h"
#include "keyconfig_setting.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_ROLL = "data\\TEXTURE\\scroll\\scroll_roll.png";		// ���[�������̃e�N�X�`��
	const std::string TEXTURE_PAPER = "data\\TEXTURE\\scroll\\scroll_paper.png";	// �������̃e�N�X�`��
	const std::string TEXTURE_EDGE = "data\\TEXTURE\\scroll\\scroll_edge.png";		// �[�����̃e�N�X�`��
	const float SIZE_Y = 80.0f;	// ��T�C�YY
}

namespace StateTime	// ��ԕʎ���
{
	const float FADE = 0.3f;	// �t�F�[�h
}


//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitle_OptionSelect::STATE_FUNC CTitle_OptionSelect::m_StateFunc[] =
{
	&CTitle_OptionSelect::StateNone,		// �Ȃ�
	&CTitle_OptionSelect::StateScrollWait,	// �������҂�
	&CTitle_OptionSelect::StateScrollWait_Close,	// �������҂�
	&CTitle_OptionSelect::StateSelect,		// �I��
	&CTitle_OptionSelect::StateEdit,		// �ҏW
	&CTitle_OptionSelect::StateFadeIn,		// �t�F�[�h�C��
	&CTitle_OptionSelect::StateFadeOut,		// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitle_OptionSelect::CTitle_OptionSelect(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_state = STATE::STATE_NONE;			// ���
	m_fStateTimer = 0.0f;					// ��ԃ^�C�}�[
	m_select = Select::SELECT_KEYKONFIG;	// �I����
	
	memset(m_pSelect, 0, sizeof(m_pSelect));	// �I�����̃I�u�W�F�N�g
	m_pScroll = nullptr;					// ������

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitle_OptionSelect::~CTitle_OptionSelect()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitle_OptionSelect* CTitle_OptionSelect::Create()
{
	// �������̊m��
	CTitle_OptionSelect* pObj = DEBUG_NEW CTitle_OptionSelect();

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitle_OptionSelect::Init()
{
	
	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	SetPosition(MyLib::Vector3(200.0f, 200.0f, 0.0f));

	
	// �I��������
	CreateSelect();

	// ���
	SetState(STATE::STATE_NONE);

	return S_OK;
}

//==========================================================================
// �I��������
//==========================================================================
void CTitle_OptionSelect::CreateSelect()
{
	MyLib::Vector3 pos = GetPosition();

	// �e�N�X�`���擾
	CTexture* pTexture = CTexture::GetInstance();

	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		// ����
		m_pSelect[i] = CObject2D::Create();
		if (m_pSelect[i] == nullptr) continue;

		CObject2D* pObj2D = m_pSelect[i];

		// ��ނ̐ݒ�
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// �e�N�X�`���ݒ�
		int texID = pTexture->Regist(TEXTURE_ROLL);
		pObj2D->BindTexture(texID);

		// �T�C�Y�ݒ�
		D3DXVECTOR2 size = pTexture->GetImageSize(texID);

		// �c�������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_Y);
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);

		// �ʒu�ݒ�
		pObj2D->SetPosition(pos + MyLib::Vector3(0.0f, (SIZE_Y * 2.0f) * i, 0.0f));
	}
}

//==========================================================================
// �I������
//==========================================================================
void CTitle_OptionSelect::Uninit()
{
	

	// �I������
	CObject::Release();
}

//==========================================================================
// �폜
//==========================================================================
void CTitle_OptionSelect::Kill()
{
	for (const auto& select : m_pSelect)
	{
		select->Uninit();
	}

	// �I������
	CObject::Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitle_OptionSelect::Update()
{

	// ��ԍX�V
	UpdateState();

	if (IsDeath()) return;
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTitle_OptionSelect::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitle_OptionSelect::StateNone()
{
	
}

//==========================================================================
// �������҂�
//==========================================================================
void CTitle_OptionSelect::StateScrollWait()
{
	if (m_pScroll == nullptr)
	{
		m_pScroll = CScroll::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), 0.1f, 400.0f, 1200.0f, false, true, 2);
	}

	// �X�N���[���I��
	if (m_pScroll->GetState() == CScroll::STATE::STATE_WAIT)
	{
		SetState(STATE::STATE_FADEIN);
	}
}

//==========================================================================
// �������҂�(��)
//==========================================================================
void CTitle_OptionSelect::StateScrollWait_Close()
{
	if (m_pScroll == nullptr ||
		(m_pScroll != nullptr && m_pScroll->GetState() != CScroll::STATE::STATE_FADEOUT)) return;


	// ������
	if (m_pScroll != nullptr)
	{
		m_pScroll = nullptr;
	}


	// �^�C�g���擾
	CTitle* pTitle = CTitle::GetInstance();

	// �V�[���ݒ�
	pTitle->SetSceneType(CTitle::SCENETYPE::SCENETYPE_NONE);

	// �I�����̏�Ԑݒ�
	CTitle_Select* pSelect = pTitle->GetTitlePressEnter()->GetSelect();
	pSelect->SetState(CTitle_Select::STATE::STATE_FADEIN);

}

//==========================================================================
// �I��
//==========================================================================
void CTitle_OptionSelect::StateSelect()
{
	// ���͏��擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// ��
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pKey->GetTrigger(DIK_W))
	{
		// �p�^�[��No.���X�V
		m_select = static_cast<Select>((m_select + (Select::SELECT_MAX - 1)) % Select::SELECT_MAX);
	}

	// ��
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y < 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pKey->GetTrigger(DIK_S))
	{
		// �p�^�[��No.���X�V
		m_select = static_cast<Select>(((int)m_select + 1) % Select::SELECT_MAX);
	}


	// ���j���[��
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x > 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{
		m_select = Select::SELECT_MAX;

		// �ҏW���
		SetState(STATE::STATE_EDIT);
	}

	// �I�����ɖ߂�ݒ�
	SetBackSelect();
}

//==========================================================================
// �ҏW
//==========================================================================
void CTitle_OptionSelect::StateEdit()
{
	// ���͏��擾
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// �I������
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x < 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{
		// �O��̑I�����֖߂�
		m_select = m_Oldselect;

		// �I�����
		SetState(STATE::STATE_SELECT);
	}
}

//==========================================================================
// �t�F�[�h�C��
//==========================================================================
void CTitle_OptionSelect::StateFadeIn()
{
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// ���Ԍo��
	if (m_fStateTimer >= StateTime::FADE)
	{
		SetState(STATE::STATE_SELECT);
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CTitle_OptionSelect::StateFadeOut()
{
	// �s�����x�X�V
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// ���Ԍo��
	if (m_fStateTimer >= StateTime::FADE)
	{
		SetState(STATE::STATE_SCROLLWAIT_CLOSE);
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitle_OptionSelect::Draw()
{
	
}

//==========================================================================
// �I�����ɖ߂�ݒ�
//==========================================================================
void CTitle_OptionSelect::SetBackSelect()
{
	// �L�[�R���t�B�O�擾
	CKeyConfig* pKeyConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKey = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (m_pScroll == nullptr ||
		(m_pScroll != nullptr && m_pScroll->GetState() != CScroll::STATE::STATE_WAIT)) return;

	// �I������ʂ֖߂�
	if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) ||
		pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		// ����������
		if (m_pScroll != nullptr)
		{
			m_pScroll->SetState(CScroll::STATE::STATE_CLOSE);
		}


		// �^�C�g���擾
		CTitle* pTitle = CTitle::GetInstance();
		pTitle->SetSceneType(CTitle::SCENETYPE::SCENETYPE_NONE);

		// �L�[�R���t�B�O�擾
		pTitle->GetSetting()->Uninit();
		pTitle->SetSetting(nullptr);


		// �t�F�[�h�A�E�g
		SetState(STATE::STATE_FADEOUT);
	}
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitle_OptionSelect::SetState(const STATE& state)
{
	m_state = state;
	m_fStateTimer = 0.0f;
}
