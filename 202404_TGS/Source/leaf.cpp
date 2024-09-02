//=============================================================================
// 
//  �t���Ϗ��� [leaf.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "leaf.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// �e�N�X�`���̃t�@�C��
}

namespace StateTime	// ��ԕʎ���
{
	const float FADEOUT = 0.3f;	// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CLeaf::STATE_FUNC CLeaf::m_StateFunc[] =
{
	&CLeaf::StateFall,		// ����
	&CLeaf::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLeaf::CLeaf(int nPriority, const LAYER layer) : CObject3D(nPriority, layer)
{
	// �l�̃N���A
	m_fStateTimer = 0.0f;			// ��ԃJ�E���^�[
	m_state = State::STATE_FALL;	// ���
	m_rotDest = MyLib::Vector3();	// �ڕW�̌���
	m_fRotateTimer = 0.0f;			// ��]�܂ł̎���
	m_fRotateInterval = 0.0f;		// ��]�܂ł̊Ԋu
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLeaf::~CLeaf()
{

}

//==========================================================================
// ��������
//==========================================================================
CLeaf* CLeaf::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CLeaf* pObj = DEBUG_NEW CLeaf;

	if (pObj == nullptr)
	{// ���s
		return nullptr;
	}

	// �������ݒ�
	pObj->SetPosition(pos);
	pObj->SetOriginPosition(pos);

	// ����������
	pObj->Init();

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLeaf::Init()
{

	// �I�u�W�F�N�g3D�̏�����
	CObject3D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// �c�������ɃT�C�Y�ݒ�
	float randSize = UtilFunc::Transformation::Random(40, 60);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, randSize);

	MyLib::Vector3 setsize = MyLib::Vector3(size.x, size.y, 0.0f);
	SetSize(setsize);
	SetSizeOrigin(setsize);

	// ����
	MyLib::Vector3 rot;
	rot.y = UtilFunc::Transformation::Random(-31, 31) * 0.01f;
	rot.x = UtilFunc::Transformation::Random(-31, 31) * 0.01f;
	SetRotation(rot);
	SetOriginRotation(rot);

	// �ڕW�̌���
	CalDestRotation();

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// ���
	m_fRotateInterval = UtilFunc::Transformation::Random(100, 150) * 0.01f;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CLeaf::Uninit()
{
	// �I������
	CObject3D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CLeaf::Update()
{
	// ��ԍX�V
	UpdateState();

	// �X�V����
	CObject3D::Update();
}

//==========================================================================
// �ڕW�̌����v�Z
//==========================================================================
void CLeaf::CalDestRotation()
{
	// �����擾
	MyLib::Vector3 rot = GetRotation();

	// �ڕW�̌���
	m_rotDest.y = rot.y + UtilFunc::Transformation::Random(10, 31) * 0.01f;
	m_rotDest.x = rot.x + UtilFunc::Transformation::Random(10, 31) * 0.01f;
	UtilFunc::Transformation::RotNormalize(m_rotDest);
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CLeaf::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ����
//==========================================================================
void CLeaf::StateFall()
{
	MyLib::Vector3 pos = GetPosition(), posOrigin = GetOriginPosition();
	MyLib::Vector3 rot = GetRotation(), rotOrigin = GetOriginRotation();

	// ��]�܂ł̎��ԉ��Z
	m_fRotateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��]���`���
	rot = UtilFunc::Correction::EasingEaseOut(rotOrigin, m_rotDest, 0.0f, m_fRotateInterval, m_fRotateTimer);
	UtilFunc::Transformation::RotNormalize(rot);
	SetRotation(rot);

	// ����
	pos = UtilFunc::Correction::EasingEaseOut(posOrigin, posOrigin + MyLib::Vector3(0.0f, -50.0f, 0.0f), 0.0f, m_fRotateInterval, m_fRotateTimer);

	// ���Ԍo�߁A�V�����ڕW�����Z�o
	if (m_fRotateTimer >= m_fRotateInterval)
	{
		// ��]�^�C�}�[���Z�b�g
		m_fRotateTimer = 0.0f;
		m_fRotateInterval = UtilFunc::Transformation::Random(100, 150) * 0.01f;

		// �ڕW�̌���
		CalDestRotation();
		SetOriginRotation(rot);

		// ���̈ʒu
		SetOriginPosition(pos);
	}

	// ���ݒ�
	SetPosition(pos);

}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CLeaf::StateFadeOut()
{
	if (m_fStateTimer >= StateTime::FADEOUT)
	{// ���Ԍo��


	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CLeaf::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �w�ʂ̃J�����O�Ȃ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �`�揈��
	CObject3D::Draw();

	// �J�����O�f�t�H���g
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
