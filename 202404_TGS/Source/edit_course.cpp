//=============================================================================
// 
//  �R�[�X�G�f�B�^�\���� [edit_course.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "edit_course.h"
#include "course.h"
#include "manager.h"
#include "calculation.h"
#include "map_obstacle.h"
#include "camera.h"
#include "spline.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::AABB AABB_LINE = MyLib::AABB(-25.0f, 25.0f);	// �ӂ�AABB

}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_Course::CEdit_Course()
{
	// �l�̃N���A
	m_nEditIdx = 0;			// ���삷��C���f�b�N�X�ԍ�
	m_bEdit = false;		// ���쒆����
	m_bDrag = false;		// �͂ݔ���
	m_bHoverWindow = false;	// �}�E�X�̃E�B���h�E�z�o�[����
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_Course::~CEdit_Course()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_Course::Init()
{

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_Course::Uninit()
{
	// �I������
	CEdit::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_Course::Update()
{
	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// �t�@�C������
	FileControl();

	// �ӂ̑����ύX
	ChangeLineNum();

	// ���C���I��
	SelectLine();

	// �͂ݒ�
	DragLine();

	// �g�����X�t�H�[��
	Transform();

	// �ŏ��ƍŌ�ό`
	TransformBeginEnd();

}

//==========================================================================
// �t�@�C������
//==========================================================================
void CEdit_Course::FileControl()
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
// �ӂ̐��ύX
//==========================================================================
void CEdit_Course::ChangeLineNum()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �����ύX
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Change Line Num:");
	ImGui::SameLine();
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{
		pCourse->Reset();
	}
	ImGui::SameLine(0.0f);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		pCourse->Reset();

		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera == nullptr) return;

		MyLib::Vector3 pos = UtilFunc::Transformation::CalcScreenToXZ(
			D3DXVECTOR2(640.0f, 360.0f),
			D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT),
			pCamera->GetMtxView(),
			pCamera->GetMtxProjection());

		//// �ӏ��擾
		//std::vector<CCourse::LineInfo> vecInfo = pCourse->GetLineInfo();
		//int idx = vecInfo.size() - 1;

		//vecInfo[idx].pos = pos;
		//vecInfo[idx].pos.y = 0.0f;

		//pCourse->SetLineInfo(vecInfo);
	}
	ImGui::SameLine();
	ImGui::Text("%d", pCourse->GetVecPosition().size());
}

//==========================================================================
// ���C���I��
//==========================================================================
void CEdit_Course::SelectLine()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �ӏ��擾
	std::vector<MyLib::Vector3> vecSegmentPos = pCourse->GetVecPosition();
	MyLib::Vector3 coursepos = pCourse->GetPosition();

	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-25.0f, 25.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		!m_bDrag &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{// ���N���b�N��

		// ���씻�胊�Z�b�g
		m_bEdit = false;

		int i = 0;
		for (const auto& vtxpos : vecSegmentPos)
		{
			if (i == 0 || i == vecSegmentPos.size() - 1)
			{
				i++;
				continue;
			}
			// �}�g���b�N�X������
			mtx.Identity();
			mtxTrans.Identity();

			// �ʒu��񔽉f
			MyLib::Vector3 transpos = vtxpos + coursepos;
			mtxTrans.Translation(transpos);
			mtx.Multiply(mtx, mtxTrans);

			MyLib::Vector3 OBpos;

			if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
			{// �d�Ȃ� && ���N���b�N

				// ���삷��ӕۑ�
				m_nEditIdx = i;

				m_bEdit = true;
				break;
			}
			i++;
		}

		// �F���Z�b�g
		for (int i = 0; i < vecSegmentPos.size(); i++)
		{
			CCollisionLine_Box* pBox = pCourse->GetCollisionLineBox(i);
			if (pBox == nullptr) continue;

			pBox->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//==========================================================================
// ���C���͂�
//==========================================================================
void CEdit_Course::DragLine()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	if (!m_bEdit) return;

	// �ӏ��擾
	MyLib::Vector3 segmentPos = pCourse->GetVecPosition(m_nEditIdx);
	MyLib::Vector3 coursepos = pCourse->GetPosition();

	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-50.0f, 50.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		pMouse->GetPress(CInputMouse::BUTTON::BUTTON_LEFT))
	{// ���������ݎ�

		// �}�g���b�N�X������
		mtx.Identity();
		mtxTrans.Identity();

		// �ʒu��񔽉f
		MyLib::Vector3 transpos = segmentPos + coursepos;
		mtxTrans.Translation(transpos);
		mtx.Multiply(mtx, mtxTrans);

		MyLib::Vector3 OBpos;

		if (UtilFunc::Collision::CollisionRayAABB(&mousePos, &mouseRay, &aabb, &mtx, time, &OBpos))
		{// �d�Ȃ�
			m_bDrag = true;
		}

	}

	if (pMouse->GetRelease(CInputMouse::BUTTON::BUTTON_LEFT))
	{
		m_bDrag = false;
	}

	// �h���b�O��
	if (m_bDrag)
	{
		CCamera* pCamera = CManager::GetInstance()->GetCamera();
		if (pCamera == nullptr) return;

		// �Ĉړ���
		MyLib::Vector3 diffpos = pMouse->GetWorldDiffPosition();
		segmentPos.x += diffpos.x;
		segmentPos.z += diffpos.z;
	}

	// ���_�f�[�^�ݒ�
	pCourse->SetVecPosition(m_nEditIdx, segmentPos);
}

//==========================================================================
// �g�����X�t�H�[��
//==========================================================================
void CEdit_Course::Transform()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �ӏ��擾
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

	// ���삷��ӂ̏��
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

		// ���Z�b�g
		if (ImGui::Button("RESET")) {
			editpos = 0.0f;
		}
		

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		//=============================
		// �ʒu�ݒ�
		//=============================
		ImGui::Text("pos");
		ImGui::SameLine();

		// X
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &editpos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &editpos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &editpos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		////=============================
		//// �����ݒ�
		////=============================
		//ImGui::Text("rot");
		//ImGui::SameLine();

		//// X
		//ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("x", &editInfo.rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();

		//// Y
		//ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("y", &editInfo.rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();

		//// Z
		//ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("z", &editInfo.rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		//}
		//ImGui::PopID();




		////=============================
		//// ���ݒ�
		////=============================
		//ImGui::Text("width");
		//ImGui::SameLine();

		//// X
		//ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		//{
		//	ImGui::SetNextItemWidth(windowWidth);
		//	ImGui::DragFloat("width", &editInfo.width, POS_MOVE, 0.0f, 0.0f, "%.2f");
		//	ImGui::SameLine();
		//}
		//ImGui::PopID();


		ImGui::TreePop();
	}

	// ���C�����ݒ�
	pCourse->SetVecPosition(m_nEditIdx, editpos);
}

//==========================================================================
// �ŏ��ƍŌ�ό`
//==========================================================================
void CEdit_Course::TransformBeginEnd()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �ӏ��擾
	std::vector<MyLib::Vector3> segmentPos = pCourse->GetVecPosition();

	// �ŏ��ƍŌ�A�t�����ɏ����o��
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// �ŏ�
	angle = segmentPos[1].AngleXZ(segmentPos[0]);
	begin = MyLib::Vector3(
		segmentPos[1].x + sinf(angle) * -10.0f,
		segmentPos[1].y,
		segmentPos[1].z + cosf(angle) * -10.0f);

	// �Ō�
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

