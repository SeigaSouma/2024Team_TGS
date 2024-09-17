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
	const MyLib::Vector3 STR_DEFPOS = MyLib::Vector3(100.0f, 300.0f, 0.0f);	// 基点の位置
	const float STR_HEIGHT = 100.0f;	// 文字列の高さ
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
	
	MyLib::Vector3 pos = STR_DEFPOS;
	CTexture* pTex = CTexture::GetInstance();

	// 文字生成
	for (int i = 0; i < NUM_STRING; i++)
	{
		m_apObj2D[i] = CObject2D::Create();
		m_apObj2D[i]->SetType(CObject::TYPE::TYPE_NONE);
		m_apObj2D[i]->SetPosition(pos);

		// テクスチャ割り当て
		int nIdx = pTex->Regist(TEXPATH[i]);
		m_apObj2D[i]->BindTexture(nIdx);

		// サイズ変更
		D3DXVECTOR2 size = UtilFunc::Transformation::AdjustSizeByHeight(pTex->GetImageSize(nIdx), STR_HEIGHT);
		m_apObj2D[i]->SetSize(size);

		// 生成位置ずらす
		pos.x += size.x * 2.0f;

		if (i == 2)
		{// NOWとLoadingの間
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
