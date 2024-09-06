//=============================================================================
// 
// �����L���O2D���� [rankingItem.cpp]
//  Author : ���������Y
// 
//=============================================================================
#include "rankingItem.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "gamemanager.h"
#include "game.h"
#include "game_pressenter.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	//���S���
	//const std::string TEXTURE_RESULT = "data\\TEXTURE\\result\\ninja_resultlogo.png";
	//const MyLib::Vector3 LOGO_POS = MyLib::Vector3(220.0f, 60.0f, 0.0f);
	//const D3DXVECTOR2 LOGO_SIZE = D3DXVECTOR2(200.0f, 50.0f);
	
	const MyLib::Vector3 ITEM_POS = MyLib::Vector3(250.0f,250.0f, 0.0f);
	const D3DXVECTOR2 ITEM_SIZE = D3DXVECTOR2(100.0f, 50.0f);
	const std::string TEXTURE_ITEM[CRankingItem::ITEM_MAX] = { "data\\TEXTURE\\result\\ranking_num00.png",
										"data\\TEXTURE\\result\\toatalrank.png" };


	const float TIME_SET = 0.2f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRankingItem::CRankingItem(int nPriority) : CObject(nPriority)
{
	m_fStateTime = 0.0f;
	
	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		m_pItem[nCnt] = nullptr;
	}
	
}
//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRankingItem::~CRankingItem()
{

}

//==========================================================================
// ��������
//==========================================================================
CRankingItem* CRankingItem::Create()
{
	// �������̊m��
	CRankingItem *pMarker =  new CRankingItem;

	if (pMarker != nullptr)
	{
		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRankingItem::Init()
{
	//���ڂ̏���������
	MyLib::Vector3 difpos = MyLib::Vector3(-600.0f,15.0f,0.0f);
	D3DXVECTOR2 persize = D3DXVECTOR2(0.5f, 0.5f);

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		m_pItem[nCntItem] = CObject2D::Create(GetPriority());

		difpos += MyLib::Vector3(600.0f, -15.0f,0.0f);

		m_pItem[nCntItem]->SetPosition(ITEM_POS+ difpos);

		int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
		m_pItem[nCntItem]->BindTexture(nIdxTex);

		D3DXVECTOR2 size = ITEM_SIZE;

		if (nCntItem == 1)
		{
			size.x *= persize.x;
			size.y *= persize.y;
		}
		m_pItem[nCntItem]->SetSize(size);
		m_pItem[nCntItem]->SetSizeOrigin(size);

		CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

		if (nCntItem == 0)
		{
			D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(0.0f,0.0f), D3DXVECTOR2(1.0f,0.0f),  D3DXVECTOR2(0.0f,0.1f),  D3DXVECTOR2(1.0f,0.1f) };
			m_pItem[nCntItem]->SetTex(uvpos);
		}
	}
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CRankingItem::Uninit()
{

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem]!=nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}
	Release();
}

//==========================================================================
// �폜
//==========================================================================
void CRankingItem::Kill()
{

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CRankingItem::Update()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//���ڂ̍X�V
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			D3DXVECTOR2 size = m_pItem[nCntItem]->GetSize();
			size.x = UtilFunc::Correction::EasingEaseInOut(ITEM_SIZE.x * 10.0f, ITEM_SIZE.x, 0.0f, TIME_SET, m_fStateTime);
			size.y = UtilFunc::Correction::EasingEaseInOut(ITEM_SIZE.y * 10.0f, ITEM_SIZE.y, 0.0f, TIME_SET, m_fStateTime);
			m_pItem[nCntItem]->SetSize(size);

			D3DXVECTOR2 sizetext = m_pItem[nCntItem]->GetSize();
			D3DXVECTOR2 sizetextOR = m_pItem[nCntItem]->GetSizeOrigin();
			sizetext.x = UtilFunc::Correction::EasingEaseInOut(sizetextOR.x * 10.0f, sizetextOR.x, 0.0f, TIME_SET, m_fStateTime);
			sizetext.y = UtilFunc::Correction::EasingEaseInOut(sizetextOR.y * 10.0f, sizetextOR.y, 0.0f, TIME_SET, m_fStateTime);
			m_pItem[nCntItem]->SetSize(sizetext);

			m_pItem[nCntItem]->Update();
		}
	}
}
//==========================================================================
// �`�揈��
//==========================================================================
void CRankingItem::Draw()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �I�u�W�F�N�g2D�̕`��

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Draw();
		}
	}

	// �A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
