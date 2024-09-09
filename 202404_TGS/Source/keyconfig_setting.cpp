//=============================================================================
//
// キーコンフィグ設定処理 [keyconfig_setting.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig_setting.h"
#include "keyconfig.h"
#include "keyconfig_gamepad.h"
#include "keyconfig_keyboard.h"
#include "controlkeydisp.h"
#include "input_gamepad.h"
#include "texture.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::Vector3 DEFAULT_POS = MyLib::Vector3(640.0f, 120.0f, 0.0f);	// 基本座標
	const float DOWN_POSY = 70.0f;
	const float KEY_SIZE = 40.0f;
	const float FONT_WIDTH = 150.0f;
	const D3DXCOLOR FRONT_COL = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.5f);
	const float ADD_ALPHA = (1.0f / 120.0f);
}

// ファイル名
namespace FILENAME
{	
	const std::string CONFIG[INGAME::ACTION::ACT_MAX] =
	{
		"data\\TEXTURE\\ui_setting\\decide.png",
		"data\\TEXTURE\\ui_setting\\back.png",
		"data\\TEXTURE\\ui_setting\\updown.png",
		"data\\TEXTURE\\ui_setting\\air.png",
		"data\\TEXTURE\\ui_setting\\checkpoint.png",
		"data\\TEXTURE\\ui_setting\\retry.png",
		"data\\TEXTURE\\ui_setting\\pause.png",
	};
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================

//==========================================================================
// コンストラクタ
//==========================================================================
CKeyConfigSetting::CKeyConfigSetting()
{
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		m_apKeyConfig[i].s_p2D = nullptr;
		m_apKeyConfig[i].s_pKeyDisp = nullptr;
		m_apKeyConfig[i].s_p2DFront = nullptr;
	}

	m_bNowChange = false;
	m_SelectKey = 0;
	m_AlphaSin = 0.0f;
}

//==========================================================================
// 生成処理
//==========================================================================
CKeyConfigSetting* CKeyConfigSetting::Create()
{
	CKeyConfigSetting* pKeyConfig = new CKeyConfigSetting;

	if (pKeyConfig != nullptr)
	{
		pKeyConfig->Init();
	}

	return pKeyConfig;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CKeyConfigSetting::Init()
{
	CKeyConfig* pConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	// ポリゴンを生成する
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// キーを生成
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += i * DOWN_POSY;
		pos.x += FONT_WIDTH;
		m_apKeyConfig[i].s_pKeyDisp = CControlKeyDisp::Create(pos, 0.0f, KEY_SIZE, KEY_SIZE, pConfigPad->GetKey(i));

		// 文字を生成
		pos.x = DEFAULT_POS.x - FONT_WIDTH;
		m_apKeyConfig[i].s_p2D = CObject2D::Create(11);
		m_apKeyConfig[i].s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_apKeyConfig[i].s_p2D->SetPosition(pos);
		m_apKeyConfig[i].s_p2D->BindTexture(CTexture::GetInstance()->Regist(FILENAME::CONFIG[i]));
		D3DXVECTOR2 texture = CTexture::GetInstance()->GetImageSize(m_apKeyConfig[i].s_p2D->GetIdxTexture());
		m_apKeyConfig[i].s_p2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE));

		// 暗くする用のポリゴン生成
		pos.x = DEFAULT_POS.x;
		m_apKeyConfig[i].s_p2DFront = CObject2D::Create(11);
		m_apKeyConfig[i].s_p2DFront->SetPosition(pos);
		m_apKeyConfig[i].s_p2DFront->SetSize(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_apKeyConfig[i].s_p2DFront->SetColor(FRONT_COL);

	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CKeyConfigSetting::Uninit()
{
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// 2Dポリゴンの終了
		if (m_apKeyConfig[i].s_p2D != nullptr)
		{
			m_apKeyConfig[i].s_p2D->Uninit();
			m_apKeyConfig[i].s_p2D = nullptr;
		}

		// 2Dポリゴンの終了
		if (m_apKeyConfig[i].s_p2DFront != nullptr)
		{ 
			m_apKeyConfig[i].s_p2DFront->Uninit();
			m_apKeyConfig[i].s_p2DFront = nullptr;
		}

		// キーコンフィグ表示の終了
		if (m_apKeyConfig[i].s_pKeyDisp != nullptr)
		{
			m_apKeyConfig[i].s_pKeyDisp->Uninit();
			m_apKeyConfig[i].s_pKeyDisp = nullptr;
		}
	}

	delete this;
}

//==========================================================================
// 更新処理
//==========================================================================
void CKeyConfigSetting::Update()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	CKeyConfig* pKeyConfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	if (m_bNowChange) { return; }

	// 入力待ち状態にする
	if (pKeyConfig->GetTrigger(INGAME::ACT_OK))
	{
		if (m_SelectKey == INGAME::ACTION::ACT_MAX)
		{
			// キーを取得
			int oktype = pKeyConfig->GetKey(INGAME::ACT_OK);
			int backtype = pKeyConfig->GetKey(INGAME::ACT_BACK);

			// キーの入れ替え
			pKeyConfig->Join(INGAME::ACT_OK, backtype);
			pKeyConfig->Join(INGAME::ACT_BACK, oktype);

			// テクスチャ種類を設定
			m_apKeyConfig[INGAME::ACT_OK].s_pKeyDisp->SetType(backtype);
			m_apKeyConfig[INGAME::ACT_BACK].s_pKeyDisp->SetType(oktype);

			return;
		}
		// 決定キーと戻るキー以外
		else if (m_SelectKey != INGAME::ACT_OK && m_SelectKey != INGAME::ACT_BACK)
		{
			// 変更状態に
			m_bNowChange = true;

			// マルチスレッドで入力受付
			std::thread th(&CKeyConfigSetting::Chenge, this);
			th.detach();

			return;
		}
	}

	// 選択場所変更
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0))
	{
		// 上に移動
		m_SelectKey = (m_SelectKey + INGAME::ACTION::ACT_MAX - 1) % INGAME::ACTION::ACT_MAX;
		
		if (m_SelectKey < 2)
		{
			m_SelectKey = INGAME::ACTION::ACT_MAX;
		}

		m_AlphaSin = 0.0f;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0))
	{
		// 下に移動
		m_SelectKey = (m_SelectKey + 1) % (INGAME::ACTION::ACT_MAX + 1);
		m_SelectKey = UtilFunc::Transformation::Clamp(m_SelectKey, 2, static_cast<int>(INGAME::ACTION::ACT_MAX));
		m_AlphaSin = 0.0f;
	}

	m_AlphaSin += ADD_ALPHA;

	// ポリゴンを生成する
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// 使用されている場合
		if (m_apKeyConfig[i].s_p2DFront == nullptr) { continue; }

		D3DXCOLOR col = FRONT_COL;

		if (m_SelectKey != INGAME::ACT_OK && m_SelectKey != INGAME::ACT_BACK)
		{
			if (m_SelectKey == i)
			{
				col.a = sinf(m_AlphaSin);
				col.a = fabsf(col.a);
				col.a *= FRONT_COL.a;

				m_apKeyConfig[i].s_p2DFront->SetColor(col);
				m_apKeyConfig[i].s_p2DFront->SetVtx();

				continue;
			}
		}
		else
		{
			col.a = 0.0f;
		}

		m_apKeyConfig[i].s_p2DFront->SetColor(col);
		m_apKeyConfig[i].s_p2DFront->SetVtx();
	}
}

//==========================================================================
// 変更
//==========================================================================
void CKeyConfigSetting::Chenge()
{
	CKeyConfig* pKeyConfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	while (1)
	{
		if (pKeyConfig->GetRelease(INGAME::ACT_OK))
		{
			break;
		}
	}

	// 変更
	pKeyConfig->Setting(m_SelectKey);

	if (m_apKeyConfig[m_SelectKey].s_pKeyDisp != nullptr)
	{
		m_apKeyConfig[m_SelectKey].s_pKeyDisp->SetType(pKeyConfig->GetKey(m_SelectKey));
	}

	while (1)
	{
		if (!pKeyConfig->GetPress(m_SelectKey))
		{
			break;
		}
	}

	m_bNowChange = false;
}