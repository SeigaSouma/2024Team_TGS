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
		MyLib::Collider_BOX& collider = m_ObstacleInfo.boxcolliders[i];

		collider.TransformOffset(GetWorldMtx());
		box->SetPosition(collider.worldmtx.GetWorldPosition());
		box->SetRotation(GetRotation());
		box->SetAABB(MyLib::AABB(collider.vtxMin, collider.vtxMax), GetScale().x);
		i++;
	}

#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CMap_Obstacle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �Q�ƒl�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// �o�b�t�@�ւ̒l�ɑ΂��Ẵ}�X�N�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL);

	// �e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);		// Z+�X�e���V������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_ZERO);		// Z+�X�e���V�����s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_ZERO);		// Z�e�X�g�̂ݎ��s

	// �X�e���V���`��
	CObjectX::Draw();

	// �X�e���V���o�b�t�@�����ɂ���Z�o�b�t�@�ݒ�߂�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ���ʂ̕`��
	CObjectX::Draw();
}

