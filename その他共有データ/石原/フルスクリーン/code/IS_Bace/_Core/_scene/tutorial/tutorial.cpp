//======================================================
//
//チュートリアルシーン[game.cpp]
//Author:石原颯馬
//
//======================================================
#include "tutorial.h"
#include "..\..\manager\manager.h"
#include "..\..\texture\texture.h"
#include "..\..\input\input.h"
#include "..\..\camera\camera.h"
#include "..\..\sound\sound.h"

//UI系
#include "..\..\..\_Expansion\fade\fade.h"

//静的メンバ変数
const int CTutorial::TUTORIAL_MOVE = 4;		//移動説明の番号
const int CTutorial::TUTORIAL_CHANGE = 5;	//切替説明の番号

//=================================
//コンストラクタ
//=================================
CTutorial::CTutorial()
{
	m_pPlayer = nullptr;
	m_pSwitchManager = nullptr;
	m_pFade = nullptr;
	m_pMoveTutorial = nullptr;
	m_pChangeTutorial = nullptr;
}

//=================================
//デストラクタ
//=================================
CTutorial::~CTutorial()
{
}

//=================================
//初期化
//=================================
HRESULT CTutorial::Init(void)
{
	//カメラリセット
	CManager::GetInstance()->CManager::GetInstance()->GetCamera()->ResetPos();

	return S_OK;
}

//=================================
//終了
//=================================
void CTutorial::Uninit(void)
{
	//オブジェ全破棄
	for (int cnt = 0; cnt < CObject::PRIORITY_FADE; cnt++)
	{
		CObject::ReleaseAll(cnt);
	}

	//音止める
	CManager::GetInstance()->GetSound()->Stop();
}

//=================================
//更新
//=================================
void CTutorial::Update(void)
{
	CManager* pIns = CManager::GetInstance();
	CInputKeyboard* pKeyboard = pIns->GetInputKeyboard();
	CInputGamePad* pGamepad = pIns->GetInputGamePad();
	bool bPush = false;

	if (pGamepad != nullptr && pGamepad->IsConnect() == true)
	{//ゲームパッド接続
		if (pGamepad->GetTrigger(XINPUT_GAMEPAD_A))
		{
			bPush = true;
		}
	}
	else
	{//未接続
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			bPush = true;
		}
	}

	if (m_pFade == nullptr && bPush == true)
	{//スペース押された
		m_pFade = CFade::Create(CScene::MODE_GAME);
	}
}

//=================================
//描画
//=================================
void CTutorial::Draw(void)
{
	//普段はすべてCObjectクラス継承してるものが動いているので自動描画
	//ポーズの中身もすべてCObjectクラス継承してるので自動描画
	//よってここですることはない
}