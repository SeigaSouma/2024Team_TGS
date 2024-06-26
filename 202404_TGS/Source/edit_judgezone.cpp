//=============================================================================
// 
//  ����]�[���G�f�B�^�\���� [edit_judgezone.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "edit_judgezone.h"
#include "judge.h"
#include "judgezoneManager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CEdit_JudgeZone::CEdit_JudgeZone()
{
	// �l�̃N���A
	m_bHoverWindow = false;	// �}�E�X�̃E�B���h�E�z�o�[����
	for (int cntborder = 0; cntborder < CJudge::BORDER::MAX; cntborder++)
	{
		for (int cntjudge = 0; cntjudge < CJudge::JUDGE::JUDGE_MAX; cntjudge++)
		{
			m_abEnable[cntborder][cntjudge] = true;
		}
		m_aConditionType[cntborder] = CJudge::CONDITIONTYPE::TYPE_NONE;
	}
	m_JudgeZoneData.conditionTop.type = CJudge::CONDITIONTYPE::TYPE_NONE;
	m_JudgeZoneData.conditionUnder.type = CJudge::CONDITIONTYPE::TYPE_NONE;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CEdit_JudgeZone::~CEdit_JudgeZone()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CEdit_JudgeZone::Init()
{

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CEdit_JudgeZone::Uninit()
{
	// �I������
	CEdit::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CEdit_JudgeZone::Update()
{
	// �E�B���h�E�̃}�E�X�z�o�[����
	ImGuiHoveredFlags frag = 128;
	m_bHoverWindow = ImGui::IsWindowHovered(frag);

	// �t�@�C������
	FileControl();

	// ����]�[���ݒ�
	SetJudgeZone();
}

//==========================================================================
// �t�@�C������
//==========================================================================
void CEdit_JudgeZone::FileControl()
{
	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	CJudgeZoneManager* manager = CJudgeZoneManager::GetInstance();

	float width = 150.0f;
	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save"))
	{
		manager->SaveZone("data\\TEXT\\judgezone\\temphogehoge.txt", m_JudgeZoneData.zone, m_JudgeZoneData.conditionTop, m_JudgeZoneData.conditionUnder);
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Save_as"))
	{
		//manager->SaveZone();
	}
	ImGui::SameLine();

	ImGui::SetNextItemWidth(width);
	if (ImGui::Button("Load"))
	{
		//manager->LoadZone();
	}
}

//==========================================================================
// ����]�[���ݒ�
//==========================================================================
void CEdit_JudgeZone::SetJudgeZone()
{
	if (ImGui::TreeNode("JudgeZone"))
	{
		ImGui::DragFloat("Start", &m_JudgeZoneData.zone.start, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("End", &m_JudgeZoneData.zone.end, 0.01f, m_JudgeZoneData.zone.start, 1.0f);
		ImGui::DragFloat("Border", &m_JudgeZoneData.zone.borderHeight);
		
		if (ImGui::TreeNode("Condition_Top"))
		{// �㑤����
			SetCondition(m_JudgeZoneData.conditionTop,m_abEnable[CJudge::BORDER::TOP],m_aConditionType[CJudge::BORDER::TOP]);

			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Condition_Under"))
		{// ��������
			SetCondition(m_JudgeZoneData.conditionUnder,m_abEnable[CJudge::BORDER::UNDER], m_aConditionType[CJudge::BORDER::UNDER]);

			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CEdit_JudgeZone::SetCondition(CJudge::SJudgeCondition& condition, bool* pEnable, CJudge::CONDITIONTYPE& conditionType)
{
	static const char* items[] = { "None", "HitNum" };
	int selectedItem = conditionType;

	// ���
	for (int cnt = 0; cnt < CJudge::CONDITIONTYPE::TYPE_MAX; cnt++)
	{
		if (ImGui::RadioButton(items[cnt], &selectedItem, cnt))
		{
			conditionType = static_cast<CJudge::CONDITIONTYPE>(selectedItem);
		}
	}
	condition.type = conditionType;

	// �L���E�����ݒ�㐔�l�ݒ�
	// =========AAA=========
	ImGui::Checkbox("Enable_AAA", &pEnable[0]);
	if (!pEnable[0]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("AAA_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_AAA]);
	
	if (!pEnable[0])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_AAA] = -1;
	}
	// =========AAA=========

	// =========BBB=========
	ImGui::Checkbox("Enable_BBB", &pEnable[1]);
	if (!pEnable[1]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("BBB_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_BBB]);
	
	if (!pEnable[1])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_BBB] = -1;
	}
	// =========BBB=========

	// =========CCC=========
	ImGui::Checkbox("Enable_CCC", &pEnable[2]);
	if (!pEnable[2]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("CCC_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_CCC]);
	
	if (!pEnable[2])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_CCC] = -1;
	}
	// =========CCC=========

	// =========DDD=========
	ImGui::Checkbox("Enable_DDD", &pEnable[3]);
	if (!pEnable[3]) ImGui::BeginDisabled(true);
	
		ImGui::InputInt("DDD_Num", &condition.judgeParam[CJudge::JUDGE::JUDGE_DDD]);
	
	if (!pEnable[3])
	{
		ImGui::EndDisabled();
		condition.judgeParam[CJudge::JUDGE::JUDGE_DDD] = -1;
	}
	// =========DDD=========
}
