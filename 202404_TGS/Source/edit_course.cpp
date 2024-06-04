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

	// ���C���I��
	SelectLine();

	// �g�����X�t�H�[��
	Transform();
}

//==========================================================================
// ���C���I��
//==========================================================================
void CEdit_Course::SelectLine()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �ӏ��擾
	std::vector<CCourse::LineInfo> vecInfo = pCourse->GetLineInfo();
	MyLib::Vector3 coursepos = pCourse->GetPosition();

	// �}�E�X���
	CInputMouse* pMouse = CInputMouse::GetInstance();
	MyLib::Vector3 mouseRay = pMouse->GetRay();
	MyLib::Vector3 mousePos = pMouse->GetNearPosition();

	MyLib::AABB aabb = MyLib::AABB(-25.0f, 25.0f);
	float time = 0.0f;
	MyLib::Matrix mtx, mtxTrans;


	if (!m_bHoverWindow &&
		pMouse->GetTrigger(CInputMouse::BUTTON::BUTTON_LEFT))
	{// ���N���b�N��

		// ���씻�胊�Z�b�g
		m_bEdit = false;

		int i = 0;
		for (const auto& info : vecInfo)
		{
			// �}�g���b�N�X������
			mtx.Identity();
			mtxTrans.Identity();

			// �ʒu��񔽉f
			MyLib::Vector3 transpos = info.pos + coursepos;
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
	}
}

//==========================================================================
// �g�����X�t�H�[��
//==========================================================================
void CEdit_Course::Transform()
{
	CCourse* pCourse = CGame::GetInstance()->GetCourse();
	if (pCourse == nullptr) return;

	// �ӏ��擾
	std::vector<CCourse::LineInfo> vecInfo = pCourse->GetLineInfo();

	// ���삷��ӂ̏��
	CCourse::LineInfo editInfo = vecInfo[m_nEditIdx];

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

		// ���Z�b�g
		if (ImGui::Button("ALL RESET")) {
			editInfo.pos = 0.0f;
			editInfo.rot = 0.0f;
		}
		ImGui::SameLine();
		if (ImGui::Button("POS RESET")) {
			editInfo.pos = 0.0f;
		}
		ImGui::SameLine();
		if (ImGui::Button("ROT RESET")) {
			editInfo.rot = 0.0f;
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
			ImGui::DragFloat("x", &editInfo.pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &editInfo.pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &editInfo.pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();


		//=============================
		// �����ݒ�
		//=============================
		ImGui::Text("rot");
		ImGui::SameLine();

		// X
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &editInfo.rot.x, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Y
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &editInfo.rot.y, ROT_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();

		// Z
		ImGui::PushID(1); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &editInfo.rot.z, ROT_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();




		//=============================
		// ���ݒ�
		//=============================
		ImGui::Text("width");
		ImGui::SameLine();

		// X
		ImGui::PushID(2); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("width", &editInfo.width, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();


		ImGui::TreePop();
	}

	// ���C�����ݒ�
	pCourse->SetLineInfo(m_nEditIdx, editInfo);
}
