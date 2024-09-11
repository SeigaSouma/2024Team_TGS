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
#include "scroll.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::Vector3 DEFAULT_POS = MyLib::Vector3(640.0f, -50.0f, 0.0f);	// 基本座標
	const float DOWN_POSY = 100.0f;
	const float KEY_SIZE = 50.0f;
	const float FONT_WIDTH = 150.0f;
	const float TITLE_HEIGHT = KEY_SIZE * 1.3f;
	const D3DXCOLOR FRONT_COL = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.0f);
	const float ADD_ALPHA = (1.0f / 20.0f);
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

	const std::string FRONT = "data\\TEXTURE\\ui_setting\\front.png";
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
		m_aKeyConfig[i].s_p2D = nullptr;
		m_aKeyConfig[i].s_pKeyDisp = nullptr;
		m_aKeyConfig[i].s_p2DFront = nullptr;
	}

	m_checkconfig.s_p2D = nullptr;
	m_checkconfig.s_pKeyDispOK = nullptr;
	m_checkconfig.s_pKeyDispNO = nullptr;
	m_checkconfig.s_p2DFront = nullptr;
	m_pTitle2D = nullptr;
	m_pScroll = nullptr;
	m_bNowChange = false;
	m_SelectKey = 0;
	m_Alpha = 0.0f;
	m_fTime = 0.0f;
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
	CTexture* pTexture = CTexture::GetInstance();
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// 背景
	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y = SCREEN_HEIGHT * 0.5f;
		m_pScroll = CScroll::Create(pos, 0.05f, DOWN_POSY * INGAME::ACTION::ACT_MAX * 0.6f, FONT_WIDTH * 5.5f, false, false, 8);
	}

	// 説明文字生成
	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY;
		m_pTitle2D = CObject2D::Create(11);
		m_pTitle2D->SetType(CObject::TYPE_OBJECT2D);
		m_pTitle2D->SetPosition(pos);
		m_pTitle2D->BindTexture(pTexture->Regist("data\\TEXTURE\\ui_setting\\setting.png"));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_pTitle2D->GetIdxTexture());
		m_pTitle2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, TITLE_HEIGHT));
		m_pTitle2D->SetColor(col);
	}

	// ポリゴンを生成する
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// キーを生成
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += i * DOWN_POSY;
		pos.x += FONT_WIDTH;
		m_aKeyConfig[i].s_pKeyDisp = CControlKeyDisp::Create(pos, 0.0f, KEY_SIZE, KEY_SIZE, pConfigPad->GetKey(i));
		m_aKeyConfig[i].s_pKeyDisp->SetColor(col);

		// 文字を生成
		pos.x = DEFAULT_POS.x - FONT_WIDTH;
		m_aKeyConfig[i].s_p2D = CObject2D::Create(11);
		m_aKeyConfig[i].s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_aKeyConfig[i].s_p2D->SetPosition(pos);
		m_aKeyConfig[i].s_p2D->BindTexture(pTexture->Regist(FILENAME::CONFIG[i]));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_aKeyConfig[i].s_p2D->GetIdxTexture());

		// サイズ設定
		D3DXVECTOR2 setsize = UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE);
		m_aKeyConfig[i].s_p2D->SetSize(setsize);
		m_aKeyConfig[i].s_p2D->SetSizeOrigin(setsize);
		m_aKeyConfig[i].s_p2D->SetColor(col);

		// 暗くする用のポリゴン生成
		pos.x = DEFAULT_POS.x;
		m_aKeyConfig[i].s_p2DFront = CObject2D::Create(10);
		m_aKeyConfig[i].s_p2DFront->SetSize(D3DXVECTOR2(0.0f, KEY_SIZE));
		m_aKeyConfig[i].s_p2DFront->SetSizeOrigin(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_aKeyConfig[i].s_p2DFront->SetColor(FRONT_COL);
		m_aKeyConfig[i].s_p2DFront->BindTexture(pTexture->Regist(FILENAME::FRONT));

		m_aKeyConfig[i].s_p2DFront->SetAnchorType(CObject2D::AnchorPoint::LEFT);
		m_aKeyConfig[i].s_p2DFront->SetPosition(pos - MyLib::Vector3(KEY_SIZE * 6.0f, 0.0f, 0.0f));

		if (i <= INGAME::ACT_BACK)
		{
			m_aKeyConfig[i].s_pKeyDisp->SetEnableDisp(false);
			m_aKeyConfig[i].s_p2D->SetEnableDisp(false);
			m_aKeyConfig[i].s_p2DFront->SetEnableDisp(false);
		}

	}

	// 決定戻る変更用ポリゴン生成
	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY * INGAME::ACTION::ACT_MAX;

		// 文字の生成
		m_checkconfig.s_p2D = CObject2D::Create(11);
		m_checkconfig.s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_checkconfig.s_p2D->SetPosition(pos);
		m_checkconfig.s_p2D->BindTexture(pTexture->Regist("data\\TEXTURE\\ui_setting\\change.png"));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_checkconfig.s_p2D->GetIdxTexture());
		m_checkconfig.s_p2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE * 0.8f));
		m_checkconfig.s_p2D->SetColor(col);

		// キーコンフィグを生成
		float size = KEY_SIZE * 0.75f;
		pos.x += (size * 3);
		m_checkconfig.s_pKeyDispOK = CControlKeyDisp::Create(pos, 0.0f, size, size,
			pConfigPad->GetKey(INGAME::ACT_OK));
		pos.x += (size * 3);
		m_checkconfig.s_pKeyDispOK->SetColor(col);
		m_checkconfig.s_pKeyDispNO = CControlKeyDisp::Create(pos, 0.0f, size, size,
			pConfigPad->GetKey(INGAME::ACT_BACK));
		m_checkconfig.s_pKeyDispNO->SetColor(col);

		// 暗くする用のポリゴン生成
		pos = DEFAULT_POS;
		pos.y += DOWN_POSY * INGAME::ACTION::ACT_MAX;
		m_checkconfig.s_p2DFront = CObject2D::Create(10);
		m_checkconfig.s_p2DFront->SetPosition(pos);
		m_checkconfig.s_p2DFront->SetSize(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_checkconfig.s_p2DFront->SetColor(FRONT_COL);
		m_checkconfig.s_p2DFront->BindTexture(pTexture->Regist(FILENAME::FRONT));
	}

	m_SelectKey = INGAME::ACT_BACK + 1;

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
		if (m_aKeyConfig[i].s_p2D != nullptr)
		{
			m_aKeyConfig[i].s_p2D->Uninit();
			m_aKeyConfig[i].s_p2D = nullptr;
		}

		// 2Dポリゴンの終了
		if (m_aKeyConfig[i].s_p2DFront != nullptr)
		{ 
			m_aKeyConfig[i].s_p2DFront->Uninit();
			m_aKeyConfig[i].s_p2DFront = nullptr;
		}

		// キーコンフィグ表示の終了
		if (m_aKeyConfig[i].s_pKeyDisp != nullptr)
		{
			m_aKeyConfig[i].s_pKeyDisp->Uninit();
			m_aKeyConfig[i].s_pKeyDisp = nullptr;
		}
	}

	{
		// 2Dポリゴンの終了
		if (m_checkconfig.s_p2D != nullptr)
		{
			m_checkconfig.s_p2D->Uninit();
			m_checkconfig.s_p2D = nullptr;
		}

		// 2Dポリゴンの終了
		if (m_checkconfig.s_p2DFront != nullptr)
		{
			m_checkconfig.s_p2DFront->Uninit();
			m_checkconfig.s_p2DFront = nullptr;
		}

		// キーコンフィグ表示の終了
		if (m_checkconfig.s_pKeyDispOK != nullptr)
		{
			m_checkconfig.s_pKeyDispOK->Uninit();
			m_checkconfig.s_pKeyDispOK = nullptr;
		}

		if (m_checkconfig.s_pKeyDispNO != nullptr)
		{
			m_checkconfig.s_pKeyDispNO->Uninit();
			m_checkconfig.s_pKeyDispNO = nullptr;
		}
	}

	// タイトル
	if (m_pTitle2D != nullptr)
	{
		m_pTitle2D->Uninit();
		m_pTitle2D = nullptr;
	}

	// 背景
	if (m_pScroll != nullptr)
	{
		m_pScroll->SetState(CScroll::STATE::STATE_CLOSE);
		m_pScroll = nullptr;
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

	if (m_pScroll != nullptr)
	{
		// スクロール終了まで反応しない
		if (m_pScroll->GetState() != CScroll::STATE::STATE_WAIT)
		{
			return;
		}

		SetAlpha();
	}

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
			m_aKeyConfig[INGAME::ACT_OK].s_pKeyDisp->SetType(backtype);
			m_aKeyConfig[INGAME::ACT_BACK].s_pKeyDisp->SetType(oktype);
			m_checkconfig.s_pKeyDispOK->SetType(backtype);
			m_checkconfig.s_pKeyDispNO->SetType(oktype);

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

		// タイマーリセット
		m_aKeyConfig[m_SelectKey].drawtime = 0.0f;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0))
	{
		// 下に移動
		m_SelectKey = (m_SelectKey + 1) % (INGAME::ACTION::ACT_MAX + 1);
		m_SelectKey = UtilFunc::Transformation::Clamp(m_SelectKey, 2, static_cast<int>(INGAME::ACTION::ACT_MAX));

		// タイマーリセット
		m_aKeyConfig[m_SelectKey].drawtime = 0.0f;
	}

	// デルタタイム
	float deltaTime = CManager::GetInstance()->GetDeltaTime();

	// ポリゴンを生成する
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// 使用されている場合
		if (m_aKeyConfig[i].s_p2DFront == nullptr) { continue; }

		D3DXCOLOR col = FRONT_COL;

		if (i != INGAME::ACT_OK && i != INGAME::ACT_BACK)
		{
			if (m_SelectKey == i)
			{
				col.a = m_Alpha;
			}
		}

		// タイマー加算
		m_aKeyConfig[i].drawtime += deltaTime;



		CObject2D* pObj2D = m_aKeyConfig[i].s_p2DFront;

		// サイズ設定
		D3DXVECTOR2 size = pObj2D->GetSize(), sizeOrigin = pObj2D->GetSizeOrigin();
		size.x = UtilFunc::Correction::EaseInExpo(0.0f, sizeOrigin.x, 0.0f, 0.2f, m_aKeyConfig[i].drawtime);
		m_aKeyConfig[i].s_p2DFront->SetSize(size);

		// UV座標設定
		D3DXVECTOR2* pTex = pObj2D->GetTex();
		pTex[1].x = pTex[3].x = UtilFunc::Transformation::Clamp(size.x / sizeOrigin.x, 0.0f, 1.0f);

		m_aKeyConfig[i].s_p2DFront->SetColor(col);
	}

	// 切り替えの色変更
	{
		D3DXCOLOR col = FRONT_COL;

		if (m_SelectKey == INGAME::ACT_MAX)
		{
			col.a = m_Alpha;
		}

		m_checkconfig.s_p2DFront->SetColor(col);
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

	// 変更前を保存
	int oldkey = pKeyConfig->GetKey(m_SelectKey);

	// 変更
	pKeyConfig->Setting(m_SelectKey);

	if (m_aKeyConfig[m_SelectKey].s_pKeyDisp != nullptr)
	{
		m_aKeyConfig[m_SelectKey].s_pKeyDisp->SetType(pKeyConfig->GetKey(m_SelectKey));
	}

	// 変更後を保存
	int selectkey = pKeyConfig->GetKey(m_SelectKey);

	// 被りを確認する	
	for (int i = 0; i < INGAME::ACT_MAX; i++)
	{
		// 選択中
		if (m_SelectKey == i) { continue; }

		// 決定戻るの場合
		if (i <= INGAME::ACT_BACK) { continue; }

		// キー取得
		int key = pKeyConfig->GetKey(i);

		// 同じキーにしてしまっている場合
		if (selectkey == key)
		{
			// 変更前のと入れ替える
			pKeyConfig->Join(i, oldkey);
			m_aKeyConfig[i].s_pKeyDisp->SetType(pKeyConfig->GetKey(i));
		}
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

//==========================================================================
// 透明度調整
//==========================================================================
void CKeyConfigSetting::SetAlpha()
{
	if (m_Alpha >= 1.0f) { return; }

	m_fTime += ADD_ALPHA;
	m_Alpha = UtilFunc::Correction::EaseInOutExpo(0.0f, 1.0f, 0, 2.0f, m_fTime);
	m_Alpha = UtilFunc::Transformation::Clamp(m_Alpha, 0.0f, 1.0f);

	// 色を調整する

	// タイトル
	D3DXCOLOR col = m_pTitle2D->GetColor();
	col.a = m_Alpha;
	m_pTitle2D->SetColor(col);

	// 色を調整する
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// 文字
		col = m_aKeyConfig[i].s_p2D->GetColor();
		col.a = m_Alpha;
		m_aKeyConfig[i].s_p2D->SetColor(col);

		// キーコンフィグ
		col = m_aKeyConfig[i].s_pKeyDisp->GetColor();
		col.a = m_Alpha;
		m_aKeyConfig[i].s_pKeyDisp->SetColor(col);

		// 炭
		col = m_aKeyConfig[i].s_p2DFront->GetColor();
		col.a = m_Alpha;
		m_aKeyConfig[i].s_p2DFront->SetColor(col);
	}

	// 文字の生成
	col = m_checkconfig.s_p2D->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_p2D->SetColor(col);

	// キーコンフィグを生成
	col = m_checkconfig.s_pKeyDispOK->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_pKeyDispOK->SetColor(col);
	col = m_checkconfig.s_pKeyDispNO->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_pKeyDispNO->SetColor(col);

	// 暗くする用のポリゴン生成
	col = m_checkconfig.s_p2DFront->GetColor();
	col.a = m_Alpha;
	m_checkconfig.s_p2DFront->SetColor(col);
}