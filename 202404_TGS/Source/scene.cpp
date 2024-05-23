//=============================================================================
// 
//  シーン処理 [scene.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "scene.h"
#include "renderer.h"
#include "calculation.h"
#include "Xload.h"
#include "map.h"
#include "fade.h"
#include "elevation.h"
#include "player.h"
#include "camera.h"
#include "MyEffekseer.h"
#include "fog.h"
#include "edit_map.h"

// 遷移先
#include "game.h"
#include "title.h"
#include "result.h"
#include "tutorial.h"
#include "ranking.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MAP_TEXT = "data\\TEXT\\map\\info.txt";
	const char* ELEVATION_TEXT = "data\\TEXT\\elevation\\field_mountain.txt";
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CElevation *CScene::m_pObject3DMesh = nullptr;		// オブジェクト3Dメッシュのオブジェクト

//==========================================================================
// コンストラクタ
//==========================================================================
CScene::CScene()
{
	// 変数のクリア
	m_mode = MODE_TITLE;
}

//==========================================================================
// デストラクタ
//==========================================================================
CScene::~CScene()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CScene *CScene::Create(CScene::MODE mode)
{
	// 生成用のオブジェクト
	CScene *pScene = nullptr;

	if (pScene == nullptr)
	{// nullptrだったら

		// 生成処理
		switch (mode)
		{
		case CScene::MODE_TITLE:
			pScene = CTitle::Create();
			break;

		case CScene::MODE_GAME:
		case CScene::MODE::MODE_GAMETUTORIAL:
			pScene = CGame::Create(mode);
			break;

		case MODE_TUTORIAL:
			pScene = CTutorial::Create();
			break;

		case CScene::MODE_RESULT:
			pScene = DEBUG_NEW CResult;
			break;

		case CScene::MODE_RANKING:
			pScene = DEBUG_NEW CRanking;
			break;
		}

		if (pScene != nullptr)
		{// メモリの確保が出来ていたら

			// モードの設定
			pScene->m_mode = mode;
		}

		return pScene;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CScene::Init()
{

	// エフェクト全て停止
	CMyEffekseer::GetInstance()->StopAll();

	// フォグリセット
	MyFog::ToggleFogFrag(false);

	//**********************************
	// マップの生成
	//**********************************
#if _DEBUG
	if (FAILED(MyMap::Create(MAP_TEXT)))
	{// 失敗した場合
		return E_FAIL;
	}
#endif

	//**********************************
	// マップの生成
	//**********************************
#ifndef _DEBUG
	CEdit_Map_Release::Create(MAP_TEXT, CManager::BuildMode::MODE_RELEASE);
#endif

	//**********************************
	// 起伏の地面
	//**********************************
	m_pObject3DMesh = CElevation::Create(ELEVATION_TEXT);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CScene::Uninit()
{

}

//==========================================================================
// 更新処理
//==========================================================================
void CScene::Update()
{
	
}

//==========================================================================
// 描画処理
//==========================================================================
void CScene::Draw()
{
	
}

//==========================================================================
// シーンのリセット
//==========================================================================
void CScene::ResetScene()
{
	//**********************************
	// 破棄フェーズ
	//**********************************
	// 起伏の地面
	if (m_pObject3DMesh != nullptr)
	{
		m_pObject3DMesh->Uninit();
		m_pObject3DMesh = nullptr;
	}

	// マップ
	MyMap::Release();

	//**********************************
	// 生成フェーズ
	//**********************************
	// マップ
	if (FAILED(MyMap::Create(MAP_TEXT)))
	{// 失敗した場合
		return;
	}

	// ボスステージの起伏生成
	m_pObject3DMesh = CElevation::Create(ELEVATION_TEXT);
}

//==========================================================================
// 現在のモード取得
//==========================================================================
CScene::MODE CScene::GetMode()
{
	return m_mode;
}

//==========================================================================
// 3Dオブジェクトの取得
//==========================================================================
CElevation *CScene::GetElevation()
{
	return m_pObject3DMesh;
}
