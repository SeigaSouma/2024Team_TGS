//=============================================================================
// 
//  �����Ώ��� [waterstone.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "waterstone.h"
#include "manager.h"
#include "calculation.h"
#include "3D_effect.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL[] =
	{
		"data\\MODEL\\map_object\\rock_01_small.x",
		"data\\MODEL\\map_object\\rock_02.x",
		"data\\MODEL\\map_object\\rock_03.x",
		"data\\MODEL\\map_object\\rock_04.x",
		"data\\MODEL\\map_object\\rock_05.x",
	};
	float DEFAULT_SPLASHTIME = 0.14f;	// �ʏ�̂��Ԃ�����
}
CListManager<CWaterStone> CWaterStone::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CWaterStone::CWaterStone(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fSplashTimer = 0.0f;		// ���Ԃ��^�C�}�[
	m_fIntervalSplash = 0.0f;	// ���Ԃ��܂ł̃C���^�[�o��
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CWaterStone::~CWaterStone()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CWaterStone *CWaterStone::Create(const CWaterStone_Manager::SStoneInfo& info)
{
	// �������̊m��
	CWaterStone* pObj = DEBUG_NEW CWaterStone;

	if (pObj != nullptr)
	{
		// �������ݒ�
		pObj->m_StoneInfo = info;

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CWaterStone::Init()
{
	// �ʒu�ݒ�
	SetPosition(m_StoneInfo.pos);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	int idx = (sizeof(MODEL) / sizeof(*MODEL)) - 1;
	idx = UtilFunc::Transformation::Random(0, idx);

	HRESULT hr = CObjectX::Init(MODEL[idx]);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// ���X�g�ǉ�
	m_List.Regist(this);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CWaterStone::Uninit()
{
	// ���X�g�폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CWaterStone::Kill()
{
	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CWaterStone::Update()
{
	// ���Ԃ��^�C�}�[���Z
	m_fSplashTimer += CManager::GetInstance()->GetDeltaTime();


	static int createIdx = 8;
	static int Life = 50;
	static int Randrange = 28;
	static int posXZrange = 50;
	static int XZrange = 117;
	static int MinYRange = 37;
	static int MaxYRange = 43;
	static int colorRandRange = 22;
	static float defradius = 25.2f;
	static float gravity = 0.16f;
	static float movefactor = 0.4f;

	// �J���[�G�f�B�b�g
	static ImVec4 myColor = ImVec4(0.658f, 0.658f, 1.0, 0.87f); // RGBA

	if (ImGui::TreeNode("Stone Water"))
	{
		ImGui::DragInt("CreateNum", &createIdx, 1);
		ImGui::DragFloat("Interval", &DEFAULT_SPLASHTIME, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragInt("Interval_Randrange", &Randrange, 1);
		ImGui::DragInt("Life", &Life, 1);
		ImGui::DragInt("pos XZrange", &posXZrange, 1);
		ImGui::DragInt("XZrange", &XZrange, 1);
		ImGui::DragInt("MinYRange", &MinYRange, 1);
		ImGui::DragInt("MaxYRange", &MaxYRange, 1, MinYRange);
		ImGui::DragInt("colorRandRange", &colorRandRange, 1);
		ImGui::DragFloat("defradius", &defradius, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("gravity", &gravity, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::DragFloat("Move factor", &movefactor, 0.01f, 0.0f, 0.0f, "%.2f");
		ImGui::ColorEdit4("Color", &myColor.x);

		ImGui::TreePop();
	}

	// �X�v���b�V���I
	if (m_fIntervalSplash <= m_fSplashTimer)
	{

		MyLib::Vector3 move;
		MyLib::Vector3 posOrigin = GetPosition();
		MyLib::Vector3 pos;

		for (int i = 0; i < createIdx; i++)
		{
			float randmoveX = UtilFunc::Transformation::Random(-XZrange, XZrange) * 0.01f;
			float randmoveY = UtilFunc::Transformation::Random(MinYRange, MaxYRange) * 0.1f;
			float randRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;

			move.x = randmoveX;
			move.z = randmoveX;
			move.y = randmoveY;

			float radius = defradius + randRadius;

			int xz = UtilFunc::Transformation::Random(-posXZrange, posXZrange);
			pos = posOrigin + MyLib::Vector3(xz, 0.0f, xz);


			float colorrand = UtilFunc::Transformation::Random(-colorRandRange, colorRandRange) * 0.01f;
			CEffect3D* pEffect = CEffect3D::Create(
				pos,
				move,
				D3DXCOLOR(myColor.x + colorrand, myColor.y + colorrand, myColor.z, myColor.w),
				radius,
				Life,
				CEffect3D::MOVEEFFECT::MOVEEFFECT_ADD,
				CEffect3D::TYPE::TYPE_SMOKE);
			pEffect->SetEnableGravity();
			pEffect->SetGravityValue(gravity);
		}

		// �C���^�[�o���X�V
		m_fIntervalSplash = DEFAULT_SPLASHTIME + UtilFunc::Transformation::Random(-Randrange, Randrange) * 0.01f;

		// ���Ԃ��^�C�}�[
		m_fSplashTimer = 0.0f;
	}

	m_StoneInfo.pos = GetPosition();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CWaterStone::Draw()
{
	// �`��
	CObjectX::Draw();
}

