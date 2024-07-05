//=============================================================================
// 
//  �}�b�v�u���b�N�Ǘ��w�b�_�[ [map_block.h]
//  Author : Ibuki Okusada
// 
//=============================================================================

#ifndef _MAPBLOCK_H_
#define _MAPBLOCK_H_		// ��d�C���N���[�h�h�~

#include "listmanager.h"

// �O���錾
class CCheckpoint;
class CMap_Obstacle;

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�u���b�N�Ǘ��N���X
class CMapBlock
{
public:

	CMapBlock();	// �R���X�g���N�^
	~CMapBlock();	// �f�X�g���N�^

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	// ���o��
	void Save();	// �Z�[�u

	//=============================
	// �ÓI�֐�
	//=============================
	static CListManager<CMapBlock>* Create();	// ����
	static void Kill();	// �J��
	static CListManager<CMapBlock>* GetList() { m_List; }

private:

	//=============================
	// �����o�֐�
	//=============================
	void Load(std::ifstream* pFile);	// ���[�h
	void ObstacleLoad(std::ifstream* pFile); // ��Q���ǂݍ���

	//=============================
	// �����o�ϐ�
	//=============================
	static CListManager<CMapBlock> m_List;	// ���X�g

	// �z�u��񃊃X�g
	CListManager<CMap_Obstacle> m_ObstacleList = {};	// ��Q��
	CListManager<CCheckpoint> m_CheckpointList = {};	// �`�F�b�N�|�C���g
};

#endif
