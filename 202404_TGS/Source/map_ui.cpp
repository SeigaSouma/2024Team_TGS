//=============================================================================
//
// マップUI処理 [map_ui.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "map_ui.h"
#include "objectX.h"
#include "game.h"
#include "course.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "player.h"

//=============================================================================
// 定数定義
//=============================================================================
namespace
{
	const float DISTANCE = 600.0f;	// 距離
	const MyLib::Vector3 POS = MyLib::Vector3(-1000.0f, 100.0f, 100.0f);
	const MyLib::Vector3 PLAYERPOS = MyLib::Vector3(-78.0f, 33.0f, 1.7f);
	const MyLib::Vector3 CAMPOS = MyLib::Vector3(-87.0f, 108.0f, 320.0f);
	const float MODEL_ROTY = D3DX_PI * 1.0f;
	const float MODEL_ROTZ = -D3DX_PI * 0.2f;
	const float MODEL_ROTMAXMOVEZ = -D3DX_PI * 1.4f;
	const float ROTY = D3DX_PI * 0.35f;	// Y軸角度
}

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
// ファイル名
std::string CMapUI::m_aModelList[TYPE::TYPE_MAX] =
{
	"data\\MODEL\\map_ui\\map.x",
	"data\\MODEL\\map_ui\\player.x",
};

// インスタンス
CMapUI* CMapUI::m_pInstance = nullptr;

//=============================================================================
// コンストラクタ
//=============================================================================
CMapUI::CMapUI() : CObject(7)
{
	// 値のクリア
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_apObj[i] = nullptr;
	}

	m_pMyCamera = nullptr;
	m_pMyPlayer = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMapUI::~CMapUI()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CMapUI::Init()
{
	SetType(CObject::TYPE::TYPE_OBJECTX);

	// ゲーム画面が存在しているか、コースがあるか確認
	if (CGame::GetInstance() == nullptr) { return E_FAIL; }
	if (CGame::GetInstance()->GetCourse() == nullptr) { return E_FAIL; }

	// オブジェクトの生成と設定
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_apObj[i] = CObjectX::Create(m_aModelList[i]);
		m_apObj[i]->SetEnableDisp(false);
		m_apObj[i]->SetPosition(POS + i * PLAYERPOS);
		m_apObj[i]->SetRotation(MyLib::Vector3(0.0f, MODEL_ROTY, MODEL_ROTZ));
	}

	// 描画用カメラの生成と設定
	m_pMyCamera = new CCamera;
	m_pMyCamera->Init();
	m_pMyCamera->SetRotation(MyLib::Vector3(0.0f, ROTY, 0.0f));
	m_pMyCamera->SetDistance(DISTANCE);
	m_pMyCamera->WarpCamera(POS + CAMPOS);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMapUI::Uninit()
{
	// オブジェクトの廃棄
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		if (m_apObj[i] != nullptr)
		{
			m_apObj[i] = nullptr;
		}
	}

	// カメラの廃棄
	if (m_pMyCamera != nullptr)
	{
		m_pMyCamera->Uninit();
		delete m_pMyCamera;
		m_pMyCamera = nullptr;
	}

	m_pMyPlayer = nullptr;
	m_pInstance = nullptr;
}

//=============================================================================
// 更新処理
//=============================================================================
void CMapUI::Update()
{
	// マップの回転処理
	SetMapRotation();

	//float windowWidth = 100.0f;
	//const float  POS_MOVE = 0.1f;
	////=============================
	//// 位置設定
	////=============================
	//ImGui::Text("pos");
	//ImGui::SameLine();
	//// X
	//ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
	//{
	//	ImGui::SetNextItemWidth(windowWidth);
	//	ImGui::DragFloat("x", &PLAYERPOS.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
	//	ImGui::SameLine();
	//}
	//ImGui::PopID();
	//// Y
	//ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
	//{
	//	ImGui::SetNextItemWidth(windowWidth);
	//	ImGui::DragFloat("y", &PLAYERPOS.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
	//	ImGui::SameLine();
	//}
	//ImGui::PopID();
	//// Z
	//ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
	//{
	//	ImGui::SetNextItemWidth(windowWidth);
	//	ImGui::DragFloat("z", &PLAYERPOS.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
	//}
	//ImGui::PopID();
	//m_pMyCamera->WarpCamera(POS + CAMPOS);

	//m_apObj[TYPE::TYPE_PLAYER]->SetPosition(POS + PLAYERPOS);
}

//=============================================================================
// 描画処理
//=============================================================================
void CMapUI::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスへのポインタを取得
	D3DXMATRIX mtxView, mtxProjection;
	D3DVIEWPORT9 viewportDef;

	// 現在のビューポートの取得
	pDevice->GetViewport(&viewportDef);

	// 現在のビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// 現在のプロジェクションマトリックスの取得
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);

	// 別のカメラを設定して描画する
	if (m_pMyCamera != nullptr)
	{
		m_pMyCamera->SetCamera();

		// 描画処理
		for (int i = 0; i < TYPE::TYPE_MAX; i++)
		{
			if (m_apObj[i] != nullptr)
			{
				m_apObj[i]->SetEnableDisp(true);
				m_apObj[i]->Draw();
				m_apObj[i]->SetEnableDisp(false);
			}
		}
	}

	// 現在のビューポートの取得
	pDevice->SetViewport(&viewportDef);

	// 現在のビューマトリックスの取得
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// 現在のプロジェクションマトリックスの取得
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//=============================================================================
// 生成
//=============================================================================
CMapUI* CMapUI::Create()
{
	if (m_pInstance != nullptr) { return m_pInstance; }
	m_pInstance = new CMapUI;

	if (m_pInstance->Init() == E_FAIL)
	{
		// 開放
		Release();
	}

	return m_pInstance;
}

//=============================================================================
// 開放
//=============================================================================
void CMapUI::Release()
{
	if (m_pInstance == nullptr) { return; }

	// 終了処理
	m_pInstance = nullptr;
}

//=============================================================================
// 回転処理
//=============================================================================
void CMapUI::SetMapRotation()
{
	CGame* pgame = CGame::GetInstance();

	// ゲーム画面か確認
	if (pgame == nullptr) { return; }

	// プレイヤーがいるか確認
	if (m_pMyPlayer == nullptr) { return; }

	// マップオブジェクトの回転取得
	MyLib::Vector3 rot = m_apObj[TYPE::TYPE_MAP]->GetRotation();

	// プレイヤーの進行の割合を求めて回転
	float courselen = pgame->GetCourse()->GetCourceLength();
	float playerlen = m_pMyPlayer->GetMoveLength();
	m_fRatio = playerlen / courselen;
	rot.z = MODEL_ROTZ + (MODEL_ROTMAXMOVEZ * m_fRatio);

	// 設定
	m_apObj[TYPE::TYPE_MAP]->SetRotation(rot);
}
