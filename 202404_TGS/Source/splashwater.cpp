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
	const std::string TEXTURE_SPLASH[] =
	{
		"data\\TEXTURE\\effect\\drop_pattern_000.png",
		"data\\TEXTURE\\effect\\drop_pattern_005.png",
		"data\\TEXTURE\\effect\\drop_pattern_006.png",
		"data\\TEXTURE\\effect\\drop_pattern_007.png",
		"data\\TEXTURE\\effect\\drop_pattern_008.png",
		"data\\TEXTURE\\effect\\drop_pattern_009.png",
		"data\\TEXTURE\\effect\\drop_pattern_010.png",
	};	// �e�N�X�`���̃t�@�C��
	const float TIME_FADEOUT_RATIO = 0.3f;
	const float DEFAULT_LIFE = 1.0f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CSplashwater::CSplashwater(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_fLife = 0.0f;			// ����
	m_fOriginLife = 0.0f;	// ����
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
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SPLASH[0/*UtilFunc::Transformation::Random(0, 6)*/]);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 25.0f + UtilFunc::Transformation::Random(-200, 100) * 0.1f);
#endif
	SetSize(size);
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(
		UtilFunc::Transformation::Random(-64, 64) * 10.0f + 640.0f,
		UtilFunc::Transformation::Random(-40, 100),
		0.0f));

	// �ʒu�A�����ݒ�͕K�v������Βǉ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);


	m_fLife = DEFAULT_LIFE + UtilFunc::Transformation::Random(-2, 10) * 0.1f;
	m_fOriginLife = m_fLife;
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
	CObject2D::Update();

	m_fLife -= CManager::GetInstance()->GetDeltaTime();

	float fall = 0.0f;
	if (UtilFunc::Transformation::Random(0, 2) == 0)
	{
		fall = UtilFunc::Transformation::Random(0, 2000) * 0.01f;
	}

	AddPosition(MyLib::Vector3(
		UtilFunc::Transformation::Random(-100, 100) * 0.1f,
		fall,
		0.0f));

	float lifeRatio = m_fLife / m_fOriginLife;
	if (lifeRatio <= TIME_FADEOUT_RATIO)
	{
		SetAlpha(lifeRatio / TIME_FADEOUT_RATIO);
	}

	if (m_fLife <= 0.0f)
	{
		Uninit();
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CSplashwater::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}
