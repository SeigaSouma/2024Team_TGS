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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\title\\enter.png";
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

//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle_Select::STATE_FUNC CTitle_Select::m_StateFunc[] =
{
	&CTitle_Select::StateNone,		// なし
	&CTitle_Select::StateFadeIn,	// フェードイン
	&CTitle_Select::StateTutorial_FadeOut,		// チュートリアル確認のフェードアウト
	&CTitle_Select::StateNoActive,	// 反応しない
	& CTitle_Select::StateSetting,	// 設定中
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle_Select::CTitle_Select(float fadetime) : m_fFadeOutTime(fadetime)
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
	CTexture* pTexture = CTexture::GetInstance();

	MyLib::Vector3 pos = SET_POS;

	// 背景を生成
	m_pSelect = CObject2D::Create(8);
	m_pSelect->SetType(CObject::TYPE_OBJECT2D);
	m_pSelect->SetPosition(pos);
	m_pSelect->BindTexture(pTexture->Regist(FILENAME::BG));
	D3DXVECTOR2 texture = pTexture->GetImageSize(m_pSelect->GetIdxTexture());
	m_pSelect->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHT));

	// 選択肢を生成
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] == nullptr)
		{
			m_ap2D[i] = CObject2D::Create(8);
			m_ap2D[i]->SetType(CObject::TYPE_OBJECT2D);
			m_ap2D[i]->SetPosition(pos);
			m_ap2D[i]->BindTexture(pTexture->Regist(FILENAME::TEXTURE[i]));
			D3DXVECTOR2 texture = pTexture->GetImageSize(m_ap2D[i]->GetIdxTexture());
			m_ap2D[i]->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, HEIGHT));
		}

		pos.x += SCREEN_WIDTH * 0.4f;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle_Select::Uninit()
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->Uninit();
			m_ap2D[i] = nullptr;
		}
	}

	if(m_pSelect != nullptr)
	{
		m_pSelect->Uninit();
		m_pSelect = nullptr;
	}

	delete this;
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
		case SELECT::SELECT_START:
		{
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE::MODE_GAME);
		}
			break;

		case SELECT::SELECT_OPTION:
		{
			SetState(STATE::STATE_SETTING);
			CTitle::GetInstance()->SetSceneType(CTitle::SCENETYPE::SCENETYPE_KEYCONFIGSETTING);
		}
			break;
		}
	}

	if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) || pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		SetState(STATE::STATE_NOACTIVE);
	}
}

//==========================================================================
// フェードイン
//==========================================================================
void CTitle_Select::StateFadeIn()
{
	// 状態遷移カウンター減算
	m_fStateTime -= CManager::GetInstance()->GetDeltaTime();

	// 不透明度更新
	float alpha = 1.0f - (m_fStateTime / m_fFadeOutTime);
	

	if (m_fStateTime <= 0.0f)
	{
		m_fStateTime = 0.0f;
		m_state = STATE_NONE;

		for (int i = 0; i < SELECT_MAX; i++)
		{
			if (m_ap2D[i] != nullptr)
			{
				m_ap2D[i]->SetAlpha(1.0f);
			}
		}
		return;
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
// 状態設定
//==========================================================================
void CTitle_Select::SetState(STATE state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 設定
//==========================================================================
void CTitle_Select::SetDraw(const bool bDraw)
{
	for (int i = 0; i < SELECT_MAX; i++)
	{
		if (m_ap2D[i] != nullptr)
		{
			m_ap2D[i]->SetEnableDisp(bDraw);
		}
	}

	if (m_pSelect != nullptr)
	{
		m_pSelect->SetEnableDisp(bDraw);
	}
}