//=============================================================================
// 
//  �A�[�`��̋���Q���w�b�_�[ [obstacle_fisharch.h]
//  Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _OBSTACLE_FISHARCH_H_
#define _OBSTACLE_FISHARCH_H_		// ��d�C���N���[�h�h�~

#include "map_obstacle.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���N���X
class CObstacle_FishArch : public CMap_Obstacle
{
public:

	CObstacle_FishArch(int nPriority = 5,
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CObstacle_FishArch();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;
	virtual void Kill() override;

	//=============================
	// �����o�֐�
	//=============================
	CObstacle_FishArch* Create(const CMap_ObstacleManager::SObstacleInfo& info);
	

	//=============================
	// �ÓI�֐�
	//=============================

private:


	//=============================
	// �����o�֐�
	//=============================
	// ���̑�

	//=============================
	// �����o�ϐ�
	//=============================
	CMap_ObstacleManager::SObstacleInfo m_Info;	// �g�p���鋛�̏��
	std::vector<CMap_Obstacle> m_FishList;	// ���̃��X�g

};


#endif
