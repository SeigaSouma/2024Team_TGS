//======================================================
//
//ポーズ画面[game.cpp]
//Author:石原颯馬
//
//======================================================
#include "pause.h"
#include "..\..\manager\manager.h"
#include "..\..\texture\texture.h"
#include "..\..\input\input.h"
#include "..\..\..\_Expansion\bg\bg.h"
#include "..\..\_object\object_2d\object2D.h"
#include "..\..\..\_Expansion\fade\fade.h"
#include "..\..\sound\sound.h"
#include "..\..\utility\Utility.h"

//=================================
//コンストラクタ
//=================================
CPause::CPause()
{
	m_bScreenShot = false;
}

//=================================
//デストラクタ
//=================================
CPause::~CPause()
{
}

//=================================
//初期化
//=================================
HRESULT CPause::Init(void)
{
	return S_OK;
}

//=================================
//終了
//=================================
void CPause::Uninit(void)
{
	
}

//=================================
//更新
//=================================
void CPause::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad* pGamepad = CManager::GetInstance()->GetInputGamePad();

	if (pGamepad != nullptr && pGamepad->IsConnect() == true)
	{//ゲームパッド
		//（十字）
		if (pGamepad->GetTrigger(XINPUT_GAMEPAD_DPAD_UP) == true)
		{//上（-）
			m_mode = (MODE)(((m_mode + MODE_MAX) - 1) % MODE_MAX);
		}
		else if (pGamepad->GetTrigger(XINPUT_GAMEPAD_DPAD_DOWN) == true)
		{//下（+）
			m_mode = (MODE)(((m_mode + MODE_MAX) + 1) % MODE_MAX);
		}
		else if (m_mode != MODE_SCREENSHOT && pGamepad->GetTrigger(XINPUT_GAMEPAD_DPAD_RIGHT) == true)
		{//横（スクショモード以外の時スクショモード）
			m_mode = MODE_SCREENSHOT;
		}
		else if (m_mode == MODE_SCREENSHOT && pGamepad->GetTrigger(XINPUT_GAMEPAD_DPAD_LEFT) == true)
		{//左（スクショモード時ゲームに戻る）
			m_mode = MODE_RESUME;
		}
	}
	else
	{//キーボード
		//移動
		if (pKeyboard->GetTrigger(DIK_W) == true)
		{//上（-）
			m_mode = (MODE)(((m_mode + MODE_MAX) - 1) % MODE_MAX);
		}
		else if (pKeyboard->GetTrigger(DIK_S) == true)
		{//下（+）
			m_mode = (MODE)(((m_mode + MODE_MAX) + 1) % MODE_MAX);
		}
		else if (m_mode != MODE_SCREENSHOT && pKeyboard->GetTrigger(DIK_D) == true)
		{//右（スクショモード以外の時スクショモード）
			m_mode = MODE_SCREENSHOT;
		}
		else if (m_mode == MODE_SCREENSHOT && pKeyboard->GetTrigger(DIK_A) == true)
		{//左（スクショモード時ゲームに戻る）
			m_mode = MODE_RESUME;
		}
	}

	if (m_bScreenShot == false)
	{//通常
		if (pGamepad != nullptr && pGamepad->GetTrigger(XINPUT_GAMEPAD_A) == true)
		{
			Select();
		}
		else if (pKeyboard != nullptr && pKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			Select();
		}
	}
	else
	{//スクショモード
		//スクショモード解除
		if (pGamepad != nullptr && pGamepad->GetTrigger(XINPUT_GAMEPAD_A) == true)
		{
			m_bScreenShot = false;
		}
		else if (pKeyboard != nullptr && pKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			m_bScreenShot = false;
		}
	}
}

//=================================
//描画
//=================================
void CPause::Draw(void)
{
	//CObjectを親にしているから勝手に描画される
}

//=================================
//選択して実行
//=================================
void CPause::Select(void)
{
	switch (m_mode)
	{
	case CPause::MODE_RESUME:
		CManager::GetInstance()->SetEnableUpdate(true);
		break;
	case CPause::MODE_RESTART:
		CManager::GetInstance()->SetEnableUpdate(true);
		CFade::Create(CScene::MODE_GAME);
		break;
	case CPause::MODE_EXIT:
		CManager::GetInstance()->SetEnableUpdate(true);
		CFade::Create(CScene::MODE_TITLE);
		break;
	case CPause::MODE_SCREENSHOT:
		m_bScreenShot = true;
		break;
	default:
		break;
	}
}
