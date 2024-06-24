//=============================================================================
// 
//  コースエディタ―処理 [edit_waterstone.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "edit_waterstone.h"
#include "course.h"
#include "manager.h"
#include "calculation.h"
#include "map_obstacle.h"
#include "camera.h"
#include "spline.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const MyLib::AABB AABB_LINE = MyLib::AABB(-25.0f, 25.0f);	// 辺のAABB

}

//==========================================================================
// コンストラクタ
//==========================================================================
CEdit_WaterStone::CEdit_WaterStone()
{
	// 値のクリア
	m_nEditIdx = 0;			// 操作するインデックス番号
	m_bEdit = false;		// 操作中判定
	m_bDrag = false;		// 掴み判定
	m_bHoverWindow = false;	// マウスのウィンドウホバー判定
	m_bSetMode = false;		// 設定モード判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CEdit_WaterStone::~CEdit_WaterStone()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CEdit_WaterStone::Init()
{

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CEdit_WaterStone::Uninit()
{
	// 終了処理
	CEdit::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CEdit_WaterStone::Update()
{
	// ウィンドウのマウスホバー判定
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// ファイル操作
	FileControl();

	// 辺の総数変更
	ChangeLineNum();

	// モード変更
	ChangeMode();

	// 基点追加
	AddPoint();

	// ライン選択
	SelectLine();

	// 掴み中
	DragLine();

	// トランスフォーム
	Transform();

	// 最初と最後変形
	TransformBeginEnd();

}

//==========================================================================
// ファイル操作
//==========================================================================
void CEdit_WaterStone::FileControl()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	float width = 150.0f;
	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save"))
	{
		pCourse->Save();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save_as"))
	{
		pCourse->Save();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{

	}
}

//==========================================================================
// 辺の数変更
//==========================================================================
void CEdit_WaterStone::ChangeLineNum()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	ImVec2 imageSize = ImVec2(150, 50);
	if (ImGui::Button("Re : Create", imageSize))
	{
		pCourse->ReCreateVtx();
	}

}

//==========================================================================
// モード変更
//==========================================================================
void CEdit_WaterStone::ChangeMode()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (ImGui::Checkbox("Enable SetMode!!!", &m_bSetMode))
	{
		// 操作判定リセット
		m_bEdit = false;
	}
}

//==========================================================================
// ライン選択
//==========================================================================
void CEdit_WaterStone::SelectLine()
{
	if (m_bSetMode) return;	// セットモードは終わり

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// 辺情報取得
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();
	MyLib::Vector3 coursepos = pCourse->GetPosition();

	// マウス情報
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-25.0f, 25.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		!m_bDrag &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{// 左クリック時

		// 操作判定リセット
		m_bEdit = false;

		int i = 0;
		for (const auto& vtxpos : vecSegmentPos)
		{
			if (i == 0 || i == vecSegmentPos.size() - 1)
			{
				i++;
				continue;
			}
			// マトリックス初期化
			mtx.Identity();
			mtxTrans.Identity();

			// 位置情報反映
			MyLib::Vector3 transpos = vtxpos + coursepos;
			mtxTrans.Translation(transpos);
			mtx.Multiply(mtx, mtxTrans);

			MyLib::Vector3 OBpos;

			if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
			{// 重なり && 左クリック

				// 操作する辺保存
				m_nEditIdx = i;

				m_bEdit = true;
				break;
			}
			i++;
		}

		// 色リセット
		for (int i = 0; i < vecSegmentPos.size(); i++)
		{
			CCollisionLine_Box* pBox = pCourse->GetCollisionLineBox(i);
			if (pBox == nullptr) continue;

			pBox->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//==========================================================================
// ライン掴み
//==========================================================================
void CEdit_WaterStone::DragLine()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (!m_bEdit) return;
	if (m_bSetMode) return;	// セットモードは終わり

	// 辺情報取得
	MyLib::Vector3 segmentPos = pCourse->GetVecPosition(m_nEditIdx);
	MyLib::Vector3 coursepos = pCourse->GetPosition();

	// マウス情報
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-50.0f, 50.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		pMouse->GetPress(CInputMouse::BUTTON::BUTTON_LEFT))
	{// 左押し込み時

		// マトリックス初期化
		mtx.Identity();
		mtxTrans.Identity();

		// 位置情報反映
		MyLib::Vector3 transpos = segmentPos + coursepos;
		mtxTrans.Translation(transpos);
		mtx.Multiply(mtx, mtxTrans);

		MyLib::Vector3 OBpos;

		if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
		{// 重なり
			m_bDrag = true;
		}

	}

	if (pMouse->GetRelease(CInputMouse::BUTTON::BUTTON_LEFT))
	{
		m_bDrag = false;
	}

	// ドラッグ中
	if (m_bDrag)
	{
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera == nullptr) return;

		// 再移動中
		MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();
		segmentPos.x += diffpos.x;
		segmentPos.z += diffpos.z;
	}

	// 頂点データ設定
	pCourse->SetVecPosition(m_nEditIdx, segmentPos);
}

//==========================================================================
// トランスフォーム
//==========================================================================
void CEdit_WaterStone::Transform()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// 辺情報取得
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();
	if (m_bEdit)
	{
		for (int i = 0; i < vecSegmentPos.size(); i++)
		{
			CCollisionLine_Box* pBox = pCourse->GetCollisionLineBox(i);
			if (pBox == nullptr) continue;

			D3DXCOLOR col = (i == m_nEditIdx) ? D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) : D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pBox->SetColor(col);
		}
	}

	// 操作する辺の情報
	MyLib::Vector3 editpos = pCourse->GetVecPosition(m_nEditIdx);

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Transform"))
	{
		float windowWidth = 100.0f;
		const float  ROT_MOVE = D3DX_PI * 0.01f;
		const float  POS_MOVE = 0.5f;

		if (!m_bEdit)
		{
			ImGui::Text(":None Obj:");
			ImGui::TreePop();
			return;
		}

		ImGui::Text("NowEdit : [ %d ]", m_nEditIdx);
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		// リセット
		if (ImGui::Button("RESET")) {
			editpos = 0.0f;
		}
		

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		//=============================
		// 位置設定
		//=============================
		ImGui::Text("pos");
		ImGui::SameLine();

		// X
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &editpos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &editpos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &editpos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		////=============================
		//// 向き設定
		////=============================
		//ImGui::Text("rot");
		//ImGui::SameLine();

		//// X
		//ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("x", &editInfo.rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();

		//// Y
		//ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("y", &editInfo.rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();

		//// Z
		//ImGui::PushID(1); // ウィジェットごとに異なるIDを割り当てる
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("z", &editInfo.rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//}
		//ImGui::PopID();




		////=============================
		//// 幅設定
		////=============================
		//ImGui::Text("width");
		//ImGui::SameLine();

		//// X
		//ImGui::PushID(2); // ウィジェットごとに異なるIDを割り当てる
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("width", &editInfo.width, POS_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();


		ImGui::TreePop();
	}

	// ライン情報設定
	pCourse->SetVecPosition(m_nEditIdx, editpos);
}

//==========================================================================
// 地点追加
//==========================================================================
void CEdit_WaterStone::AddPoint()
{
	if (!m_bSetMode) return;	// セットモード以外は終わり

	ImGui::Text("Push Mouse Button!!!");

	// マウス情報
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	// カメラ情報
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	MyLib::Vector3 pos = UtilFunc::Transformation::CalcScreenToXZ(
		pMouse->GetPosition(),
		D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
		pCamera->GetMtxView(),
		pCamera->GetMtxProjection());

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// 基点情報取得
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();

	if (!m_bHoverWindow &&
		!CInputKeyboard::GetInstance()->GetPress(DIK_LALT) &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{

		int endIdx = (vecSegmentPos.size() - 1);
		vecSegmentPos.insert(vecSegmentPos.begin() + endIdx, pos);

		// 基点情報設定
		pCourse->SetVecPosition(vecSegmentPos);
		pCourse->ReCreateVtx();
	}
}

//==========================================================================
// 最初と最後変形
//==========================================================================
void CEdit_WaterStone::TransformBeginEnd()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// 辺情報取得
	std::vector<MyLib::Vector3> segmentPos = pCourse->GetVecPosition();

	// 最初と最後、逆方向に少し出す
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// 最初
	angle = segmentPos[2].AngleXZ(segmentPos[1]);
	begin = MyLib::Vector3(
		segmentPos[1].x + sinf(angle) * -10.0f,
		segmentPos[1].y,
		segmentPos[1].z + cosf(angle) * -10.0f);

	// 最後
	int endIdx = (segmentPos.size() - 1) - 1;
	angle = segmentPos[endIdx].AngleXZ(segmentPos[endIdx - 1]);
	end = MyLib::Vector3(
		segmentPos[endIdx].x + sinf(angle) * 10.0f,
		segmentPos[endIdx].y,
		segmentPos[endIdx].z + cosf(angle) * 10.0f);

	segmentPos[0] = begin;
	segmentPos[(segmentPos.size() - 1)] = end;

	pCourse->SetVecPosition(segmentPos);
}

