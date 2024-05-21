//=============================================================================
// 
//  障害物エディタ―処理 [edit_obstacle.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit_obstacle.h"
#include "manager.h"
#include "calculation.h"
#include "collisionLine_Box.h"
#include "map_obstacleManager.h"
#include "camera.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float DISTANCE_OBJ = 500.0f;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_Obstacle::CEdit_Obstacle()
{
	// 値のクリア
	m_nEditIdx = 0;		// 調整するインデックス	
	m_nColliderIdx = 0;	// 調整するコライダーのインデックス
	m_pObjX.clear();	// オブジェクトXのポインタ
	m_pCollisionLineBox = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_Obstacle::~CEdit_Obstacle()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_Obstacle::Init()
{
	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();

	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 300.0f, 0.0f);
	for (const auto& info : vecInfo)
	{
		CObjectX* pObj = CObjectX::Create(info.modelFile, pos);
		m_pObjX.push_back(pObj);

		pos.x += DISTANCE_OBJ;
	}

	// 当たり判定ボックス生成
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);
	m_AABB.vtxMax = info.boxcolliders[0].vtxMax;
	m_AABB.vtxMin = info.boxcolliders[0].vtxMin;
	m_pCollisionLineBox = CCollisionLine_Box::Create(m_AABB, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit_Obstacle::Uninit()
{
	for (const auto& obj : m_pObjX)
	{
		obj->Kill();
	}
	m_pObjX.clear();

	m_pCollisionLineBox->Kill();
	m_pCollisionLineBox = nullptr;

	// 終了処理
	CEdit::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_Obstacle::Update()
{

	// エディットメニュー
	ImGui::Begin("Obstacle Edit", NULL, ImGuiWindowFlags_MenuBar);
	{
		MenuBar();

		//***********************
		// 現在のキー変更
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(140.0f);

		// [スライダー]調整するインデックス
		if (ImGui::SliderInt("Edit Idx", &m_nEditIdx, 0, m_pObjX.size() - 1))
		{

		}

		Resize();
	}
	ImGui::End();



	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetTargetPosition(m_pObjX[m_nEditIdx]->GetPosition());
	
}

//==========================================================================
// メニューバー処理
//==========================================================================
void CEdit_Obstacle::MenuBar()
{
	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// 書き出し
	ImGui::BeginMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save"))
		{
			pObstacleMgr->SaveInfo();
		}

		if (ImGui::MenuItem("Save_as"))
		{
			OPENFILENAMEA filename = {};
			char sFilePass[1024] = {};
			// ファイル選択ダイアログの設定
			filename.lStructSize = sizeof(OPENFILENAMEA);
			filename.hwndOwner = NULL;
			filename.lpstrFilter = "テキストファイル\0*.txt\0画像ファイル\0*.bmp;.jpg\0すべてのファイル\0.*\0\0";
			filename.lpstrFile = sFilePass;
			filename.nMaxFile = MAX_PATH;
			filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;


			// カレントディレクトリを取得する
			char szCurrentDir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

			// "data"フォルダの絶対パスを求める
			std::string strDataDir = szCurrentDir;
			strDataDir += "\\data\\TEXT\\map";

			// 存在する場合は、lpstrInitialDirに指定する
			if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				filename.lpstrInitialDir = strDataDir.c_str();
			}


			// ファイル選択ダイアログを表示
			if (GetOpenFileNameA(&filename))
			{
				// 選択されたファイルのパスを表示
				printf("Selected file: %s\n", sFilePass);
				int n = 0;
			}
			//セーブ
			if (strcmp(&sFilePass[0], "") != 0) {
				int n = 0;
			}
		}
		ImGui::EndMenu();
	}

	// ロード
	if (ImGui::BeginMenu("Load"))
	{

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
}

//==========================================================================
// リサイズ
//==========================================================================
void CEdit_Obstacle::Resize()
{
	// 障害物マネージャ取得
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);


	// [スライダー]調整するコライダーのインデックス
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::SetNextItemWidth(140.0f);
	if (ImGui::SliderInt("Collider Idx", &m_nColliderIdx, 0, info.boxcolliders.size() - 1))
	{

	}

	float windowWidth = 100.0f;
	const float  POS_MOVE = 0.5f;

	// BOXコライダー
	MyLib::Collider_BOX collider = info.boxcolliders[m_nColliderIdx];

	// リセット
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::Button("ALL RESET")) {
		collider.vtxMin = 0.0f;
		collider.vtxMax = 0.0f;
		collider.offset = 0.0f;
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	{
		ImGui::Text("VtxMax");

		// MaxX
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &collider.vtxMax.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// MaxY
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &collider.vtxMax.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// MaxZ
		ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &collider.vtxMax.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	{
		ImGui::Text("VtxMin");

		// MinX
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &collider.vtxMin.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// MinY
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &collider.vtxMin.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// MinZ
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &collider.vtxMin.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();
	}

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	{
		ImGui::Text("Offset");

		// X
		ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &collider.offset.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &collider.offset.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &collider.offset.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();
	}

	// BOX設定
	m_AABB.vtxMax = collider.vtxMax;
	m_AABB.vtxMin = collider.vtxMin;
	m_pCollisionLineBox->SetAABB(m_AABB);


	// BOXコライダー設定
	info.boxcolliders[m_nColliderIdx] = collider;
	pObstacleMgr->SetObstacleInfo(info, m_nEditIdx);
}
