//=============================================================================
// 
//  サンプル_オブジェクト2D処理 [sample_obj2D.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "splashwater.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SPLASH = "data\\TEXTURE\\effect\\drop_pattern_000.png";	// テクスチャのファイル
}

//==========================================================================
// コンストラクタ
//==========================================================================
CSplashwater::CSplashwater(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CSplashwater::~CSplashwater()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSplashwater* CSplashwater::Create()
{
	// メモリの確保
	CSplashwater* pObj = DEBUG_NEW CSplashwater;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSplashwater::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SPLASH);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
#endif
	SetSize(D3DXVECTOR2(100.0f, 100.0f));
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	SetAlpha(0.0f);

	// 位置、向き設定は必要があれば追加

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSplashwater::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSplashwater::Update()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 描画処理
//==========================================================================
void CSplashwater::Draw()
{
	// 描画処理
	CObject2D::Draw();
}
