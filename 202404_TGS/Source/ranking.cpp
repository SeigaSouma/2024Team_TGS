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
	const int NUM_RANK = 10;
	const std::string FILE_BIN = "data\\TEXT\\ranking\\ranking.bin";
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
	m_pRankData = nullptr;
	
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

	// ランキングデータ格納用構造体を作る
	m_pRankData = new SRankdata[NUM_RANK];

	// ファイル読み込み
	Load();

	// ファイル保存
	Save();

	//仮日付データ

	for (int i = 0; i < NUM_RANK; i++)
	{
		int nDate[3] = { m_pRankData[i].year,m_pRankData[i].month,m_pRankData[i].day };
		int minutes = m_pRankData[i].minutes;
		int seconds = m_pRankData[i].seconds;
		int milliseconds = m_pRankData[i].milliSeconds;
		int allrank = m_pRankData[i].allrank;
		bool rankin = m_pRankData[i].rankin;

		if (i < 3)
		{
			//TOP3のアイテム生成（順位、時間、総評、日付、基準位置、拡大率、ランキング更新）
			m_pRanking[i] = CRankingItem_top3::Create(i + 1, minutes, seconds, milliseconds, allrank, nDate, MyLib::Vector3(250.0f, 250.0f + i * 140.0f, 0.0f), 0.9f, rankin);
		}
		else
		{
			int j = i - 3;
			//下位のアイテム生成（順位、時間、総評、基準位置、拡大率、ランキング更新）
			m_pRanking[i] = CRankingItem::Create(i + 1, minutes, seconds, milliseconds, allrank, MyLib::Vector3(250.0f, 700.0f + j * 100.0f, 0.0f), 0.85f, rankin);
		}
	}
	
	CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_RANKING);

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

	// ランキング情報の削除
	if (m_pRankData != nullptr)
	{
		delete[] m_pRankData;
		m_pRankData = nullptr;
	}

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

	if (pKey->GetTrigger(OUTGAME::ACT_OK, 0) || pPad->GetTrigger(OUTGAME::ACT_OK, 0))
	{
		//スクロール開始処理
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			m_pRanking[nCnt]->SetMove(MyLib::Vector3(0.0f, SCROLL_SPEED, 0.0f));
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

//==========================================================================
// ファイル読み込み
//==========================================================================
void CRanking::Load()
{
	// ファイルを開く
	std::ifstream File(FILE_BIN, std::ios::binary);
	if (!File.is_open()) {

		if (m_pRankData == nullptr)
		return;

		// 時刻を取得
		time_t Time = time(NULL);

		// 現在の時刻を現地時間に変換
		std::tm* now = std::localtime(&Time);

		// 年、月、日、時、分、秒をそれぞれintに変換
		int year = now->tm_year + 1900;  // 年は1900年からの経過年数
		int month = now->tm_mon + 1;     // 月は0から始まるので+1
		int day = now->tm_mday;          // 日

		// 初期値を入れる
		for (int i = 0; i < NUM_RANK; i++)
		{
			m_pRankData[i].year = year;
			m_pRankData[i].month = month;
			m_pRankData[i].day = day;
			m_pRankData[i].minutes = 8 + i * 1;
			m_pRankData[i].seconds = i % 6 * 10;
			m_pRankData[i].milliSeconds = 0;
			m_pRankData[i].allrank = 3;
			m_pRankData[i].rankin = false;
		}

		return;
	}


	// データ読み込み
	File.read(reinterpret_cast<char*>(m_pRankData), sizeof(SRankdata) * NUM_RANK);

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// 保存
//==========================================================================
void CRanking::Save()
{
	// ファイルを開く
	std::ofstream File(FILE_BIN, std::ios::binary);
	if (!File.is_open()) {

		return;
	}

	// データ保存
	File.write(reinterpret_cast<const char*>(m_pRankData), sizeof(SRankdata) * NUM_RANK);

	// ファイルを閉じる
	File.close();
}