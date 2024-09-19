//=============================================================================
// 
//  ランキング処理 [ranking.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "rankingscore.h"
#include "keyconfig.h"

#include "rankingItem.h"
#include "rankingItem_top3.h"
//=============================================================================
// 定数定義
//=============================================================================
namespace
{
	//ランキングのコンフィグ
	const float SCROLL_SPEED = -10.0f;
	const float SCROLL_STOP_POS_Y = 600.0f;
}
//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CRankingScore *CRanking::m_pRankingScore = nullptr;	// ランキングスコアのオブジェクト
bool CRanking::m_bAllArrival = false;		// 全て到着した判定

//==========================================================================
// コンストラクタ
//==========================================================================
CRanking::CRanking()
{
	m_pRankingScore = nullptr;	// ランキングスコアのオブジェクト
	m_bAllArrival = false;	// 全て到着した判定
	
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_pRanking[nCnt] = nullptr;
	}
}

//==========================================================================
// デストラクタ
//==========================================================================
CRanking::~CRanking()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRanking::Init()
{

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	// ランキングのスコア生成
	/*m_pRankingScore = CRankingScore::Create();*/

	//仮日付データ
	int nDate[3] = { 2030,1,1 };

	//TOP3のアイテム生成（順位、時間、総評、日付、基準位置、拡大率、ランキング更新）
	m_pRanking[0] = CRankingItem_top3::Create(1, 0,1,99, 2, nDate, MyLib::Vector3(250.0f, 250.0f, 0.0f), 0.9f, true);
	m_pRanking[1] = CRankingItem_top3::Create(2, 1,12,33, 2, nDate, MyLib::Vector3(250.0f, 390.0f, 0.0f), 0.9f, false);
	m_pRanking[2] = CRankingItem_top3::Create(3, 2,22,22, 3, nDate, MyLib::Vector3(250.0f, 530.0f, 0.0f), 0.9f, false);

	//下位のアイテム生成（順位、時間、総評、基準位置、拡大率、ランキング更新）
	m_pRanking[3] = CRankingItem::Create(4, 4,44,44, 3, MyLib::Vector3(250.0f, 700.0f, 0.0f), 0.85f, true);
	m_pRanking[4] = CRankingItem::Create(5, 55,55,55, 3, MyLib::Vector3(250.0f, 800.0f, 0.0f), 0.85f, false);
	m_pRanking[5] = CRankingItem::Create(6, 66,66,66, 3, MyLib::Vector3(250.0f, 900.0f, 0.0f), 0.85f, false);
	m_pRanking[6] = CRankingItem::Create(7, 77,77,77, 3, MyLib::Vector3(250.0f, 1000.0f, 0.0f), 0.85f, false);
	m_pRanking[7] = CRankingItem::Create(8, 88,88,88, 3, MyLib::Vector3(250.0f, 1100.0f, 0.0f), 0.85f, false);
	m_pRanking[8] = CRankingItem::Create(9, 99,99,99, 3, MyLib::Vector3(250.0f, 1200.0f, 0.0f), 0.85f, false);
	m_pRanking[9] = CRankingItem::Create(10, 10,10,10, 3, MyLib::Vector3(250.0f, 1300.0f, 0.0f), 0.85f, false);
	

	// 成功
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRanking::Uninit()
{
	// ランキングスコアのオブジェクト
	m_pRankingScore = nullptr;

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CRanking::Update()
{
	CManager::GetInstance()->GetDebugProc()->Print(
		"現在のモード：【ランキング】\n"
		"切り替え：【 F 】\n\n");

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// キーコンフィグ
	CKeyConfigManager* pConfigMgr = CKeyConfigManager::GetInstance();
	CKeyConfig* pPad = pConfigMgr->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CKeyConfig* pKey = pConfigMgr->GetConfig(CKeyConfigManager::CONTROL_INKEY);

	if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) || pPad->GetTrigger(OUTGAME::ACT_OK, 0))
	{
		//スクロール開始処理
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			m_pRanking[nCnt]->SetMove(MyLib::Vector3(0.0f, SCROLL_SPEED, 0.0f));
		}
	}

	//スクロール停止判定
	MyLib::Vector3 latestpos = m_pRanking[9]->GetPos();
	if (latestpos.y <= SCROLL_STOP_POS_Y)
	{
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			m_pRanking[nCnt]->SetMove(MyLib::Vector3(0.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) || pPad->GetTrigger(OUTGAME::ACT_OK, 0))
		{
			// モード設定
			CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_TITLE);
		}
	}

	//スクロール更新処理
	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_pRanking[nCnt]->Update();
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CRanking::Draw()
{

}

//==========================================================================
// 到着設定ON
//==========================================================================
void CRanking::SetEnableArrival()
{
	m_bAllArrival = true;
}
