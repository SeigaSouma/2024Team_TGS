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

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTimer_Result::CTimer_Result(int nPriority) : CTimer()
{
	// �l�̃N���A
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
	m_pos = m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSizeOrigin().x * 2.0f + 200.0f, 0.0f, 0.0f);

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
// �`�揈��
//==========================================================================
void CTimer_Result::Draw()
{

}
