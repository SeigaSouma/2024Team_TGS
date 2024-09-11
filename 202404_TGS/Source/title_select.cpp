//=============================================================================
// 
// タイトルエンター処理 [title_select.cpp]
// Author : Ibuki Okusada
// 
//=============================================================================
#include "title_select.h"
#include "title.h"
#include "manager.h"
#include "sound.h"
#include "calculation.h"
#include "input.h"
#include "fade.h"
#include "keyconfig.h"
#include "title_pressenter.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float TIME_TUTORIAL_FADEOUT = 0.3f;	// チュートリアル確認のフェードアウト
	const MyLib::Vector3 SET_POS = MyLib::Vector3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.85f, 0.0f);
	const float HEIGHT = 100.0f;
}

namespace FILENAME
{
	const std::string TEXTURE[CTitle_Select::SELECT_MAX] =
	{
		"data\\TEXTURE\\title\\start.png",
		"data\\TEXTURE\\title\\option.png",
	};

	const std::string BG = "data\\TEXTURE\\ui_setting\\front.png";
}

namespace StateTime
{
	const float FADE = 0.3f;
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle_Select::STATE_FUNC CTitle_Select::m_StateFunc[] =
{
	&CTitle_Select::StateNone,		// なし
	&CTitle_Select::StateFadeIn,	// フェードイン
	&CTitle_Select::StateFadeOut,	// フェードアウト
	&CTitle_Select::StateTutorial_FadeOut,		// チュートリアル確認のフェードアウト
	&CTitle_Select::StateNoActive,	// 反応しない
	&CTitle_Select::StateSetting,	// 設定中
	&CTitle_Select::StateBack,		// 戻る
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle_Select::CTitle_Select(float fadetime) : CObject() , m_fFadeOutTime(fadetime)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;	// 状態
	m_fStateTime = 0.0f;			// 状態カウンター
	m_nSelect = 0;

	for (int i = 0; i < SELECT_MAX; i++)
	{
		m_ap2D[i] = nullptr;
	}

	m_pSelect = nullptr;
	m_bPress = false;
}

//==========================================================================
// 生成処理
//==========================================================================
CTitle_Select* CTitle_Select::Create(float fadetime)
{
	// メモリの確保
	CTitle_Select* pScreen = DEBUG_NEW CTitle_Select(fadetime);

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
HRESULT CTitle_Select::Init()
{
	// 種類設定
	CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);


	CTexture* pTexture = CTexture::GetInstance();

	MyLib::Vector3 pos = SET_POS;

	//=============================
	// 背景を生成
	//=============================
	m_pSelect = CObject2D::Create(8);
	m_pSelect->SetType(CObject::TYPE_OBJECT2D);
	m_pSelect->SetPosition(pos);
	m_pSelect->SetAlpha(0.0f);

	// テクスチャ設定
	m_pSelect->BindTexture(pTexture->Regist(FILENAME::BG));
	D3DXVECTOR2 texture = pTexture->GetImageSize(m_pSelect->GetIdxTexture());
	m_pSelect->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHT));
	m_pSelect->SetSizeOrigin(m_pSelect->GetSize());

	//=============================
	// 選択肢を生成
	//=============================
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] == nullptr)
		{
			// 生成
			m_ap2D[i] = CObject2D::Create(8);
			CObject2D* pObj2D = m_ap2D[i];

			pObj2D->SetType(CObject::TYPE_OBJECT2D);
			pObj2D->SetPosition(pos);
			pObj2D->SetAlpha(0.0f);

			// テクスチャ設定
			pObj2D->BindTexture(pTexture->Regist(FILENAME::TEXTURE[i]));
			D3DXVECTOR2 texture = pTexture->GetImageSize(pObj2D->GetIdxTexture());
			pObj2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHT));
			pObj2D->SetSizeOrigin(pObj2D->GetSize());
		}

		pos.x += SCREEN_WIDTH * 0.4f;
	}

	// 状態遷移
	SetState(STATE::STATE_FADEIN);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle_Select::Uninit()
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		m_ap2D[i] = nullptr;
	}
	m_pSelect = nullptr;

	Release();
}

//==========================================================================
// 削除
//==========================================================================
void CTitle_Select::Kill()
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->Uninit();
			m_ap2D[i] = nullptr;
		}
	}

	if (m_pSelect != nullptr)
	{
		m_pSelect->Uninit();
		m_pSelect = nullptr;
	}

	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle_Select::Update()
{
	if (CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// フェード中は抜ける
		return;
	}

	// 状態遷移カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態別更新処理
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// なにもなし
//==========================================================================
void CTitle_Select::StateNone()
{
	if (CTitle::GetInstance()->GetSetting() != nullptr)
	{
		return;
	}

	// 入力情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
	CKeyConfig* pKeyConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKey = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (m_bPress)
	{
		if (!pKeyConfigPad->GetPress(INGAME::ACT_BACK))
		{
			m_bPress = false;
		}
		return;
	}

	// 方向入力
	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_LEFT, 0) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_RIGHT, 0)
		|| pInputKeyboard->GetTrigger(DIK_A) || pInputKeyboard->GetTrigger(DIK_D))
	{
		m_nSelect ^= 1;

		MyLib::Vector3 pos = SET_POS;
		pos.x += (SCREEN_WIDTH * 0.4f * m_nSelect);
		m_pSelect->SetPosition(pos);

		return;
	}

	if (pKeyConfigPad->GetTrigger(INGAME::ACT_OK) || pKeyConfigKey->GetTrigger(INGAME::ACT_OK))
	{
		switch (m_nSelect)
		{
		case SELECT::SELECT_START:	// ゲーム開始
		{
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE::MODE_GAME);
		}
			break;

		case SELECT::SELECT_OPTION:	// オプション
		{
			SetState(STATE::STATE_FADEOUT);

			// キーコンフィグ設定
			CTitle::GetInstance()->SetSceneType(CTitle::SCENETYPE::SCENETYPE_KEYCONFIGSETTING);
		}
			break;
		}
	}

	if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) || pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		SetState(STATE::STATE_BACK);

		// プレスエンター呼び戻し
		CTitle_PressEnter* pEnter = CTitle::GetInstance()->GetTitlePressEnter();
		if (pEnter != nullptr)
		{
			pEnter->SetState(CTitle_PressEnter::STATE::STATE_FADEIN);
		}

	}
}

//==========================================================================
// フェードイン
//==========================================================================
void CTitle_Select::StateFadeIn()
{
	// 不透明度更新
	float alpha = m_fStateTime / StateTime::FADE;
	
	if (m_fStateTime >= StateTime::FADE)
	{
		// 状態遷移
		SetState(STATE::STATE_NONE);
		alpha = 1.0f;
	}


	// 背景の不透明度
	m_pSelect->SetAlpha(alpha);

	// 選択肢の不透明度
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CTitle_Select::StateFadeOut()
{
	// 不透明度更新
	float alpha = 1.0f - (m_fStateTime / StateTime::FADE);


	if (m_fStateTime >= StateTime::FADE)
	{
		// 状態遷移
		SetState(STATE::STATE_SETTING);
		alpha = 0.0f;
	}

	// 背景の不透明度
	m_pSelect->SetAlpha(alpha);

	// 選択肢の不透明度
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// チュートリアル確認のフェードアウト
//==========================================================================
void CTitle_Select::StateTutorial_FadeOut()
{
	// 状態遷移カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, TIME_TUTORIAL_FADEOUT, m_fStateTime);

	if (m_fStateTime >= TIME_TUTORIAL_FADEOUT)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_NONE;
	}
}


//==========================================================================
// 反応しない
//==========================================================================
void CTitle_Select::StateNoActive()
{
	
}

//==========================================================================
// 設定中
//==========================================================================
void CTitle_Select::StateSetting()
{
	m_bPress = true;
}

//==========================================================================
// 戻る
//==========================================================================
void CTitle_Select::StateBack()
{
	// 不透明度更新
	float alpha = 1.0f - (m_fStateTime / StateTime::FADE);
	alpha = UtilFunc::Transformation::Clamp(alpha, 0.0f, 1.0f);

	if (m_fStateTime >= StateTime::FADE)
	{
		SetState(STATE::STATE_NOACTIVE);
	}

	// 背景の不透明度
	m_pSelect->SetAlpha(alpha);

	// 選択肢の不透明度
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetAlpha(alpha);
		}
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CTitle_Select::SetState(STATE state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 描画
//==========================================================================
void CTitle_Select::Draw()
{
	return;
}