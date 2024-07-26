//=============================================================================
// 
//  �N���A�����N���� [clearrank.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "clearrank.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE[] =	// �e�N�X�`���̃t�@�C��
	{
		"data\\TEXTURE\\result\\rank_S.png",
		"data\\TEXTURE\\result\\rank_A.png",
		"data\\TEXTURE\\result\\rank_B.png",
		"data\\TEXTURE\\result\\rank_C.png",
		"data\\TEXTURE\\result\\rank_C.png",
	};
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\clearrank.png";
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CClearRank::CClearRank(int nPriority) : CObject2D(nPriority)
{
	// �l�̃N���A
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// �����N
	m_pText = nullptr;		// ����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CClearRank::~CClearRank()
{

}

//==========================================================================
// ��������
//==========================================================================
CClearRank* CClearRank::Create(CJudge::JUDGE rank)
{
	// �������̊m��
	CClearRank* pClearRank = DEBUG_NEW CClearRank;

	if (pClearRank != nullptr)
	{
		pClearRank->m_Rank = rank;

		// ����������
		pClearRank->Init();
	}

	return pClearRank;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CClearRank::Init()
{

	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_Rank]);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 100.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 100.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(1040.0f,360.0f,0.0f));

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);


	//=============================
	// ��������
	//=============================
	CreateText();
	return S_OK;
}

//==========================================================================
// ��������
//==========================================================================
void CClearRank::CreateText()
{
	// ����
	m_pText = CObject2D::Create(GetPriority());
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 100.0f);
	m_pText->SetSize(size);
	m_pText->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pText->SetPosition(GetPosition() + MyLib::Vector3(-size.x, 0.0f, 0.0f));
}

//==========================================================================
// �I������
//==========================================================================
void CClearRank::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CClearRank::Update()
{
	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CClearRank::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}
