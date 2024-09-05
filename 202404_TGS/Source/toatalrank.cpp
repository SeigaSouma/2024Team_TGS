//=============================================================================
// 
//  トータルランク処理 [toatalrank.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "toatalrank.h"
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
	};
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\toatalrank.png";

	const int RANKPOINT[] =	// ランクの基準
	{
		4,
		3,
		2,
		1
	};

	const float TIMEPOINT[] =	// タイマーの基準
	{
		80.0f,
		100.0f,
		120.0f,
		140.0f
	};

	const float SIZE_HEIGHT = 50.0f;	// 縦幅のサイズ
	const float MOVEVALUE_TEXT = 3.0f;	//テキストの移動量
}

//==========================================================================
// コンストラクタ
//==========================================================================
CToatalRank::CToatalRank(int nPriority) : CObject2D(nPriority)
{
	// 値のトータル
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// ランク
}

//==========================================================================
// デストラクタ
//==========================================================================
CToatalRank::~CToatalRank()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CToatalRank* CToatalRank::Create(CJudge::JUDGE rank, float time)
{
	// メモリの確保
	CToatalRank* pClearRank = DEBUG_NEW CToatalRank;

	if (pClearRank != nullptr)
	{
		pClearRank->m_Rank = pClearRank->CalRank(rank, time);

		// 初期化処理
		pClearRank->Init();
	}

	return pClearRank;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CToatalRank::Init()
{
	//=============================
	// 文字生成
	//=============================
	CreateText();



	// オブジェクト2Dの初期化
	CObject2D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE[m_Rank]);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 120.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);
	SetPosition(m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSize().x * 2.0f + 50.0f,100.0f,0.0f));

	// 位置、向き設定は必要があれば追加

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 文字生成
//==========================================================================
void CToatalRank::CreateText()
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
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(size);
	m_pText->SetSizeOrigin(size);

	// 位置設定
	m_pText->SetPosition(MyLib::Vector3(700.0f, 400.0f, 0.0f));

	// アンカーポイントの設定
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// 終了処理
//==========================================================================
void CToatalRank::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CToatalRank::Update()
{
	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CToatalRank::Draw()
{
	// 描画処理
	CObject2D::Draw();
}

//==========================================================================
// ランク計算
//==========================================================================
CJudge::JUDGE CToatalRank::CalRank(CJudge::JUDGE rank, float time)
{
	int point = 0, i = 0;

	// 時間のポイント計算
	while (1)
	{
		if (TIMEPOINT[i] >= time)
		{
			point += CJudge::JUDGE::JUDGE_DDD - (i);
			break;
		}
	}
	
	// ランクのポイント計算
	point += CJudge::JUDGE::JUDGE_DDD - rank;

	if (point >= CJudge::JUDGE::JUDGE_DDD * 2)
	{
		return CJudge::JUDGE::JUDGE_AAA;
	}
	else if (point >= CJudge::JUDGE::JUDGE_CCC * 2)
	{
		return CJudge::JUDGE::JUDGE_BBB;
	}
	else if (point >= CJudge::JUDGE::JUDGE_BBB * 2)
	{
		return CJudge::JUDGE::JUDGE_CCC;
	}
	else
	{
		return CJudge::JUDGE::JUDGE_DDD;
	}
}