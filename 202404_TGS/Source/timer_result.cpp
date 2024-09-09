//=============================================================================
// 
//  �^�C�}�[����(�^��) [timer_result.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "timer_result.h"
#include "manager.h"
#include "multinumber.h"
#include "debugproc.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano.png";	// �e�N�X�`���̃t�@�C��
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\cleartime.png";

	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(1100.0f, 100.0f, 0.0f);	// �����ʒu
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(30.0f, 30.0f);
	const float DSTANCE_TIMER = SIZE_NUMBER.x * 2.25f;

	const float SIZE_HEIGHT = 50.0f;	// �c���̃T�C�Y
	const float MOVEVALUE_TEXT = 3.0f;	//�e�L�X�g�̈ړ���
}

namespace StateTime
{
	const float WAIT = 0.5f;	// �ҋ@
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTimer_Result::STATE_FUNC CTimer_Result::m_StateFunc[] =
{
	&CTimer_Result::StateScrollText,	// ��������
	&CTimer_Result::StateSrollVoid,		// ��ԑ���
	&CTimer_Result::StateScrollTime,	// �^�C������
	&CTimer_Result::StateFinish,		// �I��
	&CTimer_Result::StateNone,			// �Ȃɂ��Ȃ�

};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTimer_Result::CTimer_Result(int nPriority) : CTimer(nPriority)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;		// ��ԃJ�E���^�[
	m_state = State::STATE_SCROLL_TEXT;			// ���
	m_fMoveTextLen = 0.0f;	// �e�L�X�g�̈ړ�����
	m_fMoveTimeLen = 0.0f;	// �^�C���̈ړ�����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTimer_Result::~CTimer_Result()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTimer_Result::Init()
{
	//=============================
	// ��������
	//=============================
	CreateText();

	// ������
	CTimer::Init();
	m_pos = m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSizeOrigin().x * 2.0f + 150.0f, 0.0f, 0.0f);

	// �A���J�[�|�C���g�����ɂ���
	for (int i = 0; i < 3; i++)
	{
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			CObject2D* pObj2D = pNumber[j]->GetObject2D();

			pObj2D->SetAnchorType(CObject2D::AnchorPoint::LEFT);
		}

	}

	// �^�C�}�[
	ApplyTimer();

	return S_OK;
}


//==========================================================================
// ��������
//==========================================================================
void CTimer_Result::CreateText()
{
	// ����
	m_pText = CObject2D::Create(6);
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(size);
	m_pText->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pText->SetPosition(MyLib::Vector3(200.0f, 350.0f, 0.0f));
	
	// �A���J�[�|�C���g�̐ݒ�
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// �I������
//==========================================================================
void CTimer_Result::Uninit()
{
	CTimer::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTimer_Result::Update()
{
	// �X�V����
	CTimer::Update();

	
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTimer_Result::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// ��������
//==========================================================================
void CTimer_Result::StateScrollText()
{
	// �T�C�Y�擾
	D3DXVECTOR2 size = m_pText->GetSize(), sizeOrigin = m_pText->GetSizeOrigin();

	// �e�L�X�g�ړ��������Z
	m_fMoveTextLen += MOVEVALUE_TEXT;
	m_fMoveTextLen = UtilFunc::Transformation::Clamp(m_fMoveTextLen, 0.0f, sizeOrigin.x);

	if (m_fMoveTextLen >= sizeOrigin.x)
	{
		// ��ԑJ��
		SetState(State::STATE_SCROLL_VOID);
	}

	// �T�C�Y�ݒ�
	size.x = m_fMoveTextLen;
	m_pText->SetSize(size);

	// �e�N�X�`�����W�ݒ�
	D3DXVECTOR2* pTex = m_pText->GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);
}

//==========================================================================
// ��ԑ���
//==========================================================================
void CTimer_Result::StateSrollVoid()
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		// ��ԑJ��
		SetState(State::STATE_SCROLL_TIME);
	}
}

//==========================================================================
// �^�C������
//==========================================================================
void CTimer_Result::StateScrollTime()
{

	// �A���J�[�|�C���g�����ɂ���
	D3DXVECTOR2 size, sizeOrigin;

	for (int i = 0; i < 3; i++)
	{
		// �i���o�[�擾
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			// ������2D�I�u�W�F�N�g�擾
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			size = pObj2D->GetSize();


		}

	}


	//// �T�C�Y�擾
	//D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();

	//// �e�L�X�g�ړ��������Z
	//m_fMoveTimeLen += MOVEVALUE_TEXT;
	//m_fMoveTimeLen = UtilFunc::Transformation::Clamp(m_fMoveTimeLen, 0.0f, sizeOrigin.x);

	//if (m_fMoveTimeLen >= sizeOrigin.x)
	//{
	//	// ��ԑJ��
	//	SetState(State::STATE_FINISH);
	//}

	//// �T�C�Y�ݒ�
	//size.x = m_fMoveTimeLen;
	//SetSize(size);

	//// �e�N�X�`�����W�ݒ�
	//D3DXVECTOR2* pTex = GetTex();
	//pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);
}

//==========================================================================
// �I��
//==========================================================================
void CTimer_Result::StateFinish()
{
	//// �T�C�Y�ݒ�
	//SetSize(GetSizeOrigin());
	//m_pText->SetSize(m_pText->GetSizeOrigin());

	//// �e�N�X�`�����W�ݒ�
	//D3DXVECTOR2* pTex = GetTex();
	//D3DXVECTOR2* pTexText = m_pText->GetTex();
	//pTex[1].x = pTex[3].x = pTexText[1].x = pTexText[3].x = 1.0f;

	//// ��ԑJ��
	//SetState(State::STATE_NONE);
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTimer_Result::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTimer_Result::Draw()
{

}
