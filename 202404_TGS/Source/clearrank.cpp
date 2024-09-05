//=============================================================================
// 
//  クリアランク処理(一番上) [clearrank.cpp]
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
	const float MOVEVALUE_TEXT = 3.0f;	//テキストの移動量
}

namespace StateTime
{
	const float WAIT = 0.5f;	// 待機
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CClearRank::STATE_FUNC CClearRank::m_StateFunc[] =
{
	&CClearRank::StateScrollText,	// 文字送り
	&CClearRank::StateSrollVoid,	// 空間送り
	&CClearRank::StateScrollRank,	// ランク送り
	&CClearRank::StateFinish,		// 終了
	&CClearRank::StateNone,			// なにもなし

};

//==========================================================================
// コンストラクタ
//==========================================================================
CClearRank::CClearRank(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
	m_Rank = CJudge::JUDGE::JUDGE_DDD;	// ランク
	m_pText = nullptr;		// 文字


	m_fStateTime = 0.0f;		// 状態カウンター
	m_state = State::STATE_SCROLL_TEXT;			// 状態
	m_fMoveTextLen = 0.0f;	// テキストの移動距離
	m_fMoveRankLen = 0.0f;	// ランクの移動距離
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
	SetSize(D3DXVECTOR2(0.0f, size.y));
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(1040.0f,360.0f,0.0f));

	// 種類の設定
	SetType(CObject::TYPE::TYPE_ENEMY);

	// アンカーポイントの設定
	SetAnchorType(AnchorPoint::LEFT);

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
	m_pText->SetSize(D3DXVECTOR2(0.0f, size.y));
	m_pText->SetSizeOrigin(size);

	// 位置設定
	m_pText->SetPosition(GetPosition() + MyLib::Vector3(-size.x, 0.0f, 0.0f));

	// アンカーポイントの設定
	m_pText->SetAnchorType(AnchorPoint::LEFT);
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
	// 状態更新
	UpdateState();

	// 更新処理
	CObject2D::Update();
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CClearRank::SetVtx()
{
	CObject2D::SetVtx();
}

//==========================================================================
// 状態更新
//==========================================================================
void CClearRank::UpdateState()
{
	// 状態タイマー
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// 文字送り
//==========================================================================
void CClearRank::StateScrollText()
{
	// サイズ取得
	D3DXVECTOR2 size = m_pText->GetSize(), sizeOrigin = m_pText->GetSizeOrigin();

	// テキスト移動距離加算
	m_fMoveTextLen += MOVEVALUE_TEXT;
	m_fMoveTextLen = UtilFunc::Transformation::Clamp(m_fMoveTextLen, 0.0f, sizeOrigin.x);

	if (m_fMoveTextLen >= sizeOrigin.x)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_VOID);
	}

	// サイズ設定
	size.x = m_fMoveTextLen;
	m_pText->SetSize(size);

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = m_pText->GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);
}

//==========================================================================
// 空間送り
//==========================================================================
void CClearRank::StateSrollVoid()
{

	if (m_fStateTime >= StateTime::WAIT)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_RANK);
	}

}

//==========================================================================
// ランク送り
//==========================================================================
void CClearRank::StateScrollRank()
{
	// サイズ取得
	D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();

	// テキスト移動距離加算
	m_fMoveRankLen += MOVEVALUE_TEXT;
	m_fMoveRankLen = UtilFunc::Transformation::Clamp(m_fMoveRankLen, 0.0f, sizeOrigin.x);

	if (m_fMoveRankLen >= sizeOrigin.x)
	{
		// 状態遷移
		SetState(State::STATE_FINISH);
	}

	// サイズ設定
	size.x = m_fMoveRankLen;
	SetSize(size);

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);
}

//==========================================================================
// 終了
//==========================================================================
void CClearRank::StateFinish()
{
	// サイズ設定
	SetSize(GetSizeOrigin());
	m_pText->SetSize(m_pText->GetSizeOrigin());

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = GetTex();
	D3DXVECTOR2* pTexText = m_pText->GetTex();
	pTex[1].x = pTex[3].x = pTexText[1].x = pTexText[3].x = 1.0f;

	// 状態遷移
	SetState(State::STATE_NONE);
}

//==========================================================================
// 状態設定
//==========================================================================
void CClearRank::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 描画処理
//==========================================================================
void CClearRank::Draw()
{
	// 描画処理
	CObject2D::Draw();
}
