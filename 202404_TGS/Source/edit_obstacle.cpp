//=============================================================================
// 
//  ��Q���G�f�B�^�\���� [edit_obstacle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_obstacle.h"
#include "manager.h"
#include "calculation.h"
#include "collisionLine_Box.h"
#include "map_obstacleManager.h"
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
	m_nEditIdx = 0;		// ��������C���f�b�N�X	
	m_nColliderIdx = 0;	// ��������R���C�_�[�̃C���f�b�N�X
	m_pObjX.clear();	// �I�u�W�F�N�gX�̃|�C���^
	m_pCollisionLineBox = nullptr;
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
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	std::vector<CMap_ObstacleManager::SObstacleInfo> vecInfo = pObstacleMgr->GetObstacleInfo();

	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 300.0f, 0.0f);
	for (const auto& info : vecInfo)
	{
		CObjectX* pObj = CObjectX::Create(info.modelFile, pos);
		m_pObjX.push_back(pObj);

		pos.x += DISTANCE_OBJ;
	}

	// �����蔻��{�b�N�X����
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);
	m_AABB.vtxMax = info.boxcolliders[0].vtxMax;
	m_AABB.vtxMin = info.boxcolliders[0].vtxMin;
	m_pCollisionLineBox = CCollisionLine_Box::Create(m_AABB, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	return S_OK;
}

//==========================================================================
// �I������
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

	// �I������
	CEdit::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Obstacle::Update()
{

	// �G�f�B�b�g���j���[
	ImGui::Begin("Obstacle Edit", NULL, ImGuiWindowFlags_MenuBar);
	{
		MenuBar();

		//***********************
		// ���݂̃L�[�ύX
		//***********************
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::SetNextItemWidth(140.0f);

		// [�X���C�_�[]��������C���f�b�N�X
		if (ImGui::SliderInt("Edit Idx", &m_nEditIdx, 0, m_pObjX.size() - 1))
		{

		}

		Resize();
	}
	ImGui::End();



	// �J�����̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->SetTargetPosition(m_pObjX[m_nEditIdx]->GetPosition());
	
}

//==========================================================================
// ���j���[�o�[����
//==========================================================================
void CEdit_Obstacle::MenuBar()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();

	// �����o��
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
			strDataDir += "\\data\\TEXT\\map";

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
		ImGui::EndMenu();
	}

	// ���[�h
	if (ImGui::BeginMenu("Load"))
	{

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
}

//==========================================================================
// ���T�C�Y
//==========================================================================
void CEdit_Obstacle::Resize()
{
	// ��Q���}�l�[�W���擾
	CMap_ObstacleManager* pObstacleMgr = CMap_ObstacleManager::GetInstance();
	CMap_ObstacleManager::SObstacleInfo info = pObstacleMgr->GetObstacleInfo(m_nEditIdx);


	// [�X���C�_�[]��������R���C�_�[�̃C���f�b�N�X
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	ImGui::SetNextItemWidth(140.0f);
	if (ImGui::SliderInt("Collider Idx", &m_nColliderIdx, 0, info.boxcolliders.size() - 1))
	{

	}

	float windowWidth = 100.0f;
	const float  POS_MOVE = 0.5f;

	// BOX�R���C�_�[
	MyLib::Collider_BOX collider = info.boxcolliders[m_nColliderIdx];

	// ���Z�b�g
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

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	{
		ImGui::Text("VtxMin");

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

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	{
		ImGui::Text("Offset");

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
	m_AABB.vtxMax = collider.vtxMax;
	m_AABB.vtxMin = collider.vtxMin;
	m_pCollisionLineBox->SetAABB(m_AABB);


	// BOX�R���C�_�[�ݒ�
	info.boxcolliders[m_nColliderIdx] = collider;
	pObstacleMgr->SetObstacleInfo(info, m_nEditIdx);
}
