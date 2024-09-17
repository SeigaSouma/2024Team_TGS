//=============================================================================
// 
//  �t�F�[�h���� [loadscreen.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "loadscreen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\load\\loadscreen.jpg";
	const MyLib::Vector3 STR_DEFPOS = MyLib::Vector3(100.0f, 300.0f, 0.0f);
	const float MOVE_X = 100.0f;
	const int NUM_STRING = 10;	// NOWLOADING�̕�����
	const std::string TEXPATH[NUM_STRING] =
	{
		"data\\TEXTURE\\load\\n.png",
		"data\\TEXTURE\\load\\o.png",
		"data\\TEXTURE\\load\\w.png",
		"data\\TEXTURE\\load\\l.png",
		"data\\TEXTURE\\load\\o.png",
		"data\\TEXTURE\\load\\a.png",
		"data\\TEXTURE\\load\\d.png",
		"data\\TEXTURE\\load\\i.png",
		"data\\TEXTURE\\load\\mini_n.png",
		"data\\TEXTURE\\load\\g.png",
	};

	const float STR_HEIGHT = 100.0f;
}


//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLoadScreen::CLoadScreen()
{
	// �l�̃N���A
	m_aObject2D = nullptr;					// �I�u�W�F�N�g2D�̃I�u�W�F�N�g

	for (int i = 0; i < NUM_STRING; i++)
	{
		m_apObj2D[i] = nullptr;
	}
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLoadScreen::~CLoadScreen()
{

}

//==========================================================================
// ��������
//==========================================================================
CLoadScreen *CLoadScreen::Create()
{
	// �����p�̃I�u�W�F�N�g
	CLoadScreen *pFade = nullptr;

	if (pFade == nullptr)
	{// nullptr��������

		// �������̊m��
		pFade = DEBUG_NEW CLoadScreen;

		if (pFade != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			if (FAILED(pFade->Init()))
			{// ���s���Ă�����
				return nullptr;
			}
		}

		return pFade;
	}

	return nullptr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLoadScreen::Init()
{
	// ��������
	m_aObject2D = CObject2D_Anim::Create(0.0f, 5, 9, 2, false);
	if (m_aObject2D == nullptr)
	{// ���s���Ă�����
		return E_FAIL;
	}

	MyLib::Vector3 pos = STR_DEFPOS;

	// ��������
	for (int i = 0; i < NUM_STRING; i++)
	{
		CTexture* pTex = CTexture::GetInstance();
		int nIdx = pTex->Regist(TEXPATH[i]);
		D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByHeight(pTex->GetImageSize(nIdx), STR_HEIGHT);
		pos.x += size.x;
		m_apObj2D[i] = CObject2D::Create();
		m_apObj2D[i]->SetType(CObject::TYPE::TYPE_NONE);
		m_apObj2D[i]->SetPosition(pos);
		m_apObj2D[i]->BindTexture(nIdx);
		m_apObj2D[i]->SetSize(size);
		pos.x += size.x;

		if (i == 2)
		{
			pos.x += size.x * 2;
		}
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CLoadScreen::Uninit()
{

	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] == nullptr)
		{
			m_apObj2D[i]->Uninit();
			m_apObj2D[i] = nullptr;
		}
	}
}

void CLoadScreen::Kill()
{
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Uninit();
			delete m_apObj2D[i];
			m_apObj2D[i] = nullptr;
		}
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CLoadScreen::Update()
{
	// ��������
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Update();
		}
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CLoadScreen::Draw()
{
	// ��������
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Draw();
		}
	}
}

//==========================================================================
// �I�u�W�F�N�g2D�I�u�W�F�N�g�̎擾
//==========================================================================
CObject2D_Anim*CLoadScreen::GetMyObject()
{
	return m_aObject2D;
}