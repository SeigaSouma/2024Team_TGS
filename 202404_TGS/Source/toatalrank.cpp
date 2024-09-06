//=============================================================================
// 
//  �g�[�^�������N���� [toatalrank.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "toatalrank.h"
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
	};
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\toatalrank.png";

	const int RANKPOINT[] =	// �����N�̊
	{
		4,
		3,
		2,
		1
	};

	const float TIMEPOINT[] =	// �^�C�}�[�̊
	{
		80.0f,
		100.0f,
		120.0f,
		140.0f
	};

	const float SIZE_HEIGHT = 50.0f;	// �c���̃T�C�Y
	const float MOVEVALUE_TEXT = 3.0f;	//�e�L�X�g�̈ړ���
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CToatalRank::CToatalRank(int nPriority) : CObject2D(nPriority)
{
	// �l�̃g�[�^��
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// �����N
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CToatalRank::~CToatalRank()
{

}

//==========================================================================
// ��������
//==========================================================================
CToatalRank* CToatalRank::Create(CJudge::JUDGE rank, float time)
{
	// �������̊m��
	CToatalRank* pClearRank = DEBUG_NEW CToatalRank;

	if (pClearRank != nullptr)
	{
		pClearRank->m_Rank = pClearRank->CalRank(rank, time);

		// ����������
		pClearRank->Init();
	}

	return pClearRank;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CToatalRank::Init()
{
	//=============================
	// ��������
	//=============================
	CreateText();



	// �I�u�W�F�N�g2D�̏�����
	CObject2D::Init();

	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_Rank]);
	BindTexture(texID);

	// �T�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// ���������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// �c�������ɃT�C�Y�ݒ�
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 120.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);
	SetPosition(m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSize().x * 2.0f + 50.0f,100.0f,0.0f));

	// �ʒu�A�����ݒ�͕K�v������Βǉ�

	// ��ނ̐ݒ�
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// ��������
//==========================================================================
void CToatalRank::CreateText()
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
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(size);
	m_pText->SetSizeOrigin(size);

	// �ʒu�ݒ�
	m_pText->SetPosition(MyLib::Vector3(700.0f, 400.0f, 0.0f));

	// �A���J�[�|�C���g�̐ݒ�
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// �I������
//==========================================================================
void CToatalRank::Uninit()
{
	// �I������
	CObject2D::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CToatalRank::Update()
{
	// �X�V����
	CObject2D::Update();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CToatalRank::Draw()
{
	// �`�揈��
	CObject2D::Draw();
}

//==========================================================================
// �����N�v�Z
//==========================================================================
CJudge::JUDGE CToatalRank::CalRank(CJudge::JUDGE rank, float time)
{
	int point = 0, i = 0;

	// ���Ԃ̃|�C���g�v�Z
	while (1)
	{
		if (TIMEPOINT[i] >= time)
		{
			point += CJudge::JUDGE::JUDGE_DDD - (i);
			break;
		}
	}
	
	// �����N�̃|�C���g�v�Z
	point += CJudge::JUDGE::JUDGE_DDD - rank;

	if (point >= CJudge::JUDGE::JUDGE_DDD * 2)
	{
		return CJudge::JUDGE::JUDGE_AAA;
	}
	else if (point >= CJudge::JUDGE::JUDGE_CCC * 2)
	{
		return CJudge::JUDGE::JUDGE_BBB;
	}
	else if (point >= CJudge::JUDGE::JUDGE_BBB * 2)
	{
		return CJudge::JUDGE::JUDGE_CCC;
	}
	else
	{
		return CJudge::JUDGE::JUDGE_DDD;
	}
}