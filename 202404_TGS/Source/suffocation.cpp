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
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSuffocation::CSuffocation(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
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

	MyLib::Vector3 move = GetMove();
	move = UtilFunc::Transformation::Random(-50, 50) * 0.1f;
	SetMove(move);

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
	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f,600.0f,0.0f));


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
	// �X�V����
	CObject2D::Update();

	int nCtr;
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	if (UtilFunc::Transformation::Random(0, 2) == 0)
	{
		move.x *= -1.0f;
	}

	move.y = -1.2f;
	pos += move;

	if (pos.y <= SCREEN_HEIGHT * 0.5f)
	{
		move.y = 0.0f;
		pos.y = SCREEN_HEIGHT * 0.5f;
		SetPosition(pos);
	}
	SetMove(move);
	SetPosition(pos);

}

//==========================================================================
// �`�揈��
//==========================================================================
void CSuffocation::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}
