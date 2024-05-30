//=============================================================================
// 
//  �ו����� [baggage.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "baggage.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "map_obstacle.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL[] =
	{
		"data\\MODEL\\map_object\\rock_02.x",	// �z
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
CBaggage::CBaggage(int nPriority) : CObjectQuaternion(nPriority)
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
	HRESULT hr = CObjectQuaternion::Init(MODEL[typeID]);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �p�����[�^�ݒ�
	m_fWeight = 1.8f;

	CreateCollisionBox();
	SetState(CObjectX::STATE::STATE_EDIT);

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
	CObjectQuaternion::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CBaggage::Kill()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectQuaternion::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBaggage::Update()
{
	if (CGame::GetInstance()->GetGameManager()->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH) {
		return;
	}

	CObjectQuaternion::Update();

	// ��Q���Ƃ̏Փ˔���
	Hit();

	// ���擾
	MyLib::Vector3 posOrigin = GetOriginPosition();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 move = GetMove();
	ImGui::DragFloat("weight", &m_fWeight, 0.1f, 0.0f, 0.0f, "%.2f");

	// �ʒu�X�V
	pos += move;
	pos += m_force;

	rot.x += fabsf(move.x) * 0.01f;
	rot.y += fabsf(move.y) * 0.01f;
	UtilFunc::Transformation::RotNormalize(rot);

	// �d�͉��Z
	move.y -= mylib_const::GRAVITY * m_fWeight;

	static float limitMoveY = 30.0f;
	ImGui::DragFloat("Limit MoveY", &limitMoveY, 1.0f, 0.0f, 0.0f, "%.2f");

	if (move.y >= limitMoveY)
	{
		move.y = limitMoveY;
	}

	if (pos.y <= GetOriginPosition().y) pos.y = GetOriginPosition().y, move.y = 0.0f;

	// ��������
	m_bDrop = pos.y <= 0.0f;

	// �����␳
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;
	/*m_force.x += (0.0f - m_force.x) * 0.01f;
	m_force.z += (0.0f - m_force.z) * 0.01f;*/

	// ���ݒ�
	SetPosition(pos);
	SetRotation(rot);
	SetMove(move);


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
	m_force.x = (power.x);
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
	CObjectQuaternion::Draw();
}

//==========================================================================
// ��Q���Ƃ̔���
//==========================================================================
void CBaggage::Hit()
{
	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	MyLib::Vector3 MyPos = GetPosition();
	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;
		MyLib::Vector3 ObjPos = pObj->GetPosition();

		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();
		for (const auto& collider : info.boxcolliders)
		{
			if (UtilFunc::Collision::IsAABBCollidingWithBox(GetAABB(), GetWorldMtx(), MyLib::AABB(collider.vtxMin, collider.vtxMax), collider.worldmtx))
			{
				MyLib::Vector3 move = GetMove();
				move.y *= -1.0f;
				SetMove(move);
				return;
			}
		}
	}
}