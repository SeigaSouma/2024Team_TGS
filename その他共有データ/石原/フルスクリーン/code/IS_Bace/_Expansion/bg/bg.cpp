//======================================================
//
//�w�i�i2D�j����[bg.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "bg.h"
#include "..\..\..\main.h"
#include "..\..\_Core\utility\Utility.h"

//=================================
//�R���X�g���N�^
//=================================
CBG::CBG(int nPriority) : CObject2D(nPriority)
{
	
}

//=================================
//�f�X�g���N�^
//=================================
CBG::~CBG()
{
}

//=================================
//������
//=================================
HRESULT CBG::Init(void)
{
	//�e�N���X����
	if (SUCCEEDED(CObject2D::Init()))
	{
		SetPos(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));
		SetRot(IS_Utility::VEC3_ZERO);
		SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		return S_OK;
	}
	else
	{
		return E_FAIL;
	}
}

//=================================
//�I��
//=================================
void CBG::Uninit(void)
{
	//�e�N���X����
	CObject2D::Uninit();
}

//=================================
//�X�V
//=================================
void CBG::Update(void)
{
	//�e�N���X����
	CObject2D::Update();
}

//=================================
//�`��
//=================================
void CBG::Draw(void)
{
	//�e�N���X����
	CObject2D::Draw();
}

//=================================
//��������
//=================================
CBG* CBG::Create(const int nPriority)
{
	CBG* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�w�i�̐���
		pObj = new CBG(nPriority);

		//������
		pObj->Init();

		return pObj;
	}
	else
	{
		return nullptr;
	}
}