//=============================================================================
// 
//  タイトルのオプション選択肢処理 [title_optionselect.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "title_optionselect.h"
#include "title_select.h"
#include "title_pressenter.h"
#include "title.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "sound.h"
#include "object2D.h"
#include "scroll.h"
#include "keyconfig_setting.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_ROLL = "data\\TEXTURE\\scroll\\scroll_roll.png";		// ロール部分のテクスチャ
	const std::string TEXTURE_PAPER = "data\\TEXTURE\\scroll\\scroll_paper.png";	// 紙部分のテクスチャ
	const std::string TEXTURE_EDGE = "data\\TEXTURE\\scroll\\scroll_edge.png";		// 端部分のテクスチャ
	const float SIZE_Y = 80.0f;	// 基準サイズY
}

namespace StateTime	// 状態別時間
{
	const float FADE = 0.3f;	// フェード
}


//==========================================================================
// 関数ポインタ
//==========================================================================
CTitle_OptionSelect::STATE_FUNC CTitle_OptionSelect::m_StateFunc[] =
{
	&CTitle_OptionSelect::StateNone,		// なし
	&CTitle_OptionSelect::StateScrollWait,	// 巻き物待ち
	&CTitle_OptionSelect::StateScrollWait_Close,	// 巻き物待ち
	&CTitle_OptionSelect::StateSelect,		// 選択
	&CTitle_OptionSelect::StateEdit,		// 編集
	&CTitle_OptionSelect::StateFadeIn,		// フェードイン
	&CTitle_OptionSelect::StateFadeOut,		// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTitle_OptionSelect::CTitle_OptionSelect(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_state = STATE::STATE_NONE;			// 状態
	m_fStateTimer = 0.0f;					// 状態タイマー
	m_select = Select::SELECT_KEYKONFIG;	// 選択肢
	
	memset(m_pSelect, 0, sizeof(m_pSelect));	// 選択肢のオブジェクト
	m_pScroll = nullptr;					// 巻き物

}

//==========================================================================
// デストラクタ
//==========================================================================
CTitle_OptionSelect::~CTitle_OptionSelect()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTitle_OptionSelect* CTitle_OptionSelect::Create()
{
	// メモリの確保
	CTitle_OptionSelect* pObj = DEBUG_NEW CTitle_OptionSelect();

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
HRESULT CTitle_OptionSelect::Init()
{
	
	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	// 位置設定
	SetPosition(MyLib::Vector3(200.0f, 200.0f, 0.0f));

	
	// 選択肢生成
	CreateSelect();

	// 状態
	SetState(STATE::STATE_NONE);

	return S_OK;
}

//==========================================================================
// 選択肢生成
//==========================================================================
void CTitle_OptionSelect::CreateSelect()
{
	MyLib::Vector3 pos = GetPosition();

	// テクスチャ取得
	CTexture* pTexture = CTexture::GetInstance();

	for (int i = 0; i < Select::SELECT_MAX; i++)
	{
		// 生成
		m_pSelect[i] = CObject2D::Create();
		if (m_pSelect[i] == nullptr) continue;

		CObject2D* pObj2D = m_pSelect[i];

		// 種類の設定
		pObj2D->SetType(CObject::TYPE::TYPE_OBJECT2D);

		// テクスチャ設定
		int texID = pTexture->Regist(TEXTURE_ROLL);
		pObj2D->BindTexture(texID);

		// サイズ設定
		D3DXVECTOR2 size = pTexture->GetImageSize(texID);

		// 縦幅を元にサイズ設定
		size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_Y);
		pObj2D->SetSize(size);
		pObj2D->SetSizeOrigin(size);

		// 位置設定
		pObj2D->SetPosition(pos + MyLib::Vector3(0.0f, (SIZE_Y * 2.0f) * i, 0.0f));
	}
}

//==========================================================================
// 終了処理
//==========================================================================
void CTitle_OptionSelect::Uninit()
{
	

	// 終了処理
	CObject::Release();
}

//==========================================================================
// 削除
//==========================================================================
void CTitle_OptionSelect::Kill()
{
	for (const auto& select : m_pSelect)
	{
		select->Uninit();
	}

	// 終了処理
	CObject::Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTitle_OptionSelect::Update()
{

	// 状態更新
	UpdateState();

	if (IsDeath()) return;
}

//==========================================================================
// 状態更新
//==========================================================================
void CTitle_OptionSelect::UpdateState()
{
	// 状態タイマー
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// なにもなし
//==========================================================================
void CTitle_OptionSelect::StateNone()
{
	
}

//==========================================================================
// 巻き物待ち
//==========================================================================
void CTitle_OptionSelect::StateScrollWait()
{
	if (m_pScroll == nullptr)
	{
		m_pScroll = CScroll::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), 0.1f, 400.0f, 1200.0f, false, true, 2);
	}

	// スクロール終了
	if (m_pScroll->GetState() == CScroll::STATE::STATE_WAIT)
	{
		SetState(STATE::STATE_FADEIN);
	}
}

//==========================================================================
// 巻き物待ち(閉じ)
//==========================================================================
void CTitle_OptionSelect::StateScrollWait_Close()
{
	if (m_pScroll == nullptr ||
		(m_pScroll != nullptr && m_pScroll->GetState() != CScroll::STATE::STATE_FADEOUT)) return;


	// 巻き物
	if (m_pScroll != nullptr)
	{
		m_pScroll = nullptr;
	}


	// タイトル取得
	CTitle* pTitle = CTitle::GetInstance();

	// シーン設定
	pTitle->SetSceneType(CTitle::SCENETYPE::SCENETYPE_NONE);

	// 選択肢の状態設定
	CTitle_Select* pSelect = pTitle->GetTitlePressEnter()->GetSelect();
	pSelect->SetState(CTitle_Select::STATE::STATE_FADEIN);

}

//==========================================================================
// 選択
//==========================================================================
void CTitle_OptionSelect::StateSelect()
{
	// 入力情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 上
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y > 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0) ||
		pKey->GetTrigger(DIK_W))
	{
		// パターンNo.を更新
		m_select = static_cast<Select>((m_select + (Select::SELECT_MAX - 1)) % Select::SELECT_MAX);
	}

	// 下
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_Y) && pPad->GetStickMoveL(0).y < 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0) ||
		pKey->GetTrigger(DIK_S))
	{
		// パターンNo.を更新
		m_select = static_cast<Select>(((int)m_select + 1) % Select::SELECT_MAX);
	}


	// メニューへ
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x > 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_RIGHT, 0) ||
		pKey->GetTrigger(DIK_D))
	{
		m_select = Select::SELECT_MAX;

		// 編集状態
		SetState(STATE::STATE_EDIT);
	}

	// 選択肢に戻る設定
	SetBackSelect();
}

//==========================================================================
// 編集
//==========================================================================
void CTitle_OptionSelect::StateEdit()
{
	// 入力情報取得
	CInputKeyboard* pKey = CInputKeyboard::GetInstance();
	CInputGamepad* pPad = CInputGamepad::GetInstance();

	// 選択肢へ
	if ((pPad->GetLStickTrigger(CInputGamepad::STICK::STICK_X) && pPad->GetStickMoveL(0).x < 0) ||
		pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_LEFT, 0) ||
		pKey->GetTrigger(DIK_A))
	{
		// 前回の選択肢へ戻す
		m_select = m_Oldselect;

		// 選択状態
		SetState(STATE::STATE_SELECT);
	}
}

//==========================================================================
// フェードイン
//==========================================================================
void CTitle_OptionSelect::StateFadeIn()
{
	// 不透明度更新
	float alpha = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// 時間経過
	if (m_fStateTimer >= StateTime::FADE)
	{
		SetState(STATE::STATE_SELECT);
	}
}

//==========================================================================
// フェードアウト
//==========================================================================
void CTitle_OptionSelect::StateFadeOut()
{
	// 不透明度更新
	float alpha = UtilFunc::Correction::EasingLinear(1.0f, 0.0f, 0.0f, StateTime::FADE, m_fStateTimer);

	for (const auto& select : m_pSelect)
	{
		select->SetAlpha(alpha);
	}

	// 時間経過
	if (m_fStateTimer >= StateTime::FADE)
	{
		SetState(STATE::STATE_SCROLLWAIT_CLOSE);
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTitle_OptionSelect::Draw()
{
	
}

//==========================================================================
// 選択肢に戻る設定
//==========================================================================
void CTitle_OptionSelect::SetBackSelect()
{
	// キーコンフィグ取得
	CKeyConfig* pKeyConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKeyConfigKey = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (m_pScroll == nullptr ||
		(m_pScroll != nullptr && m_pScroll->GetState() != CScroll::STATE::STATE_WAIT)) return;

	// 選択肢画面へ戻る
	if (pKeyConfigPad->GetTrigger(INGAME::ACT_BACK) ||
		pKeyConfigKey->GetTrigger(INGAME::ACT_BACK))
	{
		// 巻き物閉じる
		if (m_pScroll != nullptr)
		{
			m_pScroll->SetState(CScroll::STATE::STATE_CLOSE);
		}


		// タイトル取得
		CTitle* pTitle = CTitle::GetInstance();
		pTitle->SetSceneType(CTitle::SCENETYPE::SCENETYPE_NONE);

		// キーコンフィグ取得
		pTitle->GetSetting()->Uninit();
		pTitle->SetSetting(nullptr);


		// フェードアウト
		SetState(STATE::STATE_FADEOUT);
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CTitle_OptionSelect::SetState(const STATE& state)
{
	m_state = state;
	m_fStateTimer = 0.0f;
}
