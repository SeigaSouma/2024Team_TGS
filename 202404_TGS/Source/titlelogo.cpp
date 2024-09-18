//=============================================================================
// 
// タイトルロゴ処理 [titlelogo.cpp]
// Author : 相馬靜雅
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
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\title\\titlelogo4.png";
}

namespace TexturePath	// テクスチャパス
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

namespace Size	// サイズ
{
	const float WATER = 200.0f;
	const float PLAYER = 80.0f;
	const float BRESS = 80.0f;
	const float LOVE = 60.0f;
	const float SUITON = 80.0f;
}

namespace BasePoint	// 基点の位置
{
	const MyLib::Vector3 WATER = MyLib::Vector3(640.0f, 360.0f, 0.0f);
	const MyLib::Vector3 PLAYER = MyLib::Vector3(740.0f, 460.0f, 0.0f);
	const MyLib::Vector3 BRESS = MyLib::Vector3(550.0f, 350.0f, 0.0f);
	const MyLib::Vector3 LOVE = MyLib::Vector3(200.0f, 150.0f, 0.0f);
	const MyLib::Vector3 SUITON = MyLib::Vector3(800.0f, 150.0f, 0.0f);
}

namespace DestPoint	// 目標の位置
{
	const MyLib::Vector3 PLAYER = MyLib::Vector3(640.0f, 400.0f, 0.0f);
	const MyLib::Vector3 BRESS = MyLib::Vector3(530.0f, 320.0f, 0.0f);
	const MyLib::Vector3 LOVE[] =	// LOVEの場合は差分
	{
		MyLib::Vector3(-10.0f, -30.0f, 0.0f),
		MyLib::Vector3(-5.0f, -70.0f, 0.0f),
		MyLib::Vector3(0.0f, -50.0f, 0.0f),
		MyLib::Vector3(5.0f, -50.0f, 0.0f),
	};
}

namespace StateTime	// 状態時間
{
	const float WATER = 1.0f;
	const float PLAYER_AND_NAME = 1.0f;
	const float BRESS = 1.0f;
	const float AFTERMOVEMENT = 1.0f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitleLogo::STATE_FUNC CTitleLogo::m_StateFunc[] =
{
	&CTitleLogo::StateNone,					// なし
	&CTitleLogo::StateFadeIn_Water,			// 水フェードイン
	&CTitleLogo::StateFadeIn_PlayerAndName,	// プレイヤーと名前フェードイン
	&CTitleLogo::StateBress,				// 息
	&CTitleLogo::StateAfterMovement,		// 後の動き
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitleLogo::CTitleLogo(float fadetime, int nPriority) : m_fFadeOutTime(fadetime), CObject(nPriority)
{
	// 値のクリア
	m_state = STATE_NONE;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター
}

//==========================================================================
// デストラクタ
//==========================================================================
CTitleLogo::~CTitleLogo()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTitleLogo* CTitleLogo::Create(float fadetime)
{
	// メモリの確保
	CTitleLogo* pScreen = DEBUG_NEW CTitleLogo(fadetime);

	if (pScreen != nullptr)
	{
		// 初期化処理
		pScreen->Init();
	}

	return pScreen;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTitleLogo::Init()
{
	// 種類の設定
	SetType(TYPE_OBJECT2D);

	// 水生成
	CreateWater();

	// プレイヤー生成
	CreatePlayer();

	// 息生成
	CreateBress();

	// LOVE生成
	CreateLOVE();

	// SUITON生成
	CreateSUITON();

	m_state = State::STATE_FADEIN_WATER;	// 状態
	m_fStateTime = 0.0f;	// 状態カウンター

	return S_OK;
}

//==========================================================================
// 水生成
//==========================================================================
void CTitleLogo::CreateWater()
{
	// 生成
	m_pWater = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pWater;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置設定
	pObj2D->SetPosition(BasePoint::WATER);
	pObj2D->SetOriginPosition(BasePoint::WATER);


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TexturePath::WATER);
	pObj2D->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::WATER);

	// サイズ設定
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);
}

//==========================================================================
// プレイヤー生成
//==========================================================================
void CTitleLogo::CreatePlayer()
{
	// 生成
	m_pPlayer = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pPlayer;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置設定
	pObj2D->SetPosition(BasePoint::PLAYER);
	pObj2D->SetOriginPosition(BasePoint::PLAYER);


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TexturePath::PLAYER);
	pObj2D->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::PLAYER);

	// サイズ設定
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);
}

//==========================================================================
// 息生成
//==========================================================================
void CTitleLogo::CreateBress()
{
	// 生成
	m_pBress = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pBress;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置設定
	pObj2D->SetPosition(BasePoint::BRESS);
	pObj2D->SetOriginPosition(BasePoint::BRESS);


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TexturePath::BRESS);
	pObj2D->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::BRESS);

	// サイズ設定
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);
}

//==========================================================================
// LOVE生成
//==========================================================================
void CTitleLogo::CreateLOVE()
{
	for (int i = 0; i < LOGO_LOVE::MAX; i++)
	{
		// 生成
		m_pLOVE[i] = CObject2D::Create(GetPriority());
		CObject2D* pObj2D = m_pLOVE[i];
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// テクスチャ設定
		int texID = CTexture::GetInstance()->Regist(TexturePath::LOVE[i]);
		pObj2D->BindTexture(texID);

		// 縦幅を元にサイズ設定
		D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::LOVE);

		// サイズ設定
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);

		// 位置設定
		pObj2D->SetPosition(BasePoint::LOVE + (MyLib::Vector3(size.x * 1.5f, 0.0f, 0.0f) * i));
		pObj2D->SetOriginPosition(pObj2D->GetPosition());
	}
}

//==========================================================================
// SUITON生成
//==========================================================================
void CTitleLogo::CreateSUITON()
{
	// 生成
	m_pSUITON = CObject2D::Create(GetPriority());
	CObject2D* pObj2D = m_pSUITON;
	pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置設定
	pObj2D->SetPosition(BasePoint::SUITON);
	pObj2D->SetOriginPosition(BasePoint::SUITON);


	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TexturePath::SUITON);
	pObj2D->BindTexture(texID);

	// 縦幅を元にサイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, Size::SUITON);

	// サイズ設定
	pObj2D->SetSize(size);
	pObj2D->SetSizeOrigin(size);
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitleLogo::Uninit()
{
	// 終了処理
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitleLogo::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	// 状態別更新
	UpdateState();
	if (IsDeath())
	{
		return;
	}

	
}

//==========================================================================
// 状態更新
//==========================================================================
void CTitleLogo::UpdateState()
{
	// 状態遷移カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// なにもなし
//==========================================================================
void CTitleLogo::StateNone()
{
	m_fStateTime = 0.0f;
}

//==========================================================================
// 水フェードイン
//==========================================================================
void CTitleLogo::StateFadeIn_Water()
{
	// 不透明度設定
	float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::WATER, 0.0f, 1.0f);
	m_pWater->SetAlpha(alpha);

	if (m_fStateTime >= StateTime::WATER)
	{// 時間経過
		SetState(State::STATE_FADEIN_PLAYER_and_NAME);
	}
}

//==========================================================================
// プレイヤーと名前フェードイン
//==========================================================================
void CTitleLogo::StateFadeIn_PlayerAndName()
{
	//=============================
	// プレイヤー
	//=============================
	{
		// 不透明度設定
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::PLAYER_AND_NAME, 0.0f, 1.0f);
		m_pPlayer->SetAlpha(alpha);

		// プレイヤー移動
		MyLib::Vector3 playerpos = UtilFunc::Correction::EasingLinear(BasePoint::PLAYER, DestPoint::PLAYER, 0.0f, StateTime::PLAYER_AND_NAME, m_fStateTime);
		m_pPlayer->SetPosition(playerpos);
	}

	//=============================
	// 名前
	//=============================
	{
		// 不透明度設定
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::PLAYER_AND_NAME, 0.0f, 1.0f);
		for (const auto& love : m_pLOVE)
		{
			love->SetAlpha(alpha);
		}
		m_pSUITON->SetAlpha(alpha);
	}

	if (m_fStateTime >= StateTime::PLAYER_AND_NAME)
	{// 時間経過
		SetState(State::STATE_BRESS);
	}
}

//==========================================================================
// 息
//==========================================================================
void CTitleLogo::StateBress()
{
	//=============================
	// 息
	//=============================
	{
		// 不透明度設定
		float alpha = UtilFunc::Transformation::Clamp(m_fStateTime / StateTime::BRESS, 0.0f, 1.0f);
		m_pBress->SetAlpha(alpha);

		// 移動
		MyLib::Vector3 pos = UtilFunc::Correction::EasingLinear(BasePoint::BRESS, DestPoint::BRESS, 0.0f, StateTime::BRESS, m_fStateTime);
		m_pBress->SetPosition(pos);
	}

	//=============================
	// LOVE
	//=============================
	{
		// 移動
		for (int i = 0; i < LOGO_LOVE::MAX; i++)
		{
			// 移動
			MyLib::Vector3 pos = UtilFunc::Correction::EasingLinear(
				m_pLOVE[i]->GetOriginPosition(),
				m_pLOVE[i]->GetOriginPosition() + DestPoint::LOVE[i],
				0.0f, StateTime::BRESS, m_fStateTime);
			m_pLOVE[i]->SetPosition(pos);
		}
	}


	if (m_fStateTime >= StateTime::BRESS)
	{// 時間経過
		SetState(State::STATE_AFTERMOVEMENT);
	}
}

//==========================================================================
// 後の動き
//==========================================================================
void CTitleLogo::StateAfterMovement()
{
	//if (m_fStateTime >= StateTime::AFTERMOVEMENT)
	//{// 時間経過
	//	SetState(State::STATE_FADEIN_PLAYER_and_NAME);
	//}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitleLogo::Draw()
{
	
}

//==========================================================================
// 状態設定
//==========================================================================
void CTitleLogo::SetState(State state)
{ 
	m_state = state; 
	m_fStateTime = 0.0f;
}
