//=============================================================================
// 
//  ��Q���G�f�B�^�\�w�b�_�[ [edit_obstacle.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_OBSTACLE_H_
#define _EDIT_OBSTACLE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ��Q���G�f�B�^�\�N���X
class CEdit_Obstacle : public CEdit
{
public:

	CEdit_Obstacle();
	~CEdit_Obstacle();
	
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;



private:

	//=============================
	// �\���̒�`
	//=============================
	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	//=============================
	// �����o�֐�
	//=============================
	void Resize();
	void MenuBar();
	void CreateBoxLine();	// �����蔻��{�b�N�X����
	void DeleteBoxLine();	// �����蔻��{�b�N�X�폜

	//=============================
	// �����o�ϐ�
	//=============================
	int m_nEditIdx;						// ��������C���f�b�N�X
	int m_nColliderIdx;					// ��������R���C�_�[�̃C���f�b�N�X
	std::vector<CObjectX*> m_pObjX;		// �I�u�W�F�N�gX�̃|�C���^
	MyLib::AABB m_AABB;	// AABB���
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// �����蔻��{�b�N�X
};

#endif