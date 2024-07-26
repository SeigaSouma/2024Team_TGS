//=============================================================================
// 
//  �T���v��_�I�u�W�F�N�g2D���� [sample_obj2D.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "splashwater.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE_SPLASH = "data\\TEXTURE\\effect\\drop_pattern_000.png";	// �e�N�X�`���̃t�@�C��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSplashwater::CSplashwater(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CSplashwater::~CSplashwater()
{

}

//==========================================================================
// ��������
//==========================================================================
CSplashwater* CSplashwater::Create()
{
	// �������̊m��
	CSplashwater* pObj = DEBUG_NEW CSplashwater;

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
HRESULT CSplashwater::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SPLASH);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
#endif
	SetSize(D3DXVECTOR2(100.0f, 100.0f));
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	SetAlpha(0.0f);

	// �ʒu�A�����ݒ�͕K�v������Βǉ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CSplashwater::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CSplashwater::Update()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSplashwater::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}
