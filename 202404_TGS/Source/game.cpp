//=============================================================================
// 
//  ゲーム処理 [game.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "manager.h"
#include "game.h"
#include "game_tutorial.h"
#include "renderer.h"
#include "calculation.h"
#include "debugproc.h"
#include "fade.h"
#include "camera.h"
#include "pause.h"
#include "texture.h"

#include "input.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "timer.h"
#include "map.h"
#include "elevation.h"
#include "sound.h"
#include "edit_enemybase.h"
#include "stage.h"

#include "enemymanager.h"
#include "player.h"
#include "enemybase.h"
#include "limitarea.h"
#include "particle.h"
#include "myeffekseer.h"
#include "flower_bud.h"
#include "edit_map.h"
#include "objectLine.h"
#include "goalflag.h"

#include "sample_obj3D.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CGame* CGame::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CGame::CGame()
{
	// 値のクリア
	m_pScore = nullptr;				// スコアのオブジェクト
	m_pTimer = nullptr;				// タイマーのオブジェクト
	m_pLimitArea = nullptr;			// エリア制限のオブジェクト
	m_pEditEnemyBase = nullptr;		// 敵の拠点エディター
	m_pStage = nullptr;				// ステージのオブジェクト
	m_pGameManager = nullptr;		// ゲームマネージャのオブジェクト
	m_EditType = EDITTYPE_OFF;		// エディットの種類
	m_pEnemyBase = nullptr;			// 敵の拠点
	m_pEnemyManager = nullptr;		// 敵マネージャのオブジェクト
	m_bEdit = false;				// エディットの判定
	m_clear = false;				// クリア判定
	m_fMaxRokOnDistance = 0.0f;		// ロックオンの最大距離
	m_pEdit = nullptr;				// エディター
}

//==========================================================================
// デストラクタ
//==========================================================================
CGame::~CGame()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGame* CGame::Create(CScene::MODE mode)
{
	if (m_pThisPtr == nullptr)
	{// まだ生成していなかったら

		// インスタンス生成
		switch (mode)
		{
		case CScene::MODE_GAME:
			m_pThisPtr = DEBUG_NEW CGame;
			break;

		case CScene::MODE::MODE_GAMETUTORIAL:
			m_pThisPtr = DEBUG_NEW CGameTutorial;
			break;
		}
	}

	return m_pThisPtr;
}

//==========================================================================
// インスタンス取得
//==========================================================================
CGame* CGame::GetInstance()
{
	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGame::Init()
{
	// エディット判定OFF
	m_bEdit = false;

	// ロックオンの最大距離
	m_fMaxRokOnDistance = mylib_const::MAX_ROCKONDISTANCE_GAME;

	// プレイヤーの数設定
	CManager::GetInstance()->SetNumPlayer(1);

	// 初期化処理
	if (FAILED(CScene::Init()))
	{// 失敗した場合
		return E_FAIL;
	}

	//**********************************
	// ゲームマネージャ
	//**********************************
	m_pGameManager = CGameManager::Create(GetMode());

	// モード別初期化処理
	InitByMode();


	//**********************************
	// プレイヤー
	//**********************************
	// キャラ生成
	for (int nCntPlayer = 0; nCntPlayer < 1; nCntPlayer++)
	{
		if (CPlayer::Create(nCntPlayer) == nullptr)
		{
			return E_FAIL;
		}
	}
	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// リストループ
	while (playerList.ListLoop(&pPlayer))
	{
		pPlayer->SetPosition(MyLib::Vector3(0.0f, 10.0f, 0.0f));
		pPlayer->SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	}

	// ステージ
	m_pStage = CStage::Create("data\\TEXT\\stage\\info.txt");

	CManager::GetInstance()->GetCamera()->Reset(CScene::MODE_GAME);

	//CObjectLine* pppp = CObjectLine::Create(MyLib::Vector3(0.0f, 3000.0f, 0.0f), MyLib::Vector3(0.0f, 10.0f, 0.0f), mylib_const::DEFAULT_COLOR);

	// クリアの判定
	SetEnableClear(true);


	CSample_Obj3D::Create();

	m_pTimer = CTimer::Create();

	CGoalflagX::Create(MyLib::Vector3(300.0f,0.0f,0.0f));

	// 成功
	return S_OK;
}

//==========================================================================
// モード別初期化
//==========================================================================
void CGame::InitByMode()
{
#if 0
	//**********************************
	// 敵の拠点
	//**********************************
	m_pEnemyBase = CEnemyBase::Create("data\\TEXT\\enemydata\\base.txt");
	if (m_pEnemyBase == nullptr)
	{
		return;
	}

	//**********************************
	// 敵マネージャ
	//**********************************
	m_pEnemyManager = CEnemyManager::Create("data\\TEXT\\enemydata\\manager.txt");
	if (m_pEnemyManager == nullptr)
	{
		return;
	}
#endif
}

//==========================================================================
// 終了処理
//==========================================================================
void CGame::Uninit()
{
	m_pThisPtr = nullptr;

	// スコアの破棄
	if (m_pScore != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pScore->Uninit();

		// メモリの開放
		delete m_pScore;
		m_pScore = nullptr;
	}

	// タイマーの破棄
	if (m_pTimer != nullptr)
	{
		// 終了処理
		m_pTimer->Uninit();
		delete m_pTimer;
		m_pTimer = nullptr;
	}

	if (m_pEditEnemyBase != nullptr)
	{
		// 終了させる
		m_pEditEnemyBase->Uninit();
		delete m_pEditEnemyBase;
		m_pEditEnemyBase = nullptr;
	}

	// ステージの破棄
	if (m_pStage != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pStage->Uninit();
		delete m_pStage;
		m_pStage = nullptr;
	}

	if (m_pGameManager != nullptr)
	{
		// 終了処理
		m_pGameManager->Uninit();
		delete m_pGameManager;
		m_pGameManager = nullptr;
	}

	// 敵マネージャ
	if (m_pEnemyManager != nullptr)
	{
		m_pEnemyManager->Uninit();
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}

	// 敵の拠点
	if (m_pEnemyBase != nullptr)
	{
		m_pEnemyBase->Uninit();
		delete m_pEnemyBase;
		m_pEnemyBase = nullptr;
	}

	// 終了処理
	CScene::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CGame::Update()
{

	// ゲームマネージャ
	if (m_pGameManager != nullptr)
	{
		// 更新処理
		m_pGameManager->Update();
	}

	CManager::GetInstance()->GetDebugProc()->Print(
		"現在のモード：【ゲーム】\n"
		"切り替え：【 F 】\n\n");

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	if (m_pScore != nullptr &&
		CManager::GetInstance()->GetEdit() == nullptr &&
		m_pEnemyManager != nullptr)
	{
		// スコアの更新処理
		m_pScore->Update();
	}

#if _DEBUG

	// エディット切り替え処理
	ChangeEdit();

	if (pInputKeyboard->GetTrigger(DIK_F4))
	{// F4でエディット切り替え

		// 切り替え
		m_EditType = (EditType)(((int)m_EditType + 1) % (int)EDITTYPE_MAX);	// 追従の種類

		// リセット
		EditReset();

		// エディット判定ON
		m_bEdit = true;

		switch (m_EditType)
		{
		case CGame::EDITTYPE_OFF:	// 全部オフ
			// エディット判定OFF
			m_bEdit = false;
			break;

		case EDITTYPE_ENEMYBASE:
			if (m_pEditEnemyBase == nullptr)
			{// nullptrだったら

				// エディットの生成処理
				m_pEditEnemyBase = CEditEnemyBase::Create();
			}
			break;

		}
	}
#endif

	if (GetEnemyManager() != nullptr)
	{// 敵マネージャの更新処理
		GetEnemyManager()->Update();
	}

	if (m_pEditEnemyBase != nullptr)
	{// 敵の拠点エディターの更新処理
		m_pEditEnemyBase->Update();
	}

	// 敵の拠点
	if (m_pEnemyBase != nullptr)
	{
		m_pEnemyBase->Update();
	}

	// ステージの更新
	if (m_pStage != nullptr)
	{
		m_pStage->Update();
	}


	// タイマー更新
	if (m_pTimer != nullptr &&
		!CManager::GetInstance()->GetPause()->IsPause())
	{
		m_pTimer->Update();
	}

#if _DEBUG

	if (m_pEdit != nullptr) {
		m_pEdit->Update();
	}

	if (pInputKeyboard->GetTrigger(DIK_F))
	{
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);
	}

#endif


	if (pInputKeyboard->GetTrigger(DIK_I))
	{
		//**********************************
		// プレイヤー
		//**********************************
		// キャラ生成
		CPlayer* pPlayer = CPlayer::Create(0);
		pPlayer->SetPosition(MyLib::Vector3(UtilFunc::Transformation::Random(-10, 10) * 100.0f, 10.0f, UtilFunc::Transformation::Random(-10, 10) * 100.0f));

	}


	// シーンの更新
	CScene::Update();
}

//==========================================================================
// エディット切り替え処理
//==========================================================================
void CGame::ChangeEdit()
{
	// エディットメニュー
	ImGui::Begin("Edit"/*, NULL, ImGuiWindowFlags_MenuBar*/);
	{
		// テキスト
		static const char* items[] = { "OFF", "EnemyBase", "Map" };
		int selectedItem = m_EditType;

		// [グループ]エディット切り替え
		if (ImGui::CollapsingHeader("Change Edit Mode"))
		{
			// [ラジオボタン]モーション切り替え
			for (int i = 0; i < EditType::EDITTYPE_MAX; i++)
			{
				if (ImGui::RadioButton(items[i], &selectedItem, i))
				{
					// エディット終了
					EditReset();
					m_EditType = static_cast<EditType>(selectedItem);

					// 生成
					m_pEdit = CEdit::Create(m_EditType);
				}
			}
		}




		//// コンボボックス
		//static const char* items[] = { "OFF", "EnemyBase", "Map" };
		//static int selectedItem = 0;

		//// コンボボックス
		//if (ImGui::Combo("Change Edit Mode", &selectedItem, items, IM_ARRAYSIZE(items)))
		//{
		//	// 選択された項目が変更されたときの処理
		//	switch (m_EditType)
		//	{
		//	case CGame::EDITTYPE_OFF:
		//		break;
		//	case CGame::EDITTYPE_ENEMYBASE:
		//		break;
		//	case CGame::EDITTYPE_MAX:
		//		break;
		//	default:
		//		break;
		//	}
		//}



	}
	ImGui::End();
}

//==========================================================================
// 描画処理
//==========================================================================
void CGame::Draw()
{

}

//==========================================================================
// スコアの取得
//==========================================================================
CScore *CGame::GetScore()
{
	return m_pScore;
}

//==========================================================================
// ステージの取得
//==========================================================================
CStage *CGame::GetStage()
{
	return m_pStage;
}

//==========================================================================
// ゲームマネージャの取得
//==========================================================================
CGameManager *CGame::GetGameManager()
{
	return m_pGameManager;
}

//==========================================================================
// 敵マネージャの取得
//==========================================================================
CEnemyManager *CGame::GetEnemyManager()
{
	return m_pEnemyManager;
}

//==========================================================================
// 敵の拠点
//==========================================================================
CEnemyBase *CGame::GetEnemyBase()
{
	return m_pEnemyBase;
}

//==========================================================================
// リセット処理
//==========================================================================
void CGame::ResetBeforeBoss()
{
#if 0
	// ステージの破棄
	if (m_pStage != nullptr)
	{// メモリの確保が出来ていたら

		// 終了処理
		m_pStage->Release();
		delete m_pStage;
		m_pStage = nullptr;
	}

	// 敵の拠点
	if (m_pEnemyBase != nullptr)
	{
		m_pEnemyBase->Uninit();
		delete m_pEnemyBase;
		m_pEnemyBase = nullptr;
	}

	// エリア制限
	if (m_pLimitArea != nullptr)
	{
		m_pLimitArea->Uninit();
		m_pLimitArea = nullptr;
	}

	// ステージ
	m_pStage = CStage::Create("data\\TEXT\\stage\\boss_info.txt");

	//**********************************
	// 敵の拠点
	//**********************************
	m_pEnemyBase = CEnemyBase::Create("data\\TEXT\\enemydata\\base_boss.txt");
	if (m_pEnemyBase == nullptr)
	{// nullptrだったら
		return;
	}
#endif
	// ロックオンの最大距離
	m_fMaxRokOnDistance = mylib_const::MAX_ROCKONDISTANCE_BOSS;
}

//==========================================================================
// エディターリセット処理
//==========================================================================
void CGame::EditReset()
{
	
	if (m_pEditEnemyBase != nullptr)
	{
		// 終了させる
		m_pEditEnemyBase->Release();
		m_pEditEnemyBase->Uninit();
		delete m_pEditEnemyBase;
		m_pEditEnemyBase = nullptr;
	}

	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		m_pEdit = nullptr;
	}
}
