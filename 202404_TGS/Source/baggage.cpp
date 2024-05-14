//=============================================================================
// 
//  �ו����� [baggage.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "baggage.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL[] =
	{
		"data\\MODEL\\ballast_01.x",	// �z
	};
	const float LIMIT_HEIGHT = 800.0f;	// �������
	const float VELOCITY = 1.0f;
}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CBaggage> CBaggage::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBaggage::CBaggage(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_type = TYPE::TYPE_CLOTH;	// ���
	m_fWeight = 0.0f;	// �d��
	m_bDrop = false;	// ��������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBaggage::~CBaggage()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CBaggage *CBaggage::Create(TYPE type)
{
	// �������̊m��
	CBaggage* pObj = DEBUG_NEW CBaggage;

	if (pObj != nullptr)
	{
		// �������
		pObj->m_type = type;

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBaggage::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	int typeID = static_cast<int>(m_type);
	HRESULT hr = CObjectX::Init(MODEL[typeID]);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �p�����[�^�ݒ�
	m_fWeight = 0.7f;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBaggage::Uninit()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CBaggage::Kill()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBaggage::Update()
{
	// ���擾
	MyLib::Vector3 posOrigin = GetOriginPosition();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	ImGui::DragFloat("weight", &m_fWeight, 0.1f, 0.0f, 0.0f, "%.2f");

	// �ʒu�X�V
	pos += move;
	pos += m_force;

	// �d�͉��Z
	move.y -= mylib_const::GRAVITY * m_fWeight;

	if (pos.y <= 0.0f) pos.y = 0.0f, move.y = 0.0f;

	// ��������
	m_bDrop = pos.y <= 0.0f;

	// �����␳
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;
	/*m_force.x += (0.0f - m_force.x) * 0.01f;
	m_force.z += (0.0f - m_force.z) * 0.01f;*/

	// ���ݒ�
	SetPosition(pos);
	SetMove(move);


	// ���E���x
	(posOrigin.y + LIMIT_HEIGHT);

}

//==========================================================================
// �͒ǉ�
//==========================================================================
void CBaggage::AddForce(const MyLib::Vector3& power, const MyLib::Vector3& ActPos)
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	float angle = pos.AngleXY(ActPos);
	m_force.x += sinf(angle) * (power.x);
	m_force.x = fabsf(m_force.x);

	move.y += power.y;

	SetMove(move);
}

//==========================================================================
// �`�揈��
//==========================================================================
void CBaggage::Draw()
{
	// �`��
	CObjectX::Draw();
}

