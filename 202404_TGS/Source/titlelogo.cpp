//=============================================================================
// 
// �^�C�g�����S���� [titlelogo.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "titlelogo.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "fade.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\title\\titlelogo4.png";
}

namespace TexturePath	// �e�N�X�`���p�X
{
	const std::string WATER = "data\\TEXTURE\\title\\water.png";
	const std::string PLAYER = "data\\TEXTURE\\title\\player.png";
	const std::string BRESS = "data\\TEXTURE\\title\\bress.png";
	const std::string LOVE[] =
	{
		"data\\TEXTURE\\title\\L.png",
		"data\\TEXTURE\\title\\O.png",
		"data\\TEXTURE\\title\\V.png",
		"data\\TEXTURE\\title\\E.png",
	};
	const std::string SUITON = "data\\TEXTURE\\title\\suiton.png";
}

namespace Size	// �T�C�Y
{
	const float WATER = 200.0f;
	const float PLAYER = 80.0f;
	const float BRESS = 80.0f;
	const float LOVE = 60.0f;
	const float SUITON = 80.0f;
}

namespace BasePoint	// ��_�̈ʒu
{
	const MyLib::Vector3 WATER = MyLib::Vector3(640.0f, 360.0f, 0.0f);
	const MyLib::Vector3 PLAYER = MyLib::Vector3(740.0f, 460.0f, 0.0f);
	const MyLib::Vector3 BRESS = MyLib::Vector3(550.0f, 350.0f, 0.0f);
	const MyLib::Vector3 LOVE = MyLib::Vector3(200.0f, 150.0f, 0.0f);
	const MyLib::Vector3 SUITON = MyLib::Vector3(800.0f, 150.0f, 0.0f);
}

namespace DestPoint	// �ڕW�̈ʒu
{
	const MyLib::Vector3 PLAYER = MyLib::Vector3(640.0f, 400.0f, 0.0f);
	const MyLib::Vector3 BRESS = MyLib::Vector3(530.0f, 320.0f, 0.0f);
	const MyLib::Vector3 LOVE[] =	// LOVE�̏ꍇ�͍���
	{
		MyLib::Vector3(-10.0f, -30.0f, 0.0f),
		MyLib::Vector3(-5.0f, -70.0f, 0.0f),
		MyLib::Vector3(0.0f, -50.0f, 0.0f),
		MyLib::Vector3(5.0f, -50.0f, 0.0f),
	};
}

namespace StateTime	// ��Ԏ���
{
	const float WATER = 1.0f;
	const float PLAYER_AND_NAME = 1.0f;
	const float BRESS = 1.0f;
	const float AFTERMOVEMENT = 1.0f;
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CTitleLogo::STATE_FUNC CTitleLogo::m_StateFunc[] =
{
	&CTitleLogo::StateNone,					// �Ȃ�
	&CTitleLogo::StateFadeIn_Water,			// ���t�F�[�h�C��
	&CTitleLogo::StateFadeIn_PlayerAndName,	// �v���C���[�Ɩ��O�t�F�[�h�C��
	&CTitleLogo::StateBress,				// ��
	&CTitleLogo::StateAfterMovement,		// ��̓���
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTitleLogo::CTitleLogo(float fadetime, int nPriority) : m_fFadeOutTime(fadetime), CObject(nPriority)
{
	// �l�̃N���A
	m_state = STATE_NONE;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTitleLogo::~CTitleLogo()
{

}

//==========================================================================
// ��������
//==========================================================================
CTitleLogo* CTitleLogo::Create(float fadetime)
{
	// �������̊m��
	CTitleLogo* pScreen = DEBUG_NEW CTitleLogo(fadetime);

	if (pScreen != nullptr)
	{
		// ����������
		pScreen->Init();
	}

	return pScreen;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTitleLogo::Init()
{
	// ��ނ̐ݒ�
	SetType(TYPE_OBJECT2D);

	// ������
	CreateWater();

	// �v���C���[����
	CreatePlayer();

	// ������
	CreateBress();

	// LOVE����
	CreateLOVE();

	// SUITON����
	CreateSUITON();

	m_state = State::STATE_FADEIN_WATER;	// ���
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[

	return S_OK;
}

//==========================================================================
// ������
//==========================================================================
void CTitleLogo::CreateWater()
{
	// ����
	m_pWater = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pWater;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	pObj2D->SetPosition(BasePoint::WATER);
	pObj2D->SetOriginPosition(BasePoint::WATER);


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TexturePath::WATER);
	pObj2D->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::WATER);

	// �T�C�Y�ݒ�
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);
}

//==========================================================================
// �v���C���[����
//==========================================================================
void CTitleLogo::CreatePlayer()
{
	// ����
	m_pPlayer = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pPlayer;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	pObj2D->SetPosition(BasePoint::PLAYER);
	pObj2D->SetOriginPosition(BasePoint::PLAYER);


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TexturePath::PLAYER);
	pObj2D->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::PLAYER);

	// �T�C�Y�ݒ�
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);
}

//==========================================================================
// ������
//==========================================================================
void CTitleLogo::CreateBress()
{
	// ����
	m_pBress = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pBress;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	pObj2D->SetPosition(BasePoint::BRESS);
	pObj2D->SetOriginPosition(BasePoint::BRESS);


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TexturePath::BRESS);
	pObj2D->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::BRESS);

	// �T�C�Y�ݒ�
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);
}

//==========================================================================
// LOVE����
//==========================================================================
void CTitleLogo::CreateLOVE()
{
	for (int i = 0; i < LOGO_LOVE::MAX; i++)
	{
		// ����
		m_pLOVE[i] = CObject2D::Create(GetPriority());
		CObject2D* pObj2D = m_pLOVE[i];
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// �e�N�X�`���ݒ�
		int texID = CTexture::GetInstance()->Regist(TexturePath::LOVE[i]);
		pObj2D->BindTexture(texID);

		// �c�������ɃT�C�Y�ݒ�
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::LOVE);

		// �T�C�Y�ݒ�
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);

		// �ʒu�ݒ�
		pObj2D->SetPosition(BasePoint::LOVE + (MyLib::Vector3(size.x * 1.5f, 0.0f, 0.0f) * i));
		pObj2D->SetOriginPosition(pObj2D->GetPosition());
	}
}

//==========================================================================
// SUITON����
//==========================================================================
void CTitleLogo::CreateSUITON()
{
	// ����
	m_pSUITON = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pSUITON;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// �ʒu�ݒ�
	pObj2D->SetPosition(BasePoint::SUITON);
	pObj2D->SetOriginPosition(BasePoint::SUITON);


	// �e�N�X�`���ݒ�
	int texID = CTexture::GetInstance()->Regist(TexturePath::SUITON);
	pObj2D->BindTexture(texID);

	// �c�������ɃT�C�Y�ݒ�
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::SUITON);

	// �T�C�Y�ݒ�
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);
}

//==========================================================================
// �I������
//==========================================================================
void CTitleLogo::Uninit()
{
	// �I������
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CTitleLogo::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// �t�F�[�h���͔�����
		return;
	}

	// ��ԕʍX�V
	UpdateState();
	if (IsDeath())
	{
		return;
	}

	
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CTitleLogo::UpdateState()
{
	// ��ԑJ�ڃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// �Ȃɂ��Ȃ�
//==========================================================================
void CTitleLogo::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// ���t�F�[�h�C��
//==========================================================================
void CTitleLogo::StateFadeIn_Water()
{
	// �s�����x�ݒ�
	float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::WATER, 0.0f, 1.0f);
	m_pWater->SetAlpha(alpha);

	if (m_fStateTime >= StateTime::WATER)
	{// ���Ԍo��
		SetState(State::STATE_FADEIN_PLAYER_and_NAME);
	}
}

//==========================================================================
// �v���C���[�Ɩ��O�t�F�[�h�C��
//==========================================================================
void CTitleLogo::StateFadeIn_PlayerAndName()
{
	//=============================
	// �v���C���[
	//=============================
	{
		// �s�����x�ݒ�
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::PLAYER_AND_NAME, 0.0f, 1.0f);
		m_pPlayer->SetAlpha(alpha);

		// �v���C���[�ړ�
		MyLib::Vector3 playerpos = UtilFunc::Correction::EasingLinear(BasePoint::PLAYER, DestPoint::PLAYER, 0.0f, StateTime::PLAYER_AND_NAME, m_fStateTime);
		m_pPlayer->SetPosition(playerpos);
	}

	//=============================
	// ���O
	//=============================
	{
		// �s�����x�ݒ�
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::PLAYER_AND_NAME, 0.0f, 1.0f);
		for (const auto& love : m_pLOVE)
		{
			love->SetAlpha(alpha);
		}
		m_pSUITON->SetAlpha(alpha);
	}

	if (m_fStateTime >= StateTime::PLAYER_AND_NAME)
	{// ���Ԍo��
		SetState(State::STATE_BRESS);
	}
}

//==========================================================================
// ��
//==========================================================================
void CTitleLogo::StateBress()
{
	//=============================
	// ��
	//=============================
	{
		// �s�����x�ݒ�
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::BRESS, 0.0f, 1.0f);
		m_pBress->SetAlpha(alpha);

		// �ړ�
		MyLib::Vector3 pos = UtilFunc::Correction::EasingLinear(BasePoint::BRESS, DestPoint::BRESS, 0.0f, StateTime::BRESS, m_fStateTime);
		m_pBress->SetPosition(pos);
	}

	//=============================
	// LOVE
	//=============================
	{
		// �ړ�
		for (int i = 0; i < LOGO_LOVE::MAX; i++)
		{
			// �ړ�
			MyLib::Vector3 pos = UtilFunc::Correction::EasingLinear(
				m_pLOVE[i]->GetOriginPosition(),
				m_pLOVE[i]->GetOriginPosition() + DestPoint::LOVE[i],
				0.0f, StateTime::BRESS, m_fStateTime);
			m_pLOVE[i]->SetPosition(pos);
		}
	}


	if (m_fStateTime >= StateTime::BRESS)
	{// ���Ԍo��
		SetState(State::STATE_AFTERMOVEMENT);
	}
}

//==========================================================================
// ��̓���
//==========================================================================
void CTitleLogo::StateAfterMovement()
{
	//if (m_fStateTime >= StateTime::AFTERMOVEMENT)
	//{// ���Ԍo��
	//	SetState(State::STATE_FADEIN_PLAYER_and_NAME);
	//}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTitleLogo::Draw()
{
	
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CTitleLogo::SetState(State state)
{ 
	m_state = state; 
	m_fStateTime = 0.0f;
}
