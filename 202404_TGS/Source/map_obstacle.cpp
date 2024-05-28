//=============================================================================
// 
//  �}�b�v�̏�Q������ [map_obstacle.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "map_obstacle.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "collisionLine_Box.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{

}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CMap_Obstacle> CMap_Obstacle::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CMap_Obstacle::CMap_Obstacle(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CMap_Obstacle::~CMap_Obstacle()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CMap_Obstacle *CMap_Obstacle::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// �������̊m��
	CMap_Obstacle* pObj = DEBUG_NEW CMap_Obstacle;

	if (pObj != nullptr)
	{
		pObj->m_ObstacleInfo = info;

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CMap_Obstacle::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	HRESULT hr = CObjectX::Init(m_ObstacleInfo.modelFile);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

#if _DEBUG

	for (const auto& box : m_ObstacleInfo.boxcolliders)
	{
		CCollisionLine_Box* pBox = CCollisionLine_Box::Create(MyLib::AABB(box.vtxMin, box.vtxMax), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	// �����蔻��{�b�N�X
		m_pCollisionLineBox.push_back(pBox);
	}
#endif

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CMap_Obstacle::Uninit()
{
	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CMap_Obstacle::Kill()
{
	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CMap_Obstacle::Update()
{
	for (auto& collider : m_ObstacleInfo.boxcolliders)
	{
		collider.TransformOffset(GetWorldMtx());
	}

#if _DEBUG
	int i = 0;
	for (const auto& box : m_pCollisionLineBox)
	{
		m_ObstacleInfo.boxcolliders[i].TransformOffset(GetWorldMtx());
		box->SetPosition(m_ObstacleInfo.boxcolliders[i].worldmtx.GetWorldPosition());
		box->SetRotation(GetRotation());
		i++;
	}

#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMap_Obstacle::Draw()
{
	// �`��
	CObjectX::Draw();
}

