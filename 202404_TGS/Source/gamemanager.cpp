//=============================================================================
// 
// ゲームマネージャ処理 [gamemanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "gamemanager.h"
#include "game.h"
#include "debugproc.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "camera.h"
#include "sound.h"
#include "blackframe.h"
#include "stage.h"
#include "map.h"
#include "stagecleartext.h"
#include "limitarea.h"
#include "calculation.h"
#include "fog.h"
#include "fade.h"
#include "beforebattle.h"
#include "stagename.h"
#include "timer.h"
#include "input.h"
#include "baggage.h"
#include "course.h"
#include "judgezoneManager.h"
#include "peoplemanager.h"
#include "spline.h"
#include "request_people.h"
#include "subtitle.h"
#include "receiver_people.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int POINT_WAVECLEAR = 5;		// ウェーブクリアのポイント
	const float POSR_Y_APPROACH_COEF = 0.3f;	//カメラが近づく際の慣性
	const float POSR_Y_PULL_SCREEN_POS = 210.0f;	// カメラが引き始めるスクリーン座標
	const float POSR_Y_APPROACH_SCREEN_POS = SCREEN_HEIGHT * 0.5f;	// カメラが近づき始めるスクリーン座標
	const float POSR_YDEST_BAGGTOPLAYER_RATIO = 0.4f;	// 荷物とプレイヤー距離の割合（posRYDest）

	const int CHANGE_BASEPOINT[] =	// ポイント変更する基準
	{
		40,	// AAA
		20,	// BBB
		10,	// CCC
		1,	// DDD
	};
}

namespace SceneTime
{
	const float RequestStart = 3.5f;	// 依頼開始
}

//==========================================================================
// コンストラクタ
//==========================================================================
CGameManager::CGameManager()
{
	// 値のクリア
	m_SceneType = SCENE_MAIN;	// シーンの種類
	m_OldSceneType = SCENE_MAIN;	// シーンの種類
	m_bEndRush = false;			// ラッシュが終了したか
	m_bControll = false;		// 操作できるか
	m_bEndNormalStage = false;	// 通常ステージが終了したか
	m_bGameStart = false;		// ゲーム開始時のフラグ
	m_nNowStage = 0;			// 現在のステージ
	m_nNumStage = 0;			// ステージの総数
	m_nEvaluationPoint = 0;		// 評価ポイント
	m_fCameraLengthOld = 0;		// 前のカメラの距離
	m_fPosRY = 0.0f;
	m_fSceneTimer = 0.0f;		// シーンタイマー
	m_pRequestPeople = nullptr;	// 依頼人のポインタ
	m_pReceiverPeople = nullptr;
	m_nJudgeRank = 0;
}

//==========================================================================
// デストラクタ
//==========================================================================
CGameManager::~CGameManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGameManager* CGameManager::Create(CScene::MODE mode)
{
	// メモリ確保
	CGameManager* pManager = nullptr;

	// インスタンス生成
	switch (mode)
	{
	case CScene::MODE_GAME:
		pManager = DEBUG_NEW CGameManager;
		break;

	}

	if (pManager != nullptr)
	{// メモリの確保が出来ていたら

		// 初期化処理
		HRESULT hr = pManager->Init();

		if (FAILED(hr))
		{// 失敗していたら
			return nullptr;
		}
	}

	return pManager;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGameManager::Init()
{
	m_bControll = true;			// 操作できるか
	m_bEndNormalStage = false;	// 通常ステージが終了したか
	m_bGameStart = false;		// ゲーム開始時のフラグ

#if _DEBUG
	m_nNowStage = 0;			// 現在のステージ
	m_SceneType = SceneType::SCENE_START;	// シーンの種類 
#else
	m_nNowStage = 0;			// 現在のステージ
	m_SceneType = SceneType::SCENE_START;	// シーンの種類 
#endif

	m_OldSceneType = m_SceneType;

#if 1
	// 依頼人生成
	m_pRequestPeople = CRequestPeople::Create(MyLib::Vector3(500.0f, 0.0f, 900.0f));
	m_pRequestPeople->SetState(CRequestPeople::STATE::STATE_WAIT);

	// 黒フレームイン
	CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_INCOMPLETION);

	// カメラモーション再生
	CManager::GetInstance()->GetCamera()->GetCameraMotion()->SetMotion(CCameraMotion::MOTION::MOTION_PASS, CCameraMotion::EASING::Linear);

	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\sample.png");
	pSubTitle->SetSizeByHeight(40.0f);
#else

	m_SceneType = SceneType::SCENE_WAIT_AIRPUSH;	// シーンの種類 
#endif
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGameManager::Uninit()
{
	
}

//==========================================================================
// 更新処理
//==========================================================================
void CGameManager::Update()
{
	
	// 操作状態
	m_fSceneTimer += CManager::GetInstance()->GetDeltaTime();		// シーンタイマー
	switch (m_SceneType)
	{
	case CGameManager::SceneType::SCENE_MAIN:
		m_bControll = true;
		CheckJudgeZone();
		ContainPlayerBaggage();
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_START:
		m_bControll = false;
		SceneStart();
		break;

	case CGameManager::SceneType::SCENE_COUNTDOWN:		// カウントダウン
		TurnAway();
		break;

	case CGameManager::SceneType::SCENE_MAINCLEAR:
		m_bControll = false;
		SceneGoal();
		break;

	case SceneType::SCENE_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_DURING_MAINRESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_BEFOREBATTLE:
		m_bControll = false;
		break;

	case SceneType::SCENE_BATTLESTART:
		m_bControll = false;
		break;

	case SceneType::SCENE_SKILLTREE:	// スキルツリー
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_BOSS:
		m_bControll = true;
		break;

	case CGameManager::SceneType::SCENE_TRANSITIONWAIT:
		m_bControll = false;
		break;

	case CGameManager::SceneType::SCENE_TRANSITION:
		m_bControll = false;
		SceneTransition();
		break;

	case SceneType::SCENE_RESULT:
		m_bControll = false;
		break;

	case SceneType::SCENE_WAIT_AIRPUSH:
		m_bControll = false;
		SceneWaitAirPush();
		TurnAway();
		break;

	case SceneType::SCENE_DEBUG:
		m_bControll = true;
		break;

	default:
		break;
	}


	for (int i = 0; i < CJudge::JUDGE::JUDGE_MAX; i++)
	{
		if (m_nEvaluationPoint >= CHANGE_BASEPOINT[i])
		{
			m_nJudgeRank = i;
			CPeopleManager::GetInstance()->SetRank(static_cast<CJudge::JUDGE>(i));
			break;
		}

		m_nJudgeRank = CJudge::JUDGE::JUDGE_MAX;
		CPeopleManager::GetInstance()->SetRank(CJudge::JUDGE::JUDGE_MAX);
	}


	// テキストの描画
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- ゲームマネージャ情報 ----------------\n"
		"【モード】[%d]\n"
		"【ステージ】[%d]\n"
		"【評価ポイント】[%d]\n"
		, m_SceneType, m_nNowStage, m_nEvaluationPoint);
}

//==========================================================================
// ゲームクリア時の設定
//==========================================================================
void CGameManager::GameClearSettings()
{
	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = playerList.GetData(0);


}

//==========================================================================
// 開始演出
//==========================================================================
void CGameManager::SceneStart()
{
	if (m_fSceneTimer >= SceneTime::RequestStart &&
		m_pRequestPeople->GetState() == CRequestPeople::STATE::STATE_WAIT)
	{
		m_pRequestPeople->SetState(CRequestPeople::STATE::STATE_PASS);
	}
}

//==========================================================================
// ゲームリザルトの設定
//==========================================================================
void CGameManager::GameResultSettings()
{

	// クリアテキスト生成
	CStageClearText::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f));


}

//==========================================================================
// メイン遷移中
//==========================================================================
void CGameManager::SceneTransition()
{
	// 遷移なしフェードの状態取得
	CInstantFade::STATE fadestate = CManager::GetInstance()->GetInstantFade()->GetState();

	if (fadestate == CInstantFade::STATE_FADECOMPLETION ||
		!m_bGameStart)
	{// 完了した瞬間

		// BGMストップ
		CSound::GetInstance()->StopSound();
		CSound::GetInstance()->PlaySound(CSound::LABEL_BGM_GAME);

		// ゲーム開始時のフラグ
		m_bGameStart = true;

		// カメラ取得
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera != nullptr)
		{
			pCamera->SetEnableFollow(true);
		}

		// エリア制限情報取得
		CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
		CLimitArea* pLimitArea = nullptr;
		while (limitareaList.ListLoop(&pLimitArea))
		{
			pLimitArea->Kill();
		}

		// エフェクト全て停止
		CMyEffekseer::GetInstance()->StopAll();
		CMyEffekseer::GetInstance()->StopAll();

		// フォグリセット
		MyFog::ToggleFogFrag(false);

		// プレイヤー取得
		CListManager<CPlayer> playerList = CPlayer::GetListObj();
		CPlayer* pPlayer = playerList.GetData(0);

		// 敵配置
		SetEnemy();

		// 戦闘準備に遷移
		CBeforeBattle::Create(MyLib::Vector3(640.0f, 360.0f, 0.0f));
		SetType(SceneType::SCENE_BEFOREBATTLE);
	}
}

//==========================================================================
// 空気送り待ち
//==========================================================================
void CGameManager::SceneWaitAirPush()
{
	CTimer* pTimer = CGame::GetInstance()->GetTimer();
	CKeyConfig* pConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	if (pTimer != nullptr &&
		pConfigPad->GetTrigger(INGAME::ACT_AIR) ||
		CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN))
	{
		pTimer->SetEnableAddTime(true);
		SetType(SceneType::SCENE_MAIN);
	}
	else if (pTimer != nullptr)
	{
		pTimer->SetEnableAddTime(false);
	}
}

//==========================================================================
// ゴール状態
//==========================================================================
void CGameManager::SceneGoal()
{
	CGame::GetInstance()->GetTimer()->SetState(CTimer::eState::STATE_GOAL);
}

//==========================================================================
// プレイヤーと荷物を画面内に収める
//==========================================================================
void CGameManager::ContainPlayerBaggage()
{
	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	// 荷物取得
	CListManager<CBaggage> baggageList = CBaggage::GetListObj();
	CBaggage* pBaggage = nullptr;
	baggageList.ListLoop(&pBaggage);

	// 収めるものの座標
	MyLib::Vector3 posPlayer = pPlayer->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	
	// posR現在のY座標取得
	m_fPosRY = pCamera->GetPositionR().y;

	// 範囲内に収める
	MyLib::Vector3 screenPos = pCamera->GetScreenPos(posBaggage);

	if (screenPos.y <= POSR_Y_PULL_SCREEN_POS)
	{// 上画面外（引く）
		float posRYDest = (posBaggage.y - posPlayer.y) * POSR_YDEST_BAGGTOPLAYER_RATIO;
		if (m_fPosRY <= posRYDest)
		{
			m_fPosRY = posRYDest;
		}
	}
	else if (screenPos.y >= POSR_Y_APPROACH_SCREEN_POS)
	{// 下画面外（押す）
		float posRYDest = posBaggage.y;
		m_fPosRY += (posRYDest - m_fPosRY) * POSR_Y_APPROACH_COEF;
		m_fPosRY = UtilFunc::Transformation::Clamp(m_fPosRY, 200.0f, FLT_MAX);
	}

	pCamera->SetAutoMovingPosR(MyLib::Vector3(posBaggage.x, m_fPosRY, posBaggage.z));
}

//==========================================================================
// カメラが常に横を向くようにする
//==========================================================================
void CGameManager::TurnAway()
{
	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera->GetCameraMotion()->IsEdit())
	{
		return;
	}

	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	// 移動方向から角度算出
	float moveLength = pPlayer->GetMoveLength();
	MyLib::Vector3 posDest = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), moveLength + 1.0f);
	
	float angleXZ = pPlayer->GetPosition().AngleXZ(posDest);
	angleXZ += (D3DX_PI * 0.5f);
	UtilFunc::Transformation::RotNormalize(angleXZ);

	// 角度設定
	MyLib::Vector3 rot = pCamera->GetRotation();
	pCamera->SetRotation(MyLib::Vector3(rot.x, angleXZ + pCamera->GetOriginRotation().y, rot.z));
}

//==========================================================================
// 判定ゾーン確認
//==========================================================================
void CGameManager::CheckJudgeZone()
{
	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;
	playerList.ListLoop(&pPlayer);

	// コース取得
	CCourse* pCource = CGame::GetInstance()->GetCourse();

	// 進行度計算して確認
	float progress = pPlayer->GetMoveLength() / pCource->GetCourceLength();
	CManager::GetInstance()->GetDebugProc()->Print("進行度:%f\n", progress);
	CJudgeZoneManager::GetInstance()->Check(progress);
}

//==========================================================================
// ボス設定
//==========================================================================
void CGameManager::SetBoss()
{

}

//==========================================================================
// 敵設定
//==========================================================================
void CGameManager::SetEnemy()
{

	// ステージ切り替え
	CGame::GetInstance()->GetStage()->ChangeStage("data\\TEXT\\stage\\info.txt");

	// マップ切り替え
	MyMap::ChangeMap("data\\TEXT\\map\\info.txt");

	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// リストループ
	while (playerList.ListLoop(&pPlayer))
	{
		// 位置設定
		pPlayer->SetPosition(MyLib::Vector3(0.0f, 0.0f, -1000.0f));
		pPlayer->SetRotation(MyLib::Vector3(0.0f, D3DX_PI, 0.0f));
		pPlayer->SetRotDest(D3DX_PI);
	}

	// カメラの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->Reset(CScene::MODE_GAME);
	}

	// 種類設定
	m_SceneType = SCENE_MAIN;
}

//==========================================================================
// ステージの総数設定
//==========================================================================
void CGameManager::SetNumStage(int nStage)
{ 
	m_nNumStage = nStage;
}

//==========================================================================
// ステージの加算
//==========================================================================
void CGameManager::AddNowStage()
{
	// 加算
	m_nNowStage++;

	if (m_nNumStage <= m_nNowStage)
	{// 総ステージ数を超えたら

		// 通常ステージが終了判定
		m_bEndNormalStage = true;
	}

}

//==========================================================================
// 現在のステージ取得
//==========================================================================
int CGameManager::GetNowStage()
{
	return m_nNowStage;
}

//==========================================================================
// シーンの種類設定
//==========================================================================
void CGameManager::SetType(SceneType type)
{
	m_OldSceneType = m_SceneType;
	m_SceneType = type;
}

//==========================================================================
// シーンの種類取得
//==========================================================================
CGameManager::SceneType CGameManager::GetType()
{
	return m_SceneType;
}

