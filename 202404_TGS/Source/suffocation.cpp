//=============================================================================
// 
//  �T�t�H�P�[�V�������� [suffocation.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "suffocation.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\subtitle\\suffocation.png";	// �e�N�X�`���̃t�@�C��
	static int RANDOM_MOVEX = 250;
	static int RANDOM_MOVEINTERVAL = 2;
	static float VELOCITY_UP = 0.13f;
}

namespace StateTime	// ��ԕʎ���
{
	const float BURST = 0.3f;	// �j��
}


//==========================================================================
// �֐��|�C���^
//==========================================================================
CSuffocation::STATE_FUNC CSuffocation::m_StateFunc[] =
{
	&CSuffocation::StateSurfacing,	// �Ȃ�
	&CSuffocation::StateBurst,		// �I�[�v��
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSuffocation::CSuffocation(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;		// ��ԃJ�E���^�[
	m_state = State::STATE_SURFACING;			// ���
	m_fDestWidth = 0.0f;	// �ڕW�̕�
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSuffocation::~CSuffocation()
{

}

//==========================================================================
// ��������
//==========================================================================
CSuffocation* CSuffocation::Create()
{
	// �������̊m��
	CSuffocation* pObj = DEBUG_NEW CSuffocation;

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
HRESULT CSuffocation::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, 720.0f + size.y, 0.0f));


	// �ʒu�A�����ݒ�͕K�v������Βǉ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSuffocation::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CSuffocation::Update()
{
	// ��ԍX�V
	UpdateState();
	if (IsDeath()) return;

	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CSuffocation::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԃ��X�g
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ����
//==========================================================================
void CSuffocation::StateSurfacing()
{
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	// ���h��̃����_��
	if (UtilFunc::Transformation::Random(0, RANDOM_MOVEX) == 0)
	{
		m_fDestWidth = UtilFunc::Transformation::Random(-RANDOM_MOVEX, RANDOM_MOVEX) * 0.1f;
	}

	// ����
	move.y -= (VELOCITY_UP + UtilFunc::Transformation::Random(0, 30) * 0.01f);
	pos.y += move.y;

	// ���h��
	move.x += (m_fDestWidth - move.x) * 0.1f;
	pos.x = 640.0f + move.x;

	// ����
	if (pos.y <= SCREEN_HEIGHT * 0.5f)
	{
		move.y = 0.0f;
		pos.y = SCREEN_HEIGHT * 0.5f;
		SetPosition(pos);

		// ��ԑJ��
		m_state = State::STATE_BURST;
		m_fStateTime = 0.0f;
	}
	SetMove(move);
	SetPosition(pos);
}

//==========================================================================
// �j��
//==========================================================================
void CSuffocation::StateBurst()
{
	// �ʒu�X�V
	MyLib::Vector3 pos = GetPosition();
	pos.x += (640.0f - pos.x) * 0.2f;
	SetPosition(pos);

	// �T�C�Y�X�V
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();
	size.x = UtilFunc::Correction::EasingEaseInOut(sizeOrigin.x, sizeOrigin.x * 5.0f, 0.0f, StateTime::BURST, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseInOut(sizeOrigin.y, sizeOrigin.y * 5.0f, 0.0f, StateTime::BURST, m_fStateTime);
	SetSize(size);

	// �s�����x
	float alpha = UtilFunc::Correction::EasingEaseInOut(1.0f, 0.0f, 0.0f, StateTime::BURST, m_fStateTime);
	SetAlpha(alpha);

	if (m_fStateTime >= StateTime::BURST)
	{
		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSuffocation::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}
