//======================================================
//
//タイトルシーン[title.cpp]
//Author:石原颯馬
//
//======================================================
#include "title.h"
#include "..\..\texture\texture.h"
#include "..\..\..\_Expansion\fade\fade.h"	//ここなんとかして
#include "..\..\input\input.h"
#include "..\..\camera\camera.h"
#include "..\..\sound\sound.h"
#include "..\..\_object\object_2d\object2D.h"

//静的メンバ変数

//=================================
//コンストラクタ
//=================================
CTitle::CTitle()
{
	m_pFade = nullptr;
	m_pStart = nullptr;
}

//=================================
//デストラクタ
//=================================
CTitle::~CTitle()
{
}

//=================================
//初期化
//=================================
HRESULT CTitle::Init(void)
{
	return S_OK;
}

//=================================
//終了
//=================================
void CTitle::Uninit(void)
{
	//音停止
	CManager::GetInstance()->GetSound()->Stop();

	//オブジェ全破棄
	for (int cnt = 0; cnt < CObject::PRIORITY_FADE; cnt++)
	{
		CObject::ReleaseAll(cnt);
	}
}

//=================================
//更新
//=================================
void CTitle::Update(void)
{
	CManager* pIns = CManager::GetInstance();
	CInputKeyboard* pKeyboard = pIns->GetInputKeyboard();
	CInputGamePad* pGamepad = pIns->GetInputGamePad();
	bool bPush = false;

	if (pGamepad != nullptr && pGamepad->IsConnect() == true)
	{//ゲームパッド接続
		m_pStart->BindTexture(CTexture::PRELOAD_02_STARTGP);

		if (pGamepad->GetTrigger(XINPUT_GAMEPAD_A))
		{
			bPush = true;
		}
	}
	else
	{//未接続
		m_pStart->BindTexture(CTexture::PRELOAD_01_STARTKB);

		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			bPush = true;
		}
	}

	if (m_pFade == nullptr && bPush == true)
	{//スペース押された
		m_pFade = CFade::Create(CScene::MODE_TUTORIAL);
	}
}

//=================================
//描画
//=================================
void CTitle::Draw(void)
{
}
