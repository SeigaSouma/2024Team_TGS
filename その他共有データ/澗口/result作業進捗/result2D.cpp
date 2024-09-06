//=============================================================================
// 
//  リザルト2Dロゴ処理 [result2D.cpp]
//  Author : 澗口将太郎
// 
//=============================================================================
#include "result2D.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "gamemanager.h"
#include "game.h"
#include "game_pressenter.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	//ロゴ情報
	const std::string TEXTURE_RESULT = "data\\TEXTURE\\result\\ninja_resultlogo.png";
	const MyLib::Vector3 LOGO_POS = MyLib::Vector3(220.0f, 60.0f, 0.0f);
	const D3DXVECTOR2 LOGO_SIZE = D3DXVECTOR2(200.0f, 50.0f);

	//背景情報
	const std::string TEXTURE_BG = "data\\TEXTURE\\result\\result_bg.png";
	const MyLib::Vector3 BG_POS = MyLib::Vector3(SCREEN_WIDTH/2+15.0f, SCREEN_HEIGHT/2+50.0f, 0.0f);
	const D3DXVECTOR2 BG_SIZE = D3DXVECTOR2(675.0f, 340.0f);
	
	const MyLib::Vector3 ITEM_POS = MyLib::Vector3(5000, 5000, 0.0f);
	const D3DXVECTOR2 ITEM_SIZE = D3DXVECTOR2(80.0f, 40.0f);
	const std::string TEXTURE_ITEM[CResult2D::ITEM_MAX] = { "data\\TEXTURE\\result\\ninja_UI_result00.png",
										"data\\TEXTURE\\result\\ninja_UI_result01.png",
										"data\\TEXTURE\\result\\ninja_UI_result02.png",
										"data\\TEXTURE\\result\\ninja_UI_result03.png",
										"data\\TEXTURE\\result\\ninja_UI_result04.png" };

	const float TIME_SET = 0.2f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CResult2D::CResult2D(int nPriority) : CObject(nPriority)
{
	m_fStateTime = 0.0f;
	m_pLogo = nullptr;
	m_pBg = nullptr;

	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		m_pItem[nCnt] = nullptr;
	}
	
}
//==========================================================================
// デストラクタ
//==========================================================================
CResult2D::~CResult2D()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CResult2D* CResult2D::Create()
{
	// メモリの確保
	CResult2D *pMarker =  new CResult2D;

	if (pMarker != nullptr)
	{
		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CResult2D::Init()
{
	//ロゴ(obj2D)初期化処理
	m_pLogo = CObject2D::Create(GetPriority());
	m_pLogo->SetPosition(LOGO_POS);

	int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_RESULT);
	m_pLogo->BindTexture(nIdxTex);

	D3DXVECTOR2 size =LOGO_SIZE;
	m_pLogo->SetSize(size);
	m_pLogo->SetSizeOrigin(size);

	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	

	//背景の初期化
	m_pBg = CObject2D::Create(GetPriority());
	m_pBg->SetPosition(BG_POS);

	nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_BG);
	m_pBg->BindTexture(nIdxTex);

	size = BG_SIZE;
	m_pBg->SetSize(size);
	m_pBg->SetSizeOrigin(size);

	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

	//項目の初期化処理
	MyLib::Vector3 difpos = MyLib::Vector3(0.0f, -70.0f, 0.0f);

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		m_pItem[nCntItem] = CObject2D::Create(GetPriority());

		difpos += MyLib::Vector3(0.0f, 70.0f,0.0f);

		m_pItem[nCntItem]->SetPosition(ITEM_POS+ difpos);

		int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
		m_pItem[nCntItem]->BindTexture(nIdxTex);

		D3DXVECTOR2 size = ITEM_SIZE;
		m_pItem[nCntItem]->SetSize(size);
		m_pItem[nCntItem]->SetSizeOrigin(size);

		CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);
	}
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CResult2D::Uninit()
{
	if (m_pLogo != nullptr)
	{
		m_pLogo->Uninit();
		m_pLogo = nullptr;
	}

	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

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
// 削除
//==========================================================================
void CResult2D::Kill()
{
	if (m_pLogo != nullptr)
	{
		m_pLogo->Uninit();
		m_pLogo = nullptr;
	}

	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

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
// 更新処理
//==========================================================================
void CResult2D::Update()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//ロゴの更新
	D3DXVECTOR2 size = m_pLogo->GetSize();
	size.x = UtilFunc::Correction::EasingEaseInOut(LOGO_SIZE.x * 10.0f, LOGO_SIZE.x, 0.0f, TIME_SET, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseInOut(LOGO_SIZE.y * 10.0f, LOGO_SIZE.y, 0.0f, TIME_SET, m_fStateTime);
	m_pLogo->SetSize(size);

	D3DXVECTOR2 sizetext = m_pLogo->GetSize();
	D3DXVECTOR2 sizetextOR = m_pLogo->GetSizeOrigin();
	sizetext.x = UtilFunc::Correction::EasingEaseInOut(sizetextOR.x * 10.0f, sizetextOR.x, 0.0f, TIME_SET, m_fStateTime);
	sizetext.y = UtilFunc::Correction::EasingEaseInOut(sizetextOR.y * 10.0f, sizetextOR.y, 0.0f, TIME_SET, m_fStateTime);
	m_pLogo->SetSize(sizetext);

	m_pLogo->Update();

	//背景の更新
	size = m_pBg->GetSize();
	size.x = UtilFunc::Correction::EasingEaseInOut(BG_SIZE.x * 10.0f, BG_SIZE.x, 0.0f, TIME_SET, m_fStateTime);
	size.y = UtilFunc::Correction::EasingEaseInOut(BG_SIZE.y * 10.0f, BG_SIZE.y, 0.0f, TIME_SET, m_fStateTime);
	m_pBg->SetSize(size);

	sizetext = m_pBg->GetSize();
	sizetextOR = m_pBg->GetSizeOrigin();
	sizetext.x = UtilFunc::Correction::EasingEaseInOut(sizetextOR.x * 10.0f, sizetextOR.x, 0.0f, TIME_SET, m_fStateTime);
	sizetext.y = UtilFunc::Correction::EasingEaseInOut(sizetextOR.y * 10.0f, sizetextOR.y, 0.0f, TIME_SET, m_fStateTime);
	m_pBg->SetSize(sizetext);

	m_pBg->Update();

	//項目の更新
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
// 描画処理
//==========================================================================
void CResult2D::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// オブジェクト2Dの描画
	m_pLogo->Draw();

	m_pBg->Draw();

	/*for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Draw();
		}
	}*/

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
