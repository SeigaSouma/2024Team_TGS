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
public:
	
	//=============================
	// �񋓌^��`
	//=============================

	CMap_Obstacle(int nPriority = 6);
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
	void Kill();	// �폜
	void Save();	// �Z�[�u
	void Load();	// ���[�h


	//=============================
	// �ÓI�֐�
	//=============================
	static CMap_Obstacle *Create(const CMap_ObstacleManager::SObstacleInfo& info);	// ��������
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
	static CListManager<CMap_Obstacle> m_List;	// ���X�g

};


#endif