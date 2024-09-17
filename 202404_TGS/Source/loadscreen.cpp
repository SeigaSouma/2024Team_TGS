//=============================================================================
// 
//  フェード処理 [loadscreen.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "loadscreen.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object2D.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\load\\loadscreen.jpg";
	const MyLib::Vector3 STR_DEFPOS = MyLib::Vector3(100.0f, 300.0f, 0.0f);
	const float MOVE_X = 100.0f;
	const int NUM_STRING = 10;	// NOWLOADINGの文字数
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
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CLoadScreen::CLoadScreen()
{
	// 値のクリア
	m_aObject2D = nullptr;					// オブジェクト2Dのオブジェクト

	for (int i = 0; i < NUM_STRING; i++)
	{
		m_apObj2D[i] = nullptr;
	}
}

//==========================================================================
// デストラクタ
//==========================================================================
CLoadScreen::~CLoadScreen()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CLoadScreen *CLoadScreen::Create()
{
	// 生成用のオブジェクト
	CLoadScreen *pFade = nullptr;

	if (pFade == nullptr)
	{// nullptrだったら

		// メモリの確保
		pFade = DEBUG_NEW CLoadScreen;

		if (pFade != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			if (FAILED(pFade->Init()))
			{// 失敗していたら
				return nullptr;
			}
		}

		return pFade;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CLoadScreen::Init()
{
	// 生成処理
	m_aObject2D = CObject2D_Anim::Create(0.0f, 5, 9, 2, false);
	if (m_aObject2D == nullptr)
	{// 失敗していたら
		return E_FAIL;
	}

	MyLib::Vector3 pos = STR_DEFPOS;

	// 文字生成
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
// 終了処理
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
// 更新処理
//==========================================================================
void CLoadScreen::Update()
{
	// 文字生成
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Update();
		}
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CLoadScreen::Draw()
{
	// 文字生成
	for (int i = 0; i < NUM_STRING; i++)
	{
		if (m_apObj2D[i] != nullptr)
		{
			m_apObj2D[i]->Draw();
		}
	}
}

//==========================================================================
// オブジェクト2Dオブジェクトの取得
//==========================================================================
CObject2D_Anim*CLoadScreen::GetMyObject()
{
	return m_aObject2D;
}