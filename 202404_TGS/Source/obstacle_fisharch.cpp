//=============================================================================
// 
//  �A�[�`��̋���Q������ [obstacle_fisharch.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "obstacle_fisharch.h"
#include "debugproc.h"
#include "manager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int NUM_FISH = (10);				// ���̑���
	const float ROTATE_SPEED = (0.02f);		// ��]���x
	const float FISH_ROT = (1.0f / NUM_FISH);	// 1���ӂ�̊p�x����
	const float WIDTH = (750.0f);			// ���S����̋���
	const float HEIGHT = (1500.0f);			// �ω����鋗��
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObstacle_FishArch::CObstacle_FishArch(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// �l�̃N���A
	m_FishList.clear();
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObstacle_FishArch::~CObstacle_FishArch()
{

}

//==========================================================================
// ��������
//==========================================================================
CObstacle_FishArch* CObstacle_FishArch::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// �������̊m��
	CObstacle_FishArch* pObj = DEBUG_NEW CObstacle_FishArch;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObstacle_FishArch::Init()
{
	CMap_Obstacle::GetListObj().Regist(this);
	MyLib::Vector3 rot;
	
	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ���𐶐�����
	for (int i = 0; i < NUM_FISH; i++)
	{
		FishInfo info;
		info.nIdx = i;
		info.pFish = CMap_Obstacle::Create(GetObstacleInfo(), false);
		rot.x = (i * FISH_ROT) * (D3DX_PI * 2);
		UtilFunc::Transformation::RotNormalize(rot.x);
		info.pFish->SetRotation(rot);
		SetFishOffSet(info);
		info.pFish->SetPosition(info.offset);
		m_FishList.push_back(info);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObstacle_FishArch::Uninit()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish != nullptr)
		{
			it.pFish->Uninit();
		}
	}

	// ���X�g�N���A
	m_FishList.clear();

	// �I������
	CMap_Obstacle::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CObstacle_FishArch::Kill()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish != nullptr)
		{
			it.pFish->Uninit();
		}
	}

	// �I������
	CMap_Obstacle::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObstacle_FishArch::Update()
{
	// ��]
	MyLib::Vector3 rot = GetRotation();
	rot.x += ROTATE_SPEED;
	UtilFunc::Transformation::RotNormalize(rot.x);
	SetRotation(rot);

	// ���̍X�V
	ControllFish();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObstacle_FishArch::Draw()
{
	
}

//==========================================================================
// ���̑���ݒ�
//==========================================================================
void CObstacle_FishArch::ControllFish()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish == nullptr) continue;	// ���݂��Ă��Ȃ�

		MyLib::Vector3 rot = it.pFish->GetRotation();

		// �ʒu�̐ݒ�
		{
			MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxFish;	// �v�Z�p�}�g���b�N�X�錾
			MyLib::Matrix mtxParent = GetWorldMtx();	// �e�̃}�g���b�N�X
			MyLib::Vector3 FishPos = it.offset;

			// ���W�p�}�g���b�N�X������
			mtxFish.Identity();

			// �ʒu�𔽉f����
			mtxTrans.Translation(FishPos);
			mtxFish.Multiply(mtxFish, mtxTrans);

			//// �����𔽉f����
			//mtxRot.RotationYawPitchRoll(GetRotation().y, GetRotation().x, GetRotation().z);
			//mtxFish.Multiply(mtxFish, mtxRot);

			mtxParent.Multiply(mtxFish, mtxParent);

			// ���W��ݒ�
			FishPos = (mtxFish._41, mtxFish._42, mtxFish._43);

			it.pFish->SetPosition(mtxParent.GetWorldPosition());
		}

		// �����ƃI�t�Z�b�g�ݒ�
		{	
			rot.x = GetRotation().x + (D3DX_PI * 2) * (FISH_ROT * it.nIdx);
			UtilFunc::Transformation::RotNormalize(rot.x);
			it.pFish->SetRotation(rot);
			SetFishOffSet(it);

			CManager::GetInstance()->GetDebugProc()->Print("�p�x [ %f ]\n", rot.x);
		}
	}
}

//==========================================================================
// ���̃I�t�Z�b�g�ݒ�
//==========================================================================
void CObstacle_FishArch::SetFishOffSet(FishInfo& info)
{
	MyLib::Vector3 rot = GetRotation();
	rot.x += (D3DX_PI * 2) * (FISH_ROT * info.nIdx);
	UtilFunc::Transformation::RotNormalize(rot.x);

	// ���������߂�
	float rate = rot.x;

	rate = fabsf(fabsf(rate) / D3DX_PI - 1.0f);

	info.offset.z = sinf(rot.x) * WIDTH;
	info.offset.y = cosf(rot.x) * HEIGHT;
}