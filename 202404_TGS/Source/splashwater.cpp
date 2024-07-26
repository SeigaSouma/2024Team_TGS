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
	const std::string TEXTURE_SPLASH[] =
	{
		"data\\TEXTURE\\effect\\drop_pattern_000.png",
		"data\\TEXTURE\\effect\\drop_pattern_005.png",
		"data\\TEXTURE\\effect\\drop_pattern_006.png",
		"data\\TEXTURE\\effect\\drop_pattern_007.png",
		"data\\TEXTURE\\effect\\drop_pattern_008.png",
		"data\\TEXTURE\\effect\\drop_pattern_009.png",
		"data\\TEXTURE\\effect\\drop_pattern_010.png",
	};	// テクスチャのファイル
	const float TIME_FADEOUT_RATIO = 0.3f;
	const float DEFAULT_LIFE = 1.0f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CSplashwater::CSplashwater(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_fLife = 0.0f;			// 寿命
	m_fOriginLife = 0.0f;	// 寿命
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
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SPLASH[0/*UtilFunc::Transformation::Random(0, 6)*/]);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 25.0f + UtilFunc::Transformation::Random(-200, 100) * 0.1f);
#endif
	SetSize(size);
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(
		UtilFunc::Transformation::Random(-64, 64) * 10.0f + 640.0f,
		UtilFunc::Transformation::Random(-40, 100),
		0.0f));

	// 位置、向き設定は必要があれば追加

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);


	m_fLife = DEFAULT_LIFE + UtilFunc::Transformation::Random(-2, 10) * 0.1f;
	m_fOriginLife = m_fLife;
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
	CObject2D::Update();

	m_fLife -= CManager::GetInstance()->GetDeltaTime();

	float fall = 0.0f;
	if (UtilFunc::Transformation::Random(0, 2) == 0)
	{
		fall = UtilFunc::Transformation::Random(0, 2000) * 0.01f;
	}

	AddPosition(MyLib::Vector3(
		UtilFunc::Transformation::Random(-100, 100) * 0.1f,
		fall,
		0.0f));

	float lifeRatio = m_fLife / m_fOriginLife;
	if (lifeRatio <= TIME_FADEOUT_RATIO)
	{
		SetAlpha(lifeRatio / TIME_FADEOUT_RATIO);
	}

	if (m_fLife <= 0.0f)
	{
		Uninit();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CSplashwater::Draw()
{
	// 描画処理
	CObject2D::Draw();
}
