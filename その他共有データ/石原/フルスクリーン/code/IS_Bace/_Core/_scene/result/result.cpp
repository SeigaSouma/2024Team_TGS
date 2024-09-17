//======================================================
//
//リザルトシーン[result.cpp]
//Author:石原颯馬
//
//======================================================
#include "result.h"
#include "..\pause\pause.h"
#include "..\result\result.h"
#include "..\ranking\ranking.h"
#include "..\..\manager\manager.h"
#include "..\..\texture\texture.h"
#include "..\..\camera\camera.h"
#include "..\..\sound\sound.h"
#include "..\..\_object\object_empty\object.h"
#include "..\..\input\input.h"
#include "..\..\..\_Expansion\fade\fade.h"

//=================================
//コンストラクタ
//=================================
CResult::CResult()
{
	m_pPause = nullptr;
	m_pFade = nullptr;
}

//=================================
//デストラクタ
//=================================
CResult::~CResult()
{
}

//=================================
//初期化
//=================================
HRESULT CResult::Init(void)
{
	//カメラリセット
	CManager::GetInstance()->CManager::GetInstance()->GetCamera()->ResetPos();

	return S_OK;
}

//=================================
//終了
//=================================
void CResult::Uninit(void)
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
void CResult::Update(void)
{
	CManager* pIns = CManager::GetInstance();
	CInputKeyboard* pKeyboard = pIns->GetInputKeyboard();
	CInputGamePad* pResultpad = pIns->GetInputGamePad();
	bool bPush = false;

	if (pResultpad != nullptr && pResultpad->IsConnect() == true)
	{//ゲームパッド接続
		if (pResultpad->GetTrigger(XINPUT_GAMEPAD_A))
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
		m_pFade = CFade::Create(CScene::MODE_TUTORIAL);
	}
}

//=================================
//描画
//=================================
void CResult::Draw(void)
{
	//普段はすべてCObjectクラス継承してるものが動いているので自動描画
	//ポーズの中身もすべてCObjectクラス継承してるので自動描画
	//よってここですることはない
}