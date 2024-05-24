//=============================================================================
// 
//  ��Q���G�f�B�^�\���� [edit_obstacle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_obstacle.h"
#include "manager.h"
#include "calculation.h"
#include "map_obstacle.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const float DISTANCE_OBJ = 500.0f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_Obstacle::CEdit_Obstacle()
{
	// �l�̃N���A
	m_editType = EditType::TYPE_COLLIDER;
	m_pEditControl = nullptr;	// ��ޑ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_Obstacle::~CEdit_Obstacle()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_Obstacle::Init()
{
	// ����ύX
	ChangeMode(EditType::TYPE_COLLIDER);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_Obstacle::Uninit()
{
	// �I������
	CEdit::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Obstacle::Update()
{
	// �R���{�{�b�N�X
	static const char* items[] = { "Arrangement", "Collider" };
	static int select = 1;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::Combo(": Mode", &select, items, IM_ARRAYSIZE(items)))
	{
		ChangeMode(static_cast<EditType>(select));
	}


	// �G�f�B�b�g�X�V
	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	m_pEditControl->Update();
}

//==========================================================================
// ���[�h�ύX
//==========================================================================
void CEdit_Obstacle::ChangeMode(EditType type)
{
	if (m_pEditControl != nullptr)
	{
		m_pEditControl->Uninit();
		m_pEditControl = nullptr;
	}

	switch (type)
	{
	case CEdit_Obstacle::TYPE_ARRANGMENT:
		m_pEditControl = DEBUG_NEW CEdit_Obstacle_Arrangment;
		break;

	case CEdit_Obstacle::TYPE_COLLIDER:
		m_pEditControl = DEBUG_NEW CEdit_Obstacle_Collider;
		break;
	}
	m_pEditControl->Init();
}

//==========================================================================
// ������
//==========================================================================
void CEdit_Obstacle_Arrangment::Init()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	m_ObstacleInfo = pObstacleMgr->GetObstacleInfo(0);

	// �����蔻��{�b�N�X����
	CreateBoxLine();

}

//==========================================================================
// �����蔻��{�b�N�X����
//==========================================================================
void CEdit_Obstacle_Arrangment::CreateBoxLine()
{
	// �����蔻��{�b�N�X�폜
	DeleteBoxLine();

	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		for (auto& boxcollider : pObj->GetObstacleInfo().boxcolliders)
		{
			// AABB�ݒ�
			MyLib::AABB aabb = MyLib::AABB(boxcollider.vtxMin, boxcollider.vtxMax);

			// ��������
			CCollisionLine_Box* pBox;
			pBox = CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			m_pCollisionLineBox.push_back(pBox);

			// �ʒu�ݒ�
			boxcollider.TransformOffset(pObj->GetWorldMtx());
			m_pCollisionLineBox.back()->SetPosition(boxcollider.GetMtx().GetWorldPosition());
		}
	}

}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Obstacle_Arrangment::Update()
{
	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);



	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();

	std::vector<std::string> items;
	std::vector<const char*> items_c_str;
	for (const auto& info : vecInfo)
	{
		std::string file = UtilFunc::Transformation::RemoveFilePath(info.modelFile);
		items.push_back(file);
		items_c_str.push_back(items.back().c_str());
	}

	static int select = 0;

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::Combo("SetType", &select, items_c_str.data(), items_c_str.size()))
	{
		// ��Q�����ݒ�
		m_ObstacleInfo = vecInfo[select];
	}


	ImGuiDragDropFlags src_flags = 0;
	src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
	src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
	//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip

	ImVec2 imageSize = ImVec2(150, 50);

	// �h���b�O�\�ȗv�f�̕`��
	ImGui::Button(": Drag Me! :", imageSize);

	// �h���b�O���e�L�X�g�`��
	if (ImGui::BeginDragDropSource(src_flags))
	{
		std::string file = UtilFunc::Transformation::RemoveFilePath(m_ObstacleInfo.modelFile);
		ImGui::Text(file.c_str());
		ImGui::EndDragDropSource();
	}

	if (ImGui::IsItemHovered() &&
		ImGui::IsMouseDown(0))
	{// UI��ɃJ�[�\�� && �N���b�N
		m_bButtonDrag = true;
	}

	// �����ڂ̂ݔz�u
	if (m_bButtonDrag &&
		!m_bHoverWindow)
	{// �h���b�O�� && �E�B���h�E�O

		if (m_pDragObj != nullptr) {
			m_pDragObj->Kill();
			m_pDragObj = nullptr;
		}

		// ����
		m_pDragObj = CObjectX::Create(m_ObstacleInfo.modelFile);
		m_pDragObj->SetType(CObject::TYPE::TYPE_OBJECTX);
	}


	// �}�E�X���擾
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();

	if (m_pDragObj != nullptr) {

		MyLib::Vector3 setpos = mouseWorldPos;
		setpos.y = 0.0f;

		m_pDragObj->SetPosition(setpos);
	}

	// �z�u
	if (m_bButtonDrag &&
		ImGui::IsMouseReleased(0))
	{// �͂ݒ� && �}�E�X�����[�X

		if (m_pDragObj != nullptr) {
			m_pDragObj->Kill();
			m_pDragObj = nullptr;
		}

		if (!m_bHoverWindow) {

			CInputMouse* pMouse = CInputMouse::GetInstance();
			MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();

			MyLib::Vector3 setpos = mouseWorldPos;
			setpos.y = 0.0f;

			CMap_Obstacle* pObj = CMap_Obstacle::Create(m_ObstacleInfo);
			pObj->SetPosition(setpos);
			pObj->CalWorldMtx();

			CreateBoxLine();
		}
		m_bButtonDrag = false;
	}



}






//==========================================================================
// ������
//==========================================================================
void CEdit_Obstacle_Collider::Init()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();

	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 1000.0f, 0.0f);
	for (const auto& info : vecInfo)
	{
		CObjectX* pObj = CObjectX::Create(info.modelFile, pos);
		pObj->SetType(CObject::TYPE::TYPE_OBJECTX);

		m_pObjX.push_back(pObj);

		pos.x += DISTANCE_OBJ;
	}

	// �����蔻��BOX����
	CreateBoxLine();
}

//==========================================================================
// �����蔻��{�b�N�X����
//==========================================================================
void CEdit_Obstacle_Collider::CreateBoxLine()
{
	// �����蔻��{�b�N�X�폜
	DeleteBoxLine();

	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// �����蔻��{�b�N�X�擾
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	for (auto& boxcollider : info.boxcolliders)
	{
		// AABB�ݒ�
		MyLib::AABB aabb = MyLib::AABB(boxcollider.vtxMin, boxcollider.vtxMax);

		// ��������
		CCollisionLine_Box* pBox;
		pBox = CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_pCollisionLineBox.push_back(pBox);

		// �ʒu�ݒ�
		boxcollider.TransformOffset(m_pObjX[m_nEditIdx]->GetWorldMtx());
		m_pCollisionLineBox.back()->SetPosition(boxcollider.GetMtx().GetWorldPosition());
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Obstacle_Collider::Update()
{
	// �G�f�B�b�g���j���[
	if (ImGui::TreeNode("Obstacle Edit"))
	{
		MenuBar();

		//***********************
		// ���݂̃L�[�ύX
		//***********************
		// [�X���C�_�[]��������C���f�b�N�X
		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		// ���T�C�Y
		if (ImGui::TreeNode("Target"))
		{
			ImGui::SeparatorText("ChangeTarget Info");

			ImGui::SetNextItemWidth(140.0f);
			if (ImGui::SliderInt("Edit Idx", &m_nEditIdx, 0, m_pObjX.size() - 1))
			{

			}
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TreePop();
		}


		if (ImGui::TreeNode("Collider"))
		{
			ImGui::SeparatorText("ChangeCollider Info");

			// ��Q���}�l�[�W���擾
			CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
			CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

			// �����ύX
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Change Coolider Num:");
			ImGui::SameLine();
			if (ImGui::ArrowButton("##left", ImGuiDir_Left))
			{
				pObstacleMgr->SubCollider(m_nEditIdx);
				CreateBoxLine();
			}
			ImGui::SameLine(0.0f);
			if (ImGui::ArrowButton("##right", ImGuiDir_Right))
			{
				pObstacleMgr->AddCollider(m_nEditIdx);
				CreateBoxLine();
			}
			ImGui::SameLine();
			ImGui::Text("%d", info.boxcolliders.size());

			Resize();
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}



	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetTargetPosition(m_pObjX[m_nEditIdx]->GetPosition());

	for (int i = 0; i < 4; i++)
	{
		CEffect3D* pEffect = CEffect3D::Create(
			m_pObjX[m_nEditIdx]->GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE::TYPE_BLACK);
		pEffect->SetDisableZSort();
	}

	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	for (int i = 0; i < static_cast<int>(info.boxcolliders.size()); i++)
	{
		// BOX�R���C�_�[
		MyLib::Collider_BOX collider = info.boxcolliders[i];

		collider.TransformOffset(m_pObjX[m_nEditIdx]->GetWorldMtx());
		m_pCollisionLineBox[i]->SetPosition(collider.GetMtx().GetWorldPosition());
	}
}

//==========================================================================
// ���j���[�o�[����
//==========================================================================
void CEdit_Obstacle_Collider::MenuBar()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();


	// �R���{�{�b�N�X
	static const char* items[] = { "Save", "Save_as", "Load" };
	int select = 0;
	if (ImGui::Combo(": Save & Load", &select, items, IM_ARRAYSIZE(items)))
	{
		// �I�����ꂽ���ڂ��ύX���ꂽ�Ƃ��̏���
		switch (select)
		{
		case 0:	// Save
		{
			pObstacleMgr->SaveInfo();
		}
			break;

		case 1:	// Save_as
		{
			OPENFILENAMEA filename = {};
			char sFilePass[1024] = {};
			// �t�@�C���I���_�C�A���O�̐ݒ�
			filename.lStructSize = sizeof(OPENFILENAMEA);
			filename.hwndOwner = NULL;
			filename.lpstrFilter = "�e�L�X�g�t�@�C��\0*.txt\0�摜�t�@�C��\0*.bmp;.jpg\0���ׂẴt�@�C��\0.*\0\0";
			filename.lpstrFile = sFilePass;
			filename.nMaxFile = MAX_PATH;
			filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;


			// �J�����g�f�B���N�g�����擾����
			char szCurrentDir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

			// "data"�t�H���_�̐�΃p�X�����߂�
			std::string strDataDir = szCurrentDir;
			strDataDir += "\\data\\TEXT\\mapobstacle";

			// ���݂���ꍇ�́AlpstrInitialDir�Ɏw�肷��
			if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				filename.lpstrInitialDir = strDataDir.c_str();
			}


			// �t�@�C���I���_�C�A���O��\��
			if (GetOpenFileNameA(&filename))
			{
				// �I�����ꂽ�t�@�C���̃p�X��\��
				printf("Selected file: %s\n", sFilePass);
				int n = 0;
			}
			//�Z�[�u
			if (strcmp(&sFilePass[0], "") != 0) {
				int n = 0;
			}
		}
			break;

		case 2:	// Load
			
			break;
		}
	}

#if 0
	// �����o��
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
			// �t�@�C���I���_�C�A���O�̐ݒ�
			filename.lStructSize = sizeof(OPENFILENAMEA);
			filename.hwndOwner = NULL;
			filename.lpstrFilter = "�e�L�X�g�t�@�C��\0*.txt\0�摜�t�@�C��\0*.bmp;.jpg\0���ׂẴt�@�C��\0.*\0\0";
			filename.lpstrFile = sFilePass;
			filename.nMaxFile = MAX_PATH;
			filename.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;


			// �J�����g�f�B���N�g�����擾����
			char szCurrentDir[MAX_PATH];
			GetCurrentDirectoryA(MAX_PATH, szCurrentDir);

			// "data"�t�H���_�̐�΃p�X�����߂�
			std::string strDataDir = szCurrentDir;
			strDataDir += "\\data\\TEXT\\mapobstacle";

			// ���݂���ꍇ�́AlpstrInitialDir�Ɏw�肷��
			if (GetFileAttributesA(strDataDir.c_str()) != INVALID_FILE_ATTRIBUTES)
			{
				filename.lpstrInitialDir = strDataDir.c_str();
			}


			// �t�@�C���I���_�C�A���O��\��
			if (GetOpenFileNameA(&filename))
			{
				// �I�����ꂽ�t�@�C���̃p�X��\��
				printf("Selected file: %s\n", sFilePass);
				int n = 0;
			}
			//�Z�[�u
			if (strcmp(&sFilePass[0], "") != 0) {
				int n = 0;
			}
		}

		// ���[�h
		if (ImGui::MenuItem("Load"))
		{

		}

		ImGui::EndMenu();
	}
#endif

}

//==========================================================================
// ���T�C�Y
//==========================================================================
void CEdit_Obstacle_Collider::Resize()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);

	// [�X���C�_�[]��������R���C�_�[�̃C���f�b�N�X
	ImGui::SetNextItemWidth(140.0f);
	if (ImGui::SliderInt("Collider Idx", &m_nColliderIdx, 0, info.boxcolliders.size() - 1))
	{

	}

	float windowWidth = 100.0f;
	const float  POS_MOVE = 0.5f;

	ImGui::Dummy(ImVec2(0.0f, 5.0f));
	// ���T�C�Y
	if (ImGui::TreeNode("Resize"))
	{
		ImGui::SeparatorText("ChangeAABB Info");

		// BOX�R���C�_�[
		MyLib::Collider_BOX collider = info.boxcolliders[m_nColliderIdx];

		// ���Z�b�g
		if (ImGui::Button("ALL RESET")) {
			collider.vtxMin = 0.0f;
			collider.vtxMax = 0.0f;
			collider.offset = 0.0f;
		}

		if (ImGui::Button("VtxMax RESET")) {
			collider.vtxMax = 0.0f;
		}
		ImGui::SameLine();

		if (ImGui::Button("VtxMin RESET")) {
			collider.vtxMin = 0.0f;
		}
		ImGui::SameLine();

		if (ImGui::Button("Offset RESET")) {
			collider.offset = 0.0f;
		}

		ImGui::Dummy(ImVec2(0.0f, 5.0f));
		{
			ImGui::Text("VtxMax");
			ImGui::SameLine();
			// MaxX
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.vtxMax.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MaxY
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.vtxMax.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MaxZ
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.vtxMax.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		{
			ImGui::Text("VtxMin");
			ImGui::SameLine();
			// MinX
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.vtxMin.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MinY
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.vtxMin.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// MinZ
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.vtxMin.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		{

			ImGui::Text("Offset");
			ImGui::SameLine();
			// X
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &collider.offset.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &collider.offset.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &collider.offset.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();
		}

		// BOX�ݒ�
		MyLib::AABB aabb = MyLib::AABB(collider.vtxMin, collider.vtxMax);
		m_pCollisionLineBox[m_nColliderIdx]->SetAABB(aabb);


		// BOX�R���C�_�[�ݒ�
		info.boxcolliders[m_nColliderIdx] = collider;
		pObstacleMgr->SetObstacleInfo(info, m_nEditIdx);

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::TreePop();
	}
}
