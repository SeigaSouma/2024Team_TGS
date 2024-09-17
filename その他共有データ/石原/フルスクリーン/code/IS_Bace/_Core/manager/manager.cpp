//======================================================
//
//マネージャ[manager.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "..\renderer\renderer.h"
#include "..\texture\texture.h"
#include "..\input\input.h"
#include "..\sound\sound.h"
#include "..\debugproc\debugproc.h"
#include "..\camera\camera.h"
#include "..\light\light.h"
#include "..\lostrssmanager\lostrssmanager.h"
#include "..\_object\object_empty\object.h"
#include "..\xmodel\xmodel.h"

//シーン
#include "..\_scene/title/title.h"
#include "..\_scene/tutorial/tutorial.h"
#include "..\_scene/game/game.h"
#include "..\_scene/result/result.h"
#include "..\_scene/ranking/ranking.h"

//乱数
#include <time.h>

//マクロ
#define FPS_SPEED	(500)	//FPS計測時間

//静的メンバ変数
CManager* CManager::m_pManager = nullptr;

//************************************************
//マネージャクラス
//************************************************
//=================================
//コンストラクタ
//=================================
CManager::CManager()
{
	m_pRenderer = nullptr;
	m_pInputKeyboard = nullptr;
	m_pInputMouse = nullptr;
	m_pInputPad = nullptr;
	m_pDebProc = nullptr;
	m_pSound = nullptr;
	m_pCamera = nullptr;
	m_pLight = nullptr;
	m_pTexture = nullptr;
	m_pScene = nullptr;
	m_bEnableUpdate = true;
	m_bWindowed = true;

	m_nFPS = 0;
	m_dwFrameCount = 0;
}

//=================================
//デストラクタ
//=================================
CManager::~CManager()
{}

//=================================
//初期化
//=================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//乱数を乱数にする
	srand((unsigned int)time(nullptr));

	//生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputMouse = new CInputMouse;
	m_pInputPad = new CInputGamePad;
	m_pSound = new CSound;
	m_pRenderer = new CRenderer;
	m_pDebProc = new CDebugProc;
	m_pCamera = new CCamera;
	m_pLight = new CLight;
	m_pTexture = new CTexture;
	CLostResourceManager::GetInstance();	//取得はしないけど生成はする

	//レンダラー初期化
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	//キーボード初期化
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//マウス初期化
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//ゲームパッド初期化
	if (FAILED(m_pInputPad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//サウンド初期化
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	//デバッグ初期化
	m_pDebProc->Init();

	//カメラ初期化
	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	//ライト初期化
	if (FAILED(m_pLight->Init()))
	{
		return E_FAIL;
	}

	//テクスチャ初期化
	if (FAILED(m_pTexture->Load("data\\preload.txt")))
	{
		return E_FAIL;
	}

	//FPS計測器初期化
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//モード設定
#ifdef _DEBUG	//デバッグビルド
	GetInstance()->SetMode(CScene::MODE_GAME);
#else			//リリースビルド
	GetInstance()->SetMode(CScene::MODE_TITLE);
#endif

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CManager::Uninit(void)
{
	//モデル破棄
	CXModel::UnloadAll();			//Xモデル

	//オブジェクト終了+破棄
	CObject::ReleaseAll();
	CLostResourceManager::Release();

	//テクスチャ破棄
	if (m_pTexture != nullptr)
	{//テクスチャ終了
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	//ライト破棄
	if (m_pLight != nullptr)
	{//ライト終了
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	//カメラ破棄
	if (m_pCamera != nullptr)
	{//カメラ終了
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	//デバッグ破棄
	if (m_pDebProc != nullptr)
	{//デバッグ終了
		m_pDebProc->Uninit();
		delete m_pDebProc;
		m_pDebProc = nullptr;
	}

	//サウンド破棄
	if (m_pSound != nullptr)
	{//サウンド終了
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	//ゲームパッド破棄
	if (m_pInputPad != nullptr)
	{//マウス終了
		m_pInputPad->Uninit();
		delete m_pInputPad;
		m_pInputPad = nullptr;
	}

	//マウス破棄
	if (m_pInputMouse != nullptr)
	{//マウス終了
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	//キーボード破棄
	if (m_pInputKeyboard != nullptr)
	{//キーボード終了
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	//レンダラー破棄
	if (m_pRenderer != nullptr)
	{//レンダラー終了
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

//=================================
//更新
//=================================
void CManager::Update(void)
{
	//入力系
	m_pInputKeyboard->Update();
	m_pInputMouse->Update();
	m_pInputPad->Update();

	//描画系
	if (m_bEnableUpdate == true)
	{
		m_pRenderer->Update();
	}

	if (m_pInputKeyboard->GetTrigger(DIK_F8))
	{
		m_bWindowed = !m_bWindowed;

		if (m_bWindowed == true)
		{
			m_pRenderer->SwitchDisplayMode(CRenderer::DISPLAYMODE::MODE_WINDOW);
		}
		else
		{
			m_pRenderer->SwitchDisplayMode(CRenderer::DISPLAYMODE::MODE_FULLSCREEN);
		}
	}

	//3D系
	m_pCamera->Update();
	m_pLight->Update();

	//中身知らんけど何かしらのシーン
	m_pScene->Update();

	//この時点で死亡フラグが立っているオブジェを殺す
	CObject::Death();

	//FPS計測器の処理
	m_dwFrameCount++;

	//デバッグ表示
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
}

//=================================
//描画
//=================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();
}

//=================================
//FPS計測
//=================================
void CManager::CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime)
{
	m_nFPS = (m_dwFrameCount * 1000) / (dwCurrentTime - dwExecLastTime);
	m_dwFrameCount = 0;
}

//=================================
//取得（シングルトン）
//=================================
CManager* CManager::GetInstance(void)
{
	if (m_pManager == nullptr)
	{
		m_pManager = new CManager;
	}
	return m_pManager;
}

//=================================
//破棄
//=================================
HRESULT CManager::Release(void)
{
	if (m_pManager != nullptr)
	{
		m_pManager->Uninit();
		delete m_pManager;
		m_pManager = nullptr;

		return S_OK;
	}
	return E_FAIL;
}

//=================================
//モード設定
//=================================
void CManager::SetMode(CScene::MODE mode)
{
	//音止める
	m_pSound->Stop();

	//現在のモード破棄
	if (m_pScene != nullptr)
	{//なんか入ってる
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	if (m_pScene == nullptr)
	{//ぬるぬる
		//新モード生成
		m_pScene = CScene::Create(mode);
	}
}

//************************************************
//シーン（抽象）クラス
//************************************************
//=================================
//コンストラクタ
//=================================
CScene::CScene()
{
}

//=================================
//デストラクタ
//=================================
CScene::~CScene()
{
}

//=================================
//生成
//=================================
CScene* CScene::Create(MODE mode)
{
	CScene* pScene = nullptr;

	if (pScene == nullptr)
	{
		//シーンの生成
		switch (mode)
		{
		case MODE_TITLE:
			pScene = new CTitle;
			break;
		case MODE_TUTORIAL:
			pScene = new CTutorial;
			break;
		case MODE_GAME:
			pScene = new CGame;
			break;
		case MODE_RANKING:
			pScene = new CRanking;
			break;
		default:	//んなもんはない
			return nullptr;
			break;
		}

		//初期化
		pScene->Init();

		//モード入れとく
		pScene->m_mode = mode;

		return pScene;
	}
	else
	{
		return nullptr;
	}
}
