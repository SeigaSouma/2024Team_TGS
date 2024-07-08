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
// �}�b�v�u���b�N�z�u���܂Ƃ߃N���X
class CMapBlockInfo
{
public:
	// �\���̏��
	struct SObsacleInfo
	{
		MyLib::Vector3 pos;		// ���W
		MyLib::Vector3 rot;		// ����
		MyLib::Vector3 scale;	// �X�P�[��
		int nType;				// ���f�����

		// �R���X�g���N�^
		SObsacleInfo() : pos(0.0f), rot(0.0f), scale(1.0f), nType(0) {}
	};

	CMapBlockInfo();
	~CMapBlockInfo();

	HRESULT Init();
	void Uninit();
	void Load(std::ifstream* pFile);	// ���[�h
	void ObstacleLoad(std::ifstream* pFile); // ��Q���ǂݍ���
	std::vector<SObsacleInfo> GetObstacleInfo() { return m_ObstacleList; }
	std::vector<float> GetCheckpointInfo() { return m_CheckpointList; }

private:

	// �z�u��񃊃X�g
	std::vector<SObsacleInfo> m_ObstacleList;	// ��Q��
	std::vector<float> m_CheckpointList;	// �`�F�b�N�|�C���g
	int m_nLevel;
};

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
	
	void Set(const int Idx, const MyLib::Vector3& startpos, float startlength);
	CListManager<CMap_Obstacle> GetObstacleList() { return m_ObstacleList; }
	CListManager<CCheckpoint> GetCheckpointList() { return m_CheckpointList; }


	//=============================
	// �ÓI�֐�
	//=============================
	static CListManager<CMapBlockInfo>* Load();	// �ǂݍ���
	static void Kill();	// �J��
	static CListManager<CMapBlock> GetList() { return m_List; }
	static CListManager<CMapBlockInfo> GetInfoList() { return m_InfoList; }	// �z�u��񃊃X�g�擾

private:

	//=============================
	// �����o�֐�
	//=============================

	//=============================
	// �����o�ϐ�
	//=============================
	static CListManager<CMapBlock> m_List;	// ���X�g
	static CListManager<CMapBlockInfo> m_InfoList;	// �z�u��񃊃X�g

	// �z�u��񃊃X�g
	CListManager<CMap_Obstacle> m_ObstacleList = {};	// ��Q��
	CListManager<CCheckpoint> m_CheckpointList = {};	// �`�F�b�N�|�C���g
};

#endif
