//=============================================================================
// 
//  �}�b�v�G�f�B�^�\���� [edit_map.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_map.h"
#include "manager.h"
#include "calculation.h"
#include "collisionLine_Box.h"
#include "camera.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* LOADTEXT = "data\\TEXT\\map\\info.txt";		// �}�b�v�̃t�@�C��
	const char* SAVETEXT = "data\\TEXT\\map\\save_info.txt";
	const char* TEXTURE_HANDLE[] =							// �e�N�X�`���̃t�@�C��
	{
		"data\\TEXTURE\\handle\\move.png",
		"data\\TEXTURE\\handle\\scale.png",
		"data\\TEXTURE\\handle\\rotation.png",
	};
}
CListManager<CObjectX> CEdit_Map::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_Map::CEdit_Map()
{
	// �l�̃N���A
	m_nModelIdx.clear();	// ���f���C���f�b�N�X
	m_ModelFile.clear();	// ���f���t�@�C��
	m_pObjX.clear();		// �I�u�W�F�N�gX�̃|�C���^
	m_bGrab = false;		// �͂ݔ���
	m_bReGrab = false;		// �Ē͂ݔ���
	m_pCopyObj = nullptr;	// �R�s�[�I�u�W�F�N�g
	m_pGrabObj = nullptr;	// �͂݃I�u�W�F�N�g
	m_pHandle = nullptr;	// �ړ��n���h��
	m_HandleType = CHandle::HandleType::TYPE_MOVE;	// �n���h���̎��
	m_moveAngle = CHandle::HandleAngle::ANGLE_X;	// �ړ��̌���

	memset(m_HandleTex, 0, sizeof(m_HandleTex));// �e�N�X�`���̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_Map::~CEdit_Map()
{

}

//==========================================================================
// ��������
//==========================================================================
CEdit_Map* CEdit_Map::Create()
{
	// �������̊m��
	CEdit_Map* pMarker = DEBUG_NEW CEdit_Map;
	if (pMarker != nullptr)
	{
		// ����������
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_Map::Init()
{
	Load();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevive = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �摜�̃��[�h
	for (const auto& file : m_ModelFile)
	{
		m_pTexture.emplace_back();

		HRESULT hr = D3DXCreateTextureFromFileEx(pDevive, file.c_str(), 0, 0, 0, 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
			NULL, NULL, &m_pTexture.back());
	}

	int max = static_cast<int>(CHandle::HandleType::TYPE_MAX);
	for (int i = 0; i < max; i++)
	{
		D3DXCreateTextureFromFileEx(pDevive, TEXTURE_HANDLE[i], 0, 0, 0, 0, D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_ARGB(255, 255, 255, 255),
			NULL, NULL, &m_HandleTex[i]);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_Map::Uninit()
{
	for (const auto& texture : m_pTexture)
	{
		texture->Release();
	}
	m_pTexture.clear();

	int max = static_cast<int>(CHandle::HandleType::TYPE_MAX);
	for (int i = 0; i < max; i++)
	{
		if (m_HandleTex[i] == nullptr) {
			continue;
		}

		m_HandleTex[i]->Release();
		m_HandleTex[i] = nullptr;
	}

	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Map::Update()
{
	ImGui::Begin("Edit", NULL, ImGuiWindowFlags_MenuBar);

	// �����o��
	ImGui::BeginMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Save"))
		{
			Save();
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
			strDataDir += "\\data";

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

	/*if (ImGui::BeginMenu("Load"))
	{

		ImGui::EndMenu();
	}*/

	ImGui::EndMenuBar();


	ImGuiDragDropFlags src_flags = 0;
	src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
	src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging
	//src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip; // Hide the tooltip

	ImVec2 imageSize = ImVec2(50, 50);
	for (int i = 0; i < static_cast<int>(m_nModelIdx.size()); i++)
	{
		ImGui::PushID(i); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			if (i % 5 != 0) {
				ImGui::SameLine();
			}

			// �h���b�O�\�ȗv�f�̕`��
			ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_pTexture[i]), imageSize);

			if (ImGui::BeginDragDropSource(src_flags))
			{
				SDragDropData dragData;
				dragData.nType = m_nModelIdx[i];
				m_DragData.nType = m_nModelIdx[i];

				ImGui::SetDragDropPayload("MY_COORDINATE_TYPE", &dragData, sizeof(SDragDropData));
				ImGui::Text(m_ModelFile[i].c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::IsItemHovered() &&
				ImGui::IsMouseDown(0))
			{// UI��ɃJ�[�\�� && �N���b�N
				m_bGrab = true;
			}
		}
		ImGui::PopID();
	}


	ImGui::End();

	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	bool bHoverWindow = ImGui::IsWindowHovered(frag);


	CInputKeyboard* pKeyboard = CInputKeyboard::GetInstance();
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();
	MyLib::Vector3 mouseWorldPos = pMouse->GetWorldPosition();
	MyLib::Vector3 mouseOldWorldPos = pMouse->GetOldWorldPosition();





	//=============================
	// Imgui�ݒ�
	//=============================
	if (m_pGrabObj != nullptr) {

		ImGui::Begin("Transform");
		//ImGui::Begin("Transform", NULL, ImGuiWindowFlags_MenuBar);


		// �ړ��{�^��
		if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_HandleTex[CHandle::HandleType::TYPE_MOVE]), imageSize))
		{
			m_HandleType = CHandle::HandleType::TYPE_MOVE;
			ChangeHandle();
		}
		ImGui::SameLine();

		// �X�P�[���{�^��
		if(ImGui::ImageButton(reinterpret_cast<ImTextureID>(m_HandleTex[CHandle::HandleType::TYPE_SCALE]), imageSize))
		{
			m_HandleType = CHandle::HandleType::TYPE_SCALE;
			ChangeHandle();
		}
		ImGui::SameLine();


		if (!bHoverWindow) {
			bHoverWindow = ImGui::IsWindowHovered(frag);
		}

		//=============================
		// �����ݒ�
		//=============================
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		float windowWidth = 100.0f;
		const float  ROT_MOVE = D3DX_PI * 0.01f;
		const float  POS_MOVE = 0.5f;

		//if (ImGui::CollapsingHeader("Transform"))
		{
			//// �`�F�b�N�{�b�N�X��\��
			//unsigned int checkboxFlags = 0;
			//static bool readOnlyCheckboxState = false; // �`�F�b�N�{�b�N�X�̓ǂݎ���p���

			////ImGui::CheckboxFlags("�`�F�b�N�{�b�N�X", &checkboxFlags, 1 << 0, ImGuiCheckboxFlags_No ImGuiCheckboxFlags_NoCheckOnClick);


			//// �`�F�b�N�{�b�N�X��\�����A�ǂݎ���p��Ԃł���Ζ���������
			//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, readOnlyCheckboxState); // �ǂݎ���p��ԂȂ�`�F�b�N�{�b�N�X�𖳌���
			//ImGui::Checkbox("�ǂݎ���p�`�F�b�N�{�b�N�X", &readOnlyCheckboxState);
			//ImGui::PopItemFlag(); // �`�F�b�N�{�b�N�X�̖�����������


			// ���Z�b�g
			if (ImGui::Button("ALL RESET")) {
				m_pGrabObj->SetPosition(0.0f);
				m_pGrabObj->SetRotation(0.0f);
				m_pGrabObj->SetScale(1.0f);
			}
			ImGui::SameLine();
			if (ImGui::Button("POS RESET")) {
				m_pGrabObj->SetPosition(0.0f);
			}
			ImGui::SameLine();
			if (ImGui::Button("ROT RESET")) {
				m_pGrabObj->SetRotation(0.0f);
			}
			ImGui::SameLine();
			if (ImGui::Button("SCALE RESET")) {
				m_pGrabObj->SetScale(1.0f);
			}

			//=============================
			// �ʒu�ݒ�
			//=============================
			MyLib::Vector3 pos = m_pGrabObj->GetPosition();

			// X
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// �ʒu�ݒ�
			m_pGrabObj->SetPosition(pos);
			if (m_pGrabObj->GetCollisionLineBox() != nullptr) {
				m_pGrabObj->GetCollisionLineBox()->SetPosition(pos);
			}
			if (m_pHandle != nullptr) {
				m_pHandle->SetPosition(m_pGrabObj->GetPosition());
			}


			//=============================
			// �����ݒ�
			//=============================
			MyLib::Vector3 rot = m_pGrabObj->GetRotation();

			// X
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// �����ݒ�
			UtilFunc::Transformation::RotNormalize(rot);
			m_pGrabObj->SetRotation(rot);


			//=============================
			// �g�k�ݒ�
			//=============================
			MyLib::Vector3 scale = m_pGrabObj->GetScale();

			// X
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("x", &scale.x, 0.01f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Y
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("y", &scale.y, 0.01f, 0.0f, 0.0f, "%.2f");
				ImGui::SameLine();
			}
			ImGui::PopID();

			// Z
			ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
			{
				ImGui::SetNextItemWidth(windowWidth);
				ImGui::DragFloat("z", &scale.z, 0.01f, 0.0f, 0.0f, "%.2f");
			}
			ImGui::PopID();

			// �g�k�ݒ�
			m_pGrabObj->SetScale(scale);
		}
		ImGui::End();
	}






	if (m_bGrab && 
		!bHoverWindow) {

		// �E�B���h�E�O && �I�u�W�F�N�g������ԂŐ���
		if (m_DragData.objX == nullptr)
		{
			m_DragData.objX = CObjectX::Create(m_DragData.nType, mouseWorldPos, MyLib::Vector3(0.0f), true);
			m_DragData.objX->SetType(CObject::TYPE_XFILE);
			m_DragData.objX->CreateCollisionBox();
		}
	}
	else {

		// �E�B���h�E�� && �I�u�W�F�N�g�����Ԃō폜
		if (m_DragData.objX != nullptr) {
			m_DragData.objX->Kill();
			m_DragData.objX = nullptr;
		}
	}

	// �h���b�O���ʒu�X�V
	if (m_DragData.objX != nullptr) {
		m_DragData.objX->SetState(CObjectX::STATE::STATE_EDIT);

		MyLib::Vector3 setpos = mouseWorldPos;
		setpos.y = 0.0f;
		m_DragData.objX->SetPosition(setpos);

		if (m_DragData.objX->GetCollisionLineBox() != nullptr) {
			m_DragData.objX->GetCollisionLineBox()->SetPosition(setpos);
		}
	}

	// �z�u
	if (m_bGrab &&
		ImGui::IsMouseReleased(0))
	{// �͂ݒ� && �}�E�X�����[�X

		if (!bHoverWindow) {

			MyLib::Vector3 setpos = mouseWorldPos;
			setpos.y = 0.0f;
			Regist(m_DragData.nType, setpos, MyLib::Vector3(0.0f), true);
		}
		m_bGrab = false;
	}



	// �I�u�W�F�N�g�I��
	if (!m_bGrab &&
		m_pHandle != nullptr &&
		!m_bReGrab)
	{
		if ((pKeyboard->GetPress(DIK_LCONTROL) || pKeyboard->GetPress(DIK_RCONTROL)) &&
			pKeyboard->GetTrigger(DIK_C)) 
		{// �R�s�[
			m_pCopyObj = m_pGrabObj;
		}

		if (m_pCopyObj != nullptr &&
			(pKeyboard->GetPress(DIK_LCONTROL) || pKeyboard->GetPress(DIK_RCONTROL)) &&
			pKeyboard->GetTrigger(DIK_V))
		{// �y�[�X�g

			// �C���f�b�N�X����
			int pasteIdx = m_pCopyObj->GetIdxXFile();
			std::vector<int>::iterator idx = std::find(m_nModelIdx.begin(), m_nModelIdx.end(), pasteIdx);

			Regist((*idx), 0.0f, 0.0f, true);
		}

		if (m_pHandle->IsHoverHandle() &&
			!pKeyboard->GetPress(DIK_LALT) &&
			ImGui::IsMouseClicked(0))
		{// �z�o�[���N���b�N

			m_bReGrab = true;

			// �ړ��̌���
			m_moveAngle = m_pHandle->GetHoverAngle();
			m_pHandle->SetState(CHandle::State::STATE_GRAB);
			return;
		}

		if (pKeyboard->GetTrigger(DIK_DELETE))
		{// �폜
			Delete(m_pGrabObj);
			m_pGrabObj = nullptr;

			if (m_pHandle != nullptr) {
				m_pHandle->Kill();
				m_pHandle = nullptr;
			}
		}
	}

	// �Ĉړ���
	MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();
	if (m_bReGrab &&
		m_pGrabObj != nullptr)
	{
		MyLib::Vector3 pos = m_pGrabObj->GetPosition();
		MyLib::Vector3 rot = m_pGrabObj->GetRotation();
		MyLib::Vector3 scale = m_pGrabObj->GetScale();

		// �����擾
		CHandle::SEditHandleInfo info = m_pHandle->GetDiff(m_moveAngle);
		pos += info.pos;
		rot += info.rot;
		UtilFunc::Transformation::RotNormalize(rot);

		scale += info.scale;
		if (scale.x <= 0.1f) scale.x = 0.1f;
		if (scale.y <= 0.1f) scale.y = 0.1f;
		if (scale.z <= 0.1f) scale.z = 0.1f;

		m_pGrabObj->SetPosition(pos);
		m_pGrabObj->SetRotation(rot);
		m_pGrabObj->SetScale(scale);

		if (m_pGrabObj->GetCollisionLineBox() != nullptr) {
			m_pGrabObj->GetCollisionLineBox()->SetPosition(pos);
		}

		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}
	}


	if (m_bReGrab &&
		!pKeyboard->GetPress(DIK_LALT) &&
		ImGui::IsMouseReleased(0))
	{// �����[�X
		m_bReGrab = false;
		m_pHandle->SetState(CHandle::State::STATE_NONE);
	}

	if (!pKeyboard->GetPress(DIK_LALT) &&
		!bHoverWindow &&
		ImGui::IsMouseClicked(0))
	{// �N���b�N

		m_pGrabObj = nullptr;

		// �擪��ۑ�
		CObjectX* pObject = nullptr;

		// ���X�g�R�s�[
		std::vector<CObjectX*> pObjectSort;

		while (m_List.ListLoop(&pObject))
		{
			// �v�f�𖖔��ɒǉ�
			pObjectSort.push_back(pObject);
		}

		// Z�\�[�g
		std::sort(pObjectSort.begin(), pObjectSort.end(), CObject::ZSortInverse);

		bool bHit = false;
		for (const auto& obj : pObjectSort)
		{
			if (!bHit) {

				MyLib::AABB aabb;
				aabb.vtxMin = obj->GetVtxMin();
				aabb.vtxMax = obj->GetVtxMax();

				MyLib::Matrix mtx = obj->GetWorldMtx();
				float time = 0.0f;
				MyLib::Vector3 OBpos;

				bHit = UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos);

				if (bHit)
				{// ����Ă�
					obj->SetState(CObjectX::STATE::STATE_EDIT);
					
					// �͂݃I�u�W�F�N�g
					m_pGrabObj = obj;

					if (m_pHandle != nullptr) {
						m_pHandle->SetPosition(m_pGrabObj->GetPosition());
					}

					if (m_pHandle == nullptr) {
						m_pHandle = CHandle::Create(m_HandleType, m_pGrabObj->GetPosition());
					}
				}
				else {
					obj->SetState(CObjectX::STATE::STATE_NONE);
				}
			}
			else {
				obj->SetState(CObjectX::STATE::STATE_NONE);
			}
		}

		if (!bHit && m_pHandle != nullptr) {
			m_pHandle->Kill();
			m_pHandle = nullptr;
		}
	}

}

//==========================================================================
// �n���h���؂�ւ�
//==========================================================================
void CEdit_Map::ChangeHandle()
{
	// �폜
	if (m_pHandle != nullptr) {
		m_pHandle->Kill();
		m_pHandle = nullptr;
	}

	// �n���h������
	m_pHandle = CHandle::Create(m_HandleType, m_pGrabObj->GetPosition());

}

//==========================================================================
// �Z�[�u
//==========================================================================
void CEdit_Map::Save()
{
	// �t�@�C�����J��
	FILE* pFile = fopen(SAVETEXT, "w");
	if (pFile == nullptr)
	{// �t�@�C�����J���Ȃ������ꍇ
		return;
	}

	// �t�@�C���ɏ����o��
	fprintf(pFile,
		"\n"
		"#------------------------------------------------------------------------------\n"
		"# ���f����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", (int)m_nModelIdx.size());

	fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# ���f���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

	// ���f���t�@�C������
	for (int i = 0; i < (int)m_nModelIdx.size(); i++)
	{
		fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", m_ModelFile[i].c_str(), i);
	}

	//=============================
	// ���f��
	//=============================
	fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# ���f���̔z�u\n"
		"#==============================================================================\n");

	// ���X�g���[�v
	CObjectX* pObj = nullptr;
	while (m_List.ListLoop(&pObj))
	{
		MyLib::Vector3 pos = pObj->GetPosition();	// �ʒu
		MyLib::Vector3 rot = pObj->GetRotation();	// ����
		int nShadow = 0;							// �e�g�����ǂ���
		int nType = 0;								// ���

		if (pObj->GetUseShadow())
		{// �g���Ă���ꍇ
			nShadow = 1;
		}
		
		// ���f���C���f�b�N�X����
		std::vector<int>::iterator nIdx = std::find(m_nModelIdx.begin(), m_nModelIdx.end(), pObj->GetIdxXFile());

		// �o��
		fprintf(pFile,
			"MODELSET\n"
			"\tTYPE = %d\n"
			"\tPOS = %.2f %.2f %.2f\n"
			"\tROT = %.2f %.2f %.2f\n"
			"\tSHADOW = %d\n"
			"END_MODELSET\n\n",
			(*nIdx), pos.x, pos.y, pos.z,
			rot.x, rot.y, rot.z, nShadow);
	}

	fprintf(pFile, "\nEND_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

	//�t�@�C�������
	fclose(pFile);
}

//==========================================================================
// ���[�h
//==========================================================================
void CEdit_Map::Load()
{
	char aComment[MAX_COMMENT] = {};	//�R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�
	int nCntTexture = 0;				// �e�N�X�`���ǂݍ��݃J�E���g

	// X�t�@�C���ǂݍ���
	ReadXFile();

	// �}�b�v�t�@�C�����J��
	FILE* pFile = fopen(LOADTEXT, "r");
	if (pFile == nullptr)
	{//�t�@�C�����J�����ꍇ
		return;
	}

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f���̐ݒ�
		if (strcmp(&aComment[0], "MODELSET") == 0)
		{
			int nType = 0, nShadow = 0;
			MyLib::Vector3 pos, rot;

			while (strcmp(&aComment[0], "END_MODELSET"))
			{// END_MODELSET������܂ŌJ��Ԃ�

				fscanf(pFile, "%s", &aComment[0]);	// �m�F����

				if (strcmp(&aComment[0], "TYPE") == 0)
				{// TYPE���������ޓǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%d", &nType);		// ���f����ނ̗�
				}

				if (strcmp(&aComment[0], "POS") == 0)
				{// POS��������ʒu�ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &pos.x);	// X���W
					fscanf(pFile, "%f", &pos.y);	// Y���W
					fscanf(pFile, "%f", &pos.z);	// Z���W
				}

				if (strcmp(&aComment[0], "ROT") == 0)
				{// ROT������������ǂݍ���

					fscanf(pFile, "%s", &aComment[0]);	// =�̕�
					fscanf(pFile, "%f", &rot.x);	// X�̌���
					fscanf(pFile, "%f", &rot.y);	// Y�̌���
					fscanf(pFile, "%f", &rot.z);	// Z�̌���
				}

				if (strcmp(&aComment[0], "SHADOW") == 0)
				{// SHADOW��������e�g�p

					fscanf(pFile, "%s", &aComment[0]);		// =�̕�
					fscanf(pFile, "%d", &nShadow);			// �e���g�����ǂ���
				}

			}// END_MODELSET�̂�����

			// �ǉ�
			bool bShadow = (nShadow == 1);
			Regist(nType, pos, rot, bShadow);
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���
			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);
}

//==========================================================================
// ���f���ǂݍ��ݏ���
//==========================================================================
HRESULT CEdit_Map::ReadXFile()
{
	char aComment[MAX_COMMENT] = {};	// �R�����g�p
	int nFileNum = 0;					// �t�@�C���̐�

	// �t�@�C�����J��
	FILE* pFile = fopen(LOADTEXT, "r");
	if (pFile == nullptr)
	{// �t�@�C�����J�����ꍇ
		return E_FAIL;
	}

	int modelNum = 0;
	m_nModelIdx.clear();
	m_ModelFile.clear();

	while (1)
	{// END_SCRIPT������܂ŌJ��Ԃ�

		// ������̓ǂݍ���
		fscanf(pFile, "%s", &aComment[0]);

		// ���f�����̐ݒ�
		if (strcmp(&aComment[0], "NUM_MODEL") == 0)
		{// NUM_MODEL��������

			fscanf(pFile, "%s", &aComment[0]);	// =�̕�
			fscanf(pFile, "%d", &nFileNum);		// ���f����
		}

		while (modelNum != nFileNum)
		{// ���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			// ������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			// ���f�����̐ݒ�
			if (strcmp(&aComment[0], "MODEL_FILENAME") == 0)
			{// NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	// =�̕�
				fscanf(pFile, "%s", &aComment[0]);	// �t�@�C����

				// �ǉ�
				m_ModelFile.push_back(&aComment[0]);
				m_nModelIdx.push_back(0);

				// �C���f�b�N�X�擾
				m_nModelIdx.back() = CXLoad::GetInstance()->XLoad(aComment);

				modelNum++;	// ���f�������Z
			}
		}

		if (strcmp(&aComment[0], "END_SCRIPT") == 0)
		{// �I�������Ń��[�v�𔲂���
			break;
		}
	}

	// �t�@�C�������
	fclose(pFile);

	return S_OK;
}

//==========================================================================
// �폜
//==========================================================================
void CEdit_Map::Delete(CObjectX* obj)
{
	// �I������
	std::vector<CObjectX*>::iterator itr = std::find(m_pObjX.begin(), m_pObjX.end(), obj);

	// �v�f�����������ꍇ�̂ݍ폜
	if (itr != m_pObjX.end())
	{
		// ���X�g����폜
		m_List.Delete((*itr));

		(*itr)->Kill();
		m_pObjX.erase(itr);
	}
}

//==========================================================================
// ���蓖��
//==========================================================================
void CEdit_Map::Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow)
{

	m_pObjX.emplace_back();

	if (m_nModelIdx.size() <= idx) {
		int n = 0;
	}

	// �^�C�v�̕��𐶐�
	m_pObjX.back() = CObjectX::Create(m_nModelIdx[idx], pos, rot, bShadow);
	m_pObjX.back()->SetType(CObject::TYPE_XFILE);
	m_pObjX.back()->CreateCollisionBox();

	// ���X�g�ɒǉ�
	m_List.Regist(m_pObjX.back());

	if (m_bGrab) {

		// ���X�g�R�s�[
		CObjectX* pObject = nullptr;
		while (m_List.ListLoop(&pObject))
		{
			pObject->SetState(CObjectX::STATE::STATE_NONE);
		}

		// �͂݃I�u�W�F�N�g
		m_pGrabObj = m_pObjX.back();

		m_pObjX.back()->SetState(CObjectX::STATE::STATE_EDIT);

		if (m_pHandle != nullptr) {
			m_pHandle->SetPosition(m_pGrabObj->GetPosition());
		}

		if (m_pHandle == nullptr) {
			m_pHandle = CHandle::Create(CHandle::HandleType::TYPE_MOVE, m_pGrabObj->GetPosition());
		}
	}
}