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
	const MyLib::Vector3 STR_DEFPOS = MyLib::Vector3(100.0f, 300.0f, 0.0f);	// ��_�̈ʒu
	const float STR_HEIGHT = 100.0f;	// ������̍���
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
	
	MyLib::Vector3 pos = STR_DEFPOS;
	CTexture* pTex = CTexture::GetInstance();

	// ��������
	for (int i = 0; i < NUM_STRING; i++)
	{
		m_apObj2D[i] = CObject2D::Create();
		m_apObj2D[i]->SetType(CObject::TYPE::TYPE_NONE);
		m_apObj2D[i]->SetPosition(pos);

		// �e�N�X�`�����蓖��
		int nIdx = pTex->Regist(TEXPATH[i]);
		m_apObj2D[i]->BindTexture(nIdx);

		// �T�C�Y�ύX
		D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByHeight(pTex->GetImageSize(nIdx), STR_HEIGHT);
		m_apObj2D[i]->SetSize(size);

		// �����ʒu���炷
		pos.x += size.x * 2.0f;

		if (i == 2)
		{// NOW��Loading�̊�
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
