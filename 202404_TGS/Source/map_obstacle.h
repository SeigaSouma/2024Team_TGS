//=============================================================================
// 
//  �}�b�v�̏�Q���w�b�_�[ [map_obstacle.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _MAP_OBSTACLE_H_
#define _MAP_OBSTACLE_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "listmanager.h"
#include "map_obstacleManager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�̏�Q���N���X
class CMap_Obstacle : public CObjectX
{
private:

	enum T
	{
		SAKANA = 12,	// ��
		BIRD = 13,		// ��
	};

public:

	CMap_Obstacle(int nPriority = 5, 
		CObject::LAYER layer = CObject::LAYER::LAYER_MAP);
	~CMap_Obstacle();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	virtual void Kill();	// �폜
	void Save();	// �Z�[�u
	void Load();	// ���[�h
	bool GetSave() { return m_bSave; }
	CMap_ObstacleManager::SObstacleInfo GetObstacleInfo() { return m_ObstacleInfo; }	// ��Q�����擾
	void SetObstacleInfo(const CMap_ObstacleManager::SObstacleInfo& info) { m_ObstacleInfo = info; }	// ��Q�����ݒ�

	//=============================
	// �ÓI�֐�
	//=============================
	static CMap_Obstacle *Create(const CMap_ObstacleManager::SObstacleInfo& info, const bool bChange = true, const bool bSave = true);	// ��������
	static CListManager<CMap_Obstacle> GetListObj() { return m_List; }				// ���X�g�擾

private:

	
	//=============================
	// �����o�֐�
	//=============================
	// ���̑�

	//=============================
	// �����o�ϐ�
	//=============================
	TYPE m_type;			// ���
	CMap_ObstacleManager::SObstacleInfo m_ObstacleInfo;	// ��Q�����
	CMap_ObstacleManager::SObstacleInfo m_OriginObstacleInfo;	// ��Q�����
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// �����蔻��{�b�N�X
	static CListManager<CMap_Obstacle> m_List;	// ���X�g
	bool m_bSave;			// �ۑ����邩�ǂ���

};


#endif