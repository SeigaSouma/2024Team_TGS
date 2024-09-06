//=============================================================================
// 
//  リザルト処理 [result.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "result.h"
#include "resultManager.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "player.h"
#include "sound.h"
#include "game.h"

#include "timer.h"
#include "clearrank.h"
#include "toatalrank.h"
#include "scroll.h"

//=============================================================================
// 定数定義
//=============================================================================
namespace
{
	const D3DXCOLOR WIN_COLOR = D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
	const D3DXCOLOR LOSE_COLOR = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	const float START_LENGTH = 300.0f; // 初期距離
	const float END_LENGTH = 2000.0f; // 目標距離

	//モデルの配置位置
	const MyLib::Vector3 CENTER = MyLib::Vector3(0.0f, 0.0f, -2000.0f);
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CResultScore *CResult::m_pResultScore = nullptr;	// リザルトスクリーンのオブジェクト
bool CResult::m_bAllArrival = false;		// 全て到着した判定

//==========================================================================
// コンストラクタ
//==========================================================================
CResult::CResult() : m_clear(false)
{
	// 値のクリア
	m_pTimer = nullptr;	// タイマーのオブジェクト
	m_bAllArrival = false;	// 全て到着した判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CResult::~CResult()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CResult::Init()
{
	//プレイヤー数をリセット
	CManager::GetInstance()->SetNumPlayer(0);

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RESULT);

	// リザルト画面

	// 巻き物
	CScroll::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f), 1.0f, 350.0f, 1000.0f, true, 0);

	// リザルトマネージャ
	CResultManager* pResultManager = CResultManager::GetInstance();

	// タイマー
	m_pTimer = CTimer::Create(CTimer::Type::TYPE_RESULT);
	m_pTimer->SetTime(pResultManager->GetClearTime());

	// クリアランク
	CClearRank::Create(pResultManager->GetJudgeRank());

	// トータルランク
	CToatalRank::Create(pResultManager->GetJudgeRank(), pResultManager->GetClearTime());


	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CResult::Uninit()
{
	m_pResultScore = nullptr;

	// タイマーの破棄
	if (m_pTimer != nullptr)
	{
		// 終了処理
		m_pTimer->Uninit();
		m_pTimer = nullptr;
	}

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CResult::Update()
{
	

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// 画面遷移
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, 0))
	{
		if (m_bAllArrival)
		{
			// モード設定
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		}

		if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE)
		{
			// 全ての到着処理
			if (m_pResultScore != nullptr)
			{
			}
			m_bAllArrival = true;
		}
	}

	// タイマー更新
	m_pTimer->Update();
}

//==========================================================================
// 描画処理
//==========================================================================
void CResult::Draw()
{

}

//==========================================================================
// リザルトスクリーンの情報取得
//==========================================================================
CResultScore *CResult::GetResultScore()
{
	return m_pResultScore;
}

//==========================================================================
// 到着設定ON
//==========================================================================
void CResult::SetEnableArrival()
{
	m_bAllArrival = true;
}
