//=============================================================================
// 
//  ���g���CUI���� [retryui.cpp]
//  Author : �x�씋��
// 
//=============================================================================
#include "retryui.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "player.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_UI[3] = { "data\\TEXTURE\\dead\\restartbutton.png",
		"data\\TEXTURE\\dead\\retrybutton.png" ,
		"data\\TEXTURE\\dead\\posebutton.png" };	// �e�N�X�`���̃t�@�C��

	const MyLib::Vector3 DEST_POS[3] = { MyLib::Vector3(60.0f, 685.0f, 0.0f),
		MyLib::Vector3(640.0f, 685.0f, 0.0f),
		MyLib::Vector3(1220.0f, 685.0f, 0.0f) };	// �ڕW�̈ʒu

	const MyLib::Vector3 START_POS[3] = { MyLib::Vector3(-115.0f, 685.0f, 0.0f),
		MyLib::Vector3(640.0f, 900.0f, 0.0f),
		MyLib::Vector3(1415.0f, 685.0f, 0.0f) };	// �J�n�̈ʒu

	const float UI_HEIGHT[3] = { 50.0f,100.0f,50.0f };// Ui�̕�
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRetry_Ui::CRetry_Ui(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt] = nullptr;
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRetry_Ui::~CRetry_Ui()
{

}

//==========================================================================
// ��������
//==========================================================================
CRetry_Ui* CRetry_Ui::Create()
{
	// �������̊m��
	CRetry_Ui* pObj = DEBUG_NEW CRetry_Ui;

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
HRESULT CRetry_Ui::Init()
{

	//// �I�u�W�F�N�g2D�̏�����
	//CObject2D::Init();

	int texID;
	D3DXVECTOR2 size;
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt] = CObject2D::Create();

		// �I�u�W�F�N�g2D�̏�����
		m_Button[cnt]->Init();

		// �e�N�X�`���ݒ�
		texID = CTexture::GetInstance()->Regist(TEXTURE_UI[cnt]);
		m_Button[cnt]->BindTexture(texID);

		// �T�C�Y�ݒ�
		size = CTexture::GetInstance()->GetImageSize(texID);

#if 1	// ���������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, UI_HEIGHT[cnt]);

#else	// �c�������ɃT�C�Y�ݒ�
		size = UtilFunc::Transformation::AdjustSizeByWidth(size, 320.0f);
#endif

		m_Button[cnt]->SetSize(size);
		m_Button[cnt]->SetSizeOrigin(size);

		// �ʒu�̐ݒ�
		m_Button[cnt]->SetPosition(START_POS[cnt]);

		// ��ނ̐ݒ�
		m_Button[cnt]->SetType(CObject::TYPE::TYPE_OBJECT2D);
	}

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRetry_Ui::Uninit()
{
	// �I������
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		if (m_Button[cnt] != nullptr)
		{
			m_Button[cnt]->Uninit();
			m_Button[cnt] = nullptr;
		}
	}

	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CRetry_Ui::Update()
{
	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// �t�F�[�h���擾
	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	// ���X�g���[�v
	while (playerList.ListLoop(&pPlayer))
	{
		if (pPlayer->GetState() == CPlayer::STATE_DEAD
			|| pPlayer->GetState() == CPlayer::STATE_RESPAWN
			|| pPlayer->GetState() == CPlayer::STATE_FADEOUT)
		{
			Moveui();
			if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_Y, 0))
			{// Y����͂��Ă���

			}
			if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON::BUTTON_X, 0))
			{// X����͂��Ă���

			}
		}
	}


	// �X�V����
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		m_Button[cnt]->Update();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRetry_Ui::Draw()
{
	// �v���C���[�擾
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// ���X�g���[�v
	while (playerList.ListLoop(&pPlayer))
	{
		if (pPlayer->GetState() == CPlayer::STATE_DEAD
			|| pPlayer->GetState() == CPlayer::STATE_RESPAWN
			|| pPlayer->GetState() == CPlayer::STATE_FADEOUT)
		{
			// �`�揈��
			for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
			{
				m_Button[cnt]->Draw();
			}
		}

	}
}

//==========================================================================
// �ړ�����
//==========================================================================
void CRetry_Ui::Moveui()
{
	MyLib::Vector3 deffpos, changepos;
	for (int cnt = 0; cnt < BUTTON_MAX; cnt++)
	{
		// �ڕW�ʒu�܂ł̍����v�Z
		deffpos.x = (DEST_POS[cnt].x - m_Button[cnt]->GetPosition().x) * 0.01f;
		deffpos.y = (DEST_POS[cnt].y - m_Button[cnt]->GetPosition().y) * 0.01f;

		// ���X�ɖڕW�ʒu�Ɉړ�
		MyLib::Vector3 changepos =
			MyLib::Vector3(m_Button[cnt]->GetPosition().x +
				deffpos.x, m_Button[cnt]->GetPosition().y +
				deffpos.y, m_Button[cnt]->GetPosition().z);

		// �ʒu��ݒ�
		m_Button[cnt]->SetPosition(changepos);
	}


}