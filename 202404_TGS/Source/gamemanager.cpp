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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int POINT_WAVECLEAR = 5;		// ウェーブクリアのポイント
	const float CAMERA_MIN_LENGTH = 2000.0f;	//カメラの最短距離
	const float CAMERA_MAX_LENGTH = 1600.0f;	//カメラの最長距離
	const float CAMERA_MAX_ROT = 0.15f;			//カメラ最高角度
}

//==========================================================================
// コンストラクタ
//==========================================================================
CGameManager::CGameManager()
{
	// 値のクリア
	m_SceneType = SCENE_MAIN;	// シーンの種類
	m_bEndRush = false;			// ラッシュが終了したか
	m_bControll = false;		// 操作できるか
	m_bEndNormalStage = false;	// 通常ステージが終了したか
	m_bGameStart = false;		// ゲーム開始時のフラグ
	m_nNowStage = 0;			// 現在のステージ
	m_nNumStage = 0;			// ステージの総数
	m_fCameraLengthOld = 0;		// 前のカメラの距離
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
	m_SceneType = SceneType::SCENE_WAIT_AIRPUSH;	// シーンの種類 
#else
	m_nNowStage = 0;			// 現在のステージ
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
	switch (m_SceneType)
	{
	case CGameManager::SceneType::SCENE_MAIN:
		m_bControll = true;
		ContainPlayerBaggage();
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

	case CGameManager::SceneType::SCENE_ENHANCE:
		m_bControll = true;
		SceneEnhance();
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
		break;

	case SceneType::SCENE_DEBUG:
		m_bControll = true;
		break;

	default:
		break;
	}


	// テキストの描画
	CManager::GetInstance()->GetDebugProc()->Print(
		"---------------- ゲームマネージャ情報 ----------------\n"
		"【モード】[%d]\n"
		"【ステージ】[%d]\n"
		, m_SceneType, m_nNowStage);
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
		CManager::GetInstance()->GetSound()->StopSound();
		if (m_nNowStage != 3)
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_BGM_GAME);
		}
		else {
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_BGM_BOSS);
		}

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
// 強化シーン
//==========================================================================
void CGameManager::SceneEnhance()
{
	// 遷移なしフェードの状態取得
	CInstantFade::STATE fadestate = CManager::GetInstance()->GetInstantFade()->GetState();

	if (fadestate != CInstantFade::STATE_FADECOMPLETION &&
		m_bGameStart)
	{// 完了してない
		return;
	}

	// カメラリセット
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->Reset(CScene::MODE_GAME);
		pCamera->SetStateCameraV(DEBUG_NEW CStateCameraV_Enhance());
	}

	// エフェクト全て停止
	CMyEffekseer::GetInstance()->StopAll();

	// ステージ加算
	if (m_bGameStart)
	{
		AddNowStage();
		if (m_bEndNormalStage)
		{
			return;
		}
	}

	// ゲーム開始時のフラグ
	m_bGameStart = true;


	// BGMストップ
	CManager::GetInstance()->GetSound()->StopSound();
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_BGM_ENHANCE);
	CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_BGM_ENHANCE_WIND);

	// ステージ切り替え
	CGame::GetInstance()->GetStage()->ChangeStage("data\\TEXT\\stage\\info.txt");

	// マップ切り替え
	MyMap::ChangeMap("data\\TEXT\\map\\map_enhance.txt");

	// プレイヤー取得
	CListManager<CPlayer> playerList = CPlayer::GetListObj();
	CPlayer* pPlayer = nullptr;

	// リストループ
	while (playerList.ListLoop(&pPlayer))
	{
		pPlayer->SetPosition(0.0f);
		pPlayer->SetLife(pPlayer->GetLifeOrigin());
	}

	// エフェクト全て停止
	CMyEffekseer::GetInstance()->StopAll();

	// ステージ名生成
	CStageName::Create();

	CLimitArea::sLimitEreaInfo info;
	info.fMaxX = 600.0f;
	info.fMaxZ = 1050.0f;
	info.fMinX = -600.0f;
	info.fMinZ = -1000.0f;
	CLimitArea* pArea = CLimitArea::Create(info);
	pArea->SetEnableDisp(false);

	MyFog::SetFogparam(D3DXCOLOR(1.0f, 0.95f, 0.9f, 1.0f), info.fMaxZ, 3000.0f, D3DFOG_LINEAR);
	MyFog::ToggleFogFrag(true);
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
		pConfigPad->GetTrigger(INGAME::ACT_AIR))
	{
		pTimer->SetEnableAddTime(true);
		m_SceneType = CGameManager::SceneType::SCENE_MAIN;
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
	MyLib::Vector3 posV = pCamera->GetPositionV();
	MyLib::Vector3 cameraRot = pCamera->GetRotation();

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
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();

	// カメラ角度調整
	float lengthVR = CAMERA_MIN_LENGTH; //a
	float originRot = -pCamera->GetOriginRotation().z; //thetaA
	float lengthVInterToV = lengthVR * cosf(originRot);	//c
	float lengthBagOriginToVInter = lengthVR * sinf(originRot);	//d
	MyLib::Vector3 posVInter = posPlayer + posBaggageOrigin + posBaggageOrigin.Normal() * lengthBagOriginToVInter;
	MyLib::Vector3 posVDef = posVInter;
	posVDef.x += lengthVInterToV * sinf(cameraRot.y);
	posVDef.z += lengthVInterToV * -cosf(cameraRot.y);

	float lengthVInterToBaggage = posBaggage.Distance(posVInter);	//e
	if ((posBaggage - posVInter).y < 0.0f)
	{
		lengthVInterToBaggage *= -1;
	}

	//↓ここから問題ありそう↓//
	MyLib::Vector3 rotOrigin = pCamera->GetOriginRotation();
	//いったん仮
	//float angle = atan2f(lengthVInterToBaggage, lengthVInterToV);
	static float angle = -0.1f;
	static float angleDest = angle;
	float afsdjfnd = CAMERA_MIN_LENGTH;
	//lengthVR = CAMERA_MIN_LENGTH;
	static MyLib::Vector3 posR = MyLib::Vector3(posPlayer.x, posBaggageOrigin.y, posPlayer.z);
	static float posRYDest = posBaggageOrigin.y;
	float hoge = lengthVInterToBaggage + lengthBagOriginToVInter;
	if (hoge >= 300.0f)
	{
		angleDest = rotOrigin.z + 0.1f;
		posRYDest = 300.0f;

		if (hoge >= 500.0f)
		{
			angleDest = rotOrigin.z + 0.2f;
			posRYDest = 500.0f;

			if (hoge >= 700.0f)
			{
				angleDest = rotOrigin.z + 0.3f;
				posRYDest = 700.0f;
			}
		}
	}
	lengthVR = posR.Distance(posVDef);

	MyLib::Vector3 vec = posR - posVDef;
	angleDest = atan2f(vec.y, vec.z);
	angle += (angleDest - angle) * 0.2f;

	posR.x = posPlayer.x;
	posR.y += (posRYDest - posR.y) * 0.05f;

	CEffect3D::Create(
		MyLib::Vector3(posPlayer.x, 0.0f, posPlayer.z),
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	CEffect3D::Create(
		MyLib::Vector3(posPlayer.x, 300.0f, posPlayer.z),
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	CEffect3D::Create(
		MyLib::Vector3(posPlayer.x, 450.0f, posPlayer.z),
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	CEffect3D::Create(
		MyLib::Vector3(posPlayer.x, 700.0f, posPlayer.z),
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	//↑ここまで問題ありそう↑//

	pCamera->SetLenDest(lengthVR, 60, 1.0f,0.03f);
	pCamera->SetPositionR(posR);
	pCamera->SetRotation(MyLib::Vector3(0.0f,0.0f, angle));
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
	m_SceneType = type;
}

//==========================================================================
// シーンの種類取得
//==========================================================================
CGameManager::SceneType CGameManager::GetType()
{
	return m_SceneType;
}

