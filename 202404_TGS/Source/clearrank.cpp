//=============================================================================
// 
//  クリアランク処理 [clearrank.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "clearrank.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE[] =	// テクスチャのファイル
	{
		"data\\TEXTURE\\result\\rank_S.png",
		"data\\TEXTURE\\result\\rank_A.png",
		"data\\TEXTURE\\result\\rank_B.png",
		"data\\TEXTURE\\result\\rank_C.png",
		"data\\TEXTURE\\result\\rank_C.png",
	};
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\clearrank.png";
}

//==========================================================================
// コンストラクタ
//==========================================================================
CClearRank::CClearRank(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// ランク
	m_pText = nullptr;		// 文字
}

//==========================================================================
// デストラクタ
//==========================================================================
CClearRank::~CClearRank()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CClearRank* CClearRank::Create(CJudge::JUDGE rank)
{
	// メモリの確保
	CClearRank* pClearRank = DEBUG_NEW CClearRank;

	if (pClearRank != nullptr)
	{
		pClearRank->m_Rank = rank;

		// 初期化処理
		pClearRank->Init();
	}

	return pClearRank;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CClearRank::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_Rank]);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 100.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 100.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(1040.0f,360.0f,0.0f));

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);


	//=============================
	// 文字生成
	//=============================
	CreateText();
	return S_OK;
}

//==========================================================================
// 文字生成
//==========================================================================
void CClearRank::CreateText()
{
	// 生成
	m_pText = CObject2D::Create(GetPriority());
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 100.0f);
	m_pText->SetSize(size);
	m_pText->SetSizeOrigin(size);

	// 位置設定
	m_pText->SetPosition(GetPosition() + MyLib::Vector3(-size.x, 0.0f, 0.0f));
}

//==========================================================================
// 終了処理
//==========================================================================
void CClearRank::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CClearRank::Update()
{
	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CClearRank::Draw()
{
	// 描画処理
	CObject2D::Draw();
}
