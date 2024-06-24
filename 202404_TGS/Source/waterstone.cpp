//=============================================================================
// 
//  �����Ώ��� [waterstone.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "waterstone.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL[] =
	{
		"data\\MODEL\\map_object\\rock_01_small.x",
		"data\\MODEL\\map_object\\rock_02.x",
		"data\\MODEL\\map_object\\rock_03.x",
		"data\\MODEL\\map_object\\rock_04.x",
		"data\\MODEL\\map_object\\rock_05.x",
	};
	const float DEFAULT_SPLASHTIME = 1.5f;	// �ʏ�̂��Ԃ�����
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CWaterStone::CWaterStone(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fSplashTimer = 0.0f;		// ���Ԃ��^�C�}�[
	m_fIntervalSplash = 0.0f;	// ���Ԃ��܂ł̃C���^�[�o��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CWaterStone::~CWaterStone()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CWaterStone *CWaterStone::Create(const MyLib::Vector3& pos)
{
	// �������̊m��
	CWaterStone* pObj = DEBUG_NEW CWaterStone;

	if (pObj != nullptr)
	{
		pObj->SetPosition(pos);

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CWaterStone::Init()
{
	
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	int idx = (sizeof(MODEL) / sizeof(*MODEL)) - 1;
	idx = UtilFunc::Transformation::Random(0, idx);

	HRESULT hr = CObjectX::Init(MODEL[idx]);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CWaterStone::Uninit()
{
	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CWaterStone::Kill()
{
	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CWaterStone::Update()
{
	// ���Ԃ��^�C�}�[���Z
	m_fSplashTimer += CManager::GetInstance()->GetDeltaTime();

	// �X�v���b�V���I
	if (m_fIntervalSplash <= m_fSplashTimer)
	{


		// �C���^�[�o���X�V
		m_fIntervalSplash = DEFAULT_SPLASHTIME + UtilFunc::Transformation::Random(-30, 30) * 0.01f;

		// ���Ԃ��^�C�}�[
		m_fSplashTimer = 0.0f;
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CWaterStone::Draw()
{
	// �`��
	CObjectX::Draw();
}

