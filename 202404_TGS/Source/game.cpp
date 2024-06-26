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
#include "score.h"
#include "timer.h"
#include "map.h"
#include "elevation.h"
#include "sound.h"
#include "stage.h"

#include "player.h"
#include "limitarea.h"
#include "particle.h"
#include "myeffekseer.h"
#include "flower_bud.h"
#include "edit_map.h"
#include "objectLine.h"
#include "goalflag.h"
#include "checkpoint.h"
#include "map_obstacleManager.h"
#include "baggageManager.h"
#include "judgezoneManager.h"
#include "stencilshadow.h"

#include "sample_obj3D.h"
#include "course.h"
#include "waterfield.h"
#include "stonewall.h"
#include "stonewall_front.h"

#include "2D_Effect.h"
#include "waterripple.h"
#include "meshbubble.h"
#include "waterstone.h"
#include "map_ui.h"
#include "waterstoneManager.h"

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
	m_pStage = nullptr;				// ステージのオブジェクト
	m_pGameManager = nullptr;		// ゲームマネージャのオブジェクト
	m_EditType = EDITTYPE_OFF;		// エディットの種類
	m_bEdit = false;				// エディットの判定
	m_clear = false;				// クリア判定
	m_fMaxRokOnDistance = 0.0f;		// ロックオンの最大距離
	m_pEdit = nullptr;				// エディター
	m_pObstacleManager = nullptr;	// 障害物マネージャ
	m_pBaggageManager = nullptr;	// 荷物マネージャ
	m_pCourse = nullptr;			// コースのオブジェクト
	m_pJudgeZoneManager = nullptr;	// 判定ゾーンマネージャ
	m_pMapUI = nullptr;				// マップUI
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

	// 荷物マネージャ
	m_pBaggageManager = CBaggageManager::Create();

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

	//=============================
	// ステージ
	//=============================
	m_pStage = CStage::Create("data\\TEXT\\stage\\info.txt");

	CManager::GetInstance()->GetCamera()->Reset(CScene::MODE_GAME);

	// クリアの判定
	SetEnableClear(true);

	m_pTimer = CTimer::Create();

	CGoalflagX::Create(MyLib::Vector3(23000.0f,0.0f,0.0f));

	//=============================
	// 障害物マネージャ
	//=============================
	m_pObstacleManager = CMap_ObstacleManager::Create();


	//=============================
	// コース作成
	//=============================
	m_pCourse = CCourse::Create("data\\TEXT\\map\\course.bin");

	//=============================
	// 石垣(奥)
	//=============================
	CStoneWall* pStoneWall = CStoneWall::Create();

	// 基点地点設定
	pStoneWall->SetVecPosition(m_pCourse->GetVecPosition());
	pStoneWall->Reset();

	std::vector<CCourse::VtxInfo> vtxInfo = m_pCourse->GetVecVtxinfo();
	std::vector<MyLib::Vector3> vecpos;

	// posの要素渡し
	//std::transform(vtxInfo.begin(), vtxInfo.end(), std::back_inserter(vecpos), [](const CCourse::VtxInfo& info)->MyLib::Vector3 {return info.pos; });

	MyLib::Vector3 setpos;
	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * -600.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * -600.0f;
		vecpos.push_back(setpos);
	}

	// 各頂点座標
	pStoneWall->SetVecVtxPosition(vecpos);
	pStoneWall->BindVtxPosition();




	//=============================
	// 石垣(手前)
	//=============================
	CStoneWall* pStoneWall_Front = CStoneWall_Front::Create();

	// 基点地点設定
	pStoneWall_Front->SetVecPosition(m_pCourse->GetVecPosition());
	pStoneWall_Front->Reset();

	vtxInfo = m_pCourse->GetVecVtxinfo();
	vecpos.clear();

	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * 800.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * 800.0f;
		vecpos.push_back(setpos);
	}

	// 各頂点座標
	pStoneWall_Front->SetVecVtxPosition(vecpos);
	pStoneWall_Front->BindVtxPosition();







	//=============================
	// うねりの街フィールド
	//=============================
	CMapMesh* pTownField = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS);
	pTownField->SetVecPosition(m_pCourse->GetVecPosition());
	pTownField->Reset();

	// 石垣の頂上に頂点をそろえる
	pTownField->SetVecVtxPosition(pStoneWall->GetVecTopPosition());
	pTownField->BindVtxPosition();


	//=============================
	// うねりの街フィールド(手前)
	//=============================
	CMapMesh* pTownFieldFront = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS_FRONT);
	pTownFieldFront->SetVecPosition(m_pCourse->GetVecPosition());
	pTownFieldFront->Reset();

	// 石垣の頂上に頂点をそろえる
	pTownFieldFront->SetVecVtxPosition(pStoneWall_Front->GetVecTopPosition());
	pTownFieldFront->BindVtxPosition();



	//=============================
	// ステンシル影
	//=============================
	CStencilShadow::Create();

	/*CWaterField::Create(CWaterField::TYPE::TYPE_NORMAL);
	CWaterField::Create(CWaterField::TYPE::TYPE_RIGHT);
	CWaterField::Create(CWaterField::TYPE::TYPE_LEFT);*/


	CCheckpoint::Create(1000.0f);
	CCheckpoint::Create(2000.0f);
	CCheckpoint::Create(3000.0f);
	CCheckpoint::Create(4000.0f);
	CCheckpoint::Create(5000.0f);
	CCheckpoint::Create(6000.0f);
	CCheckpoint::Create(7000.0f);
	CCheckpoint::Create(8000.0f);
	CCheckpoint::Create(9000.0f);

	// チェックポイント通過リセット
	CCheckpoint::ResetSaveID();

	//=============================
	// 判定ゾーンマネージャ
	//=============================
	m_pJudgeZoneManager = CJudgeZoneManager::Create();
	m_pJudgeZoneManager->Load("data\\TEXT\\judgezonelist\\judgezonelist_01.txt");

	//=============================
	// マップUI作成
	//=============================
	m_pMapUI = CMapUI::Create();

	pPlayer = playerList.GetData(0);
	m_pMapUI->BindPlayer(pPlayer);

	//=============================
	// 水中石マネージャ
	//=============================
	CWaterStone_Manager::Create();

	// BGM再生
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_BGM_GAME);
	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_BGM_WATER_FLOW);

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
		m_pTimer = nullptr;
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


	// 障害物マネージャ
	if (m_pObstacleManager != nullptr)
	{
		m_pObstacleManager->Uninit();
		m_pObstacleManager = nullptr;
	}

	// 荷物マネージャ
	if (m_pBaggageManager != nullptr)
	{
		m_pBaggageManager->Uninit();
		m_pBaggageManager = nullptr;
	}

	// 判定ゾーンマネージャ
	if (m_pJudgeZoneManager != nullptr)
	{
		m_pJudgeZoneManager->Uninit();
		m_pJudgeZoneManager = nullptr;
	}

	// マップUI
	if (m_pMapUI != nullptr)
	{
		m_pMapUI = nullptr;
		CMapUI::Release();
	}

	// コース
	m_pCourse = nullptr;

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
		CManager::GetInstance()->GetEdit() == nullptr)
	{
		// スコアの更新処理
		m_pScore->Update();
	}



	static float height = 50.0f, velocity = 2.0f, thickness = 20.0f;
	static int life = 60;
	static int block = 64;
	static float blocksize = 10.0f;


	ImGui::DragInt("BLOCK", &block, 1);
	ImGui::DragFloat("BLOCK SIZE", &blocksize, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("height", &height, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("velocity", &velocity, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("thickness", &thickness, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::DragInt("life", &life, 1);

	if (pInputKeyboard->GetTrigger(DIK_2))
	{
		CWaterRipple::Create(block, blocksize, MyLib::Vector3(0.0f, -5.0f, -800.0f), height, velocity, thickness, life);
	}



	static float destRadius = 13.5f;
	static int posRange = 80, createIdx = 1;

	ImGui::DragInt("Pos Range", &posRange, 1);
	ImGui::DragInt("Create Idx", &createIdx, 1);
	ImGui::DragFloat("destRadius", &destRadius, 0.5f, 0.0f, 0.0f, "%.2f");

	if (pInputKeyboard->GetPress(DIK_3))
	{
		for (int i = 0; i < createIdx; i++)
		{
			int x = UtilFunc::Transformation::Random(-posRange, posRange);
			int z = UtilFunc::Transformation::Random(-posRange, posRange);

			float randmoveX = UtilFunc::Transformation::Random(-50, 50) * 0.01f;
			float randmoveY = UtilFunc::Transformation::Random(-20, 20) * 0.01f;
			float randRadius = UtilFunc::Transformation::Random(-20, 20) * 0.01f;
			float randDestRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;
			float randCycle = UtilFunc::Transformation::Random(-20, 20) * 0.001f;


			CMeshBubble::Create(
				MyLib::Vector3(x, -5.0f, z),
				MyLib::Vector3(8.0f + randmoveX, 3.0f + randmoveY, 0.0f),
				1.0f + randRadius,
				destRadius + randDestRadius,
				0.08f + randCycle);
		}
		
	}


	if (pInputKeyboard->GetTrigger(DIK_4))
	{

		int z = UtilFunc::Transformation::Random(-20, 20) * 10;
		int x = UtilFunc::Transformation::Random(2, 50) * 100;


		//CWaterStone::Create(MyLib::Vector3(x, -40.0f, z));

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

		}
	}
#endif


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

	if (pInputKeyboard->GetTrigger(DIK_F))
	{
		// モード設定
		CManager::GetInstance()->GetFade()->SetFade(CScene::MODE_RESULT);
	}

#endif

	// シーンの更新
	CScene::Update();
}

//==========================================================================
// エディット切り替え処理
//==========================================================================
void CGame::ChangeEdit()
{
	static bool no_titlebar = false;
	static bool no_scrollbar = true;
	static bool no_menu = true;
	static bool no_move = true;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = true;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

	// エディットメニュー
	ImGui::Begin("Edit", 0, window_flags);
	{
		if (ImGui::CollapsingHeader("Window options"))
		{
			if (ImGui::BeginTable("split", 3))
			{
				ImGui::TableNextColumn(); ImGui::Checkbox("No titlebar", &no_titlebar);
				ImGui::TableNextColumn(); ImGui::Checkbox("No scrollbar", &no_scrollbar);
				ImGui::TableNextColumn(); ImGui::Checkbox("No menu", &no_menu);
				ImGui::TableNextColumn(); ImGui::Checkbox("No move", &no_move);
				ImGui::TableNextColumn(); ImGui::Checkbox("No resize", &no_resize);
				ImGui::TableNextColumn(); ImGui::Checkbox("No collapse", &no_collapse);
				ImGui::TableNextColumn(); ImGui::Checkbox("No close", &no_close);
				ImGui::TableNextColumn(); ImGui::Checkbox("No nav", &no_nav);
				ImGui::TableNextColumn(); ImGui::Checkbox("No background", &no_background);
				ImGui::TableNextColumn(); ImGui::Checkbox("No bring to front", &no_bring_to_front);
				ImGui::TableNextColumn(); ImGui::Checkbox("Unsaved document", &unsaved_document);
				ImGui::EndTable();
			}
		}

		// テキスト
		static const char* items[] = { "OFF", "Map", "Obstacle", "Course", "WaterStone"};
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

		if (m_pEdit != nullptr) {
			m_pEdit->Update();
		}

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
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		m_pEdit = nullptr;
	}
}
