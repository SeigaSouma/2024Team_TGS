//=============================================================================
// 
//  �}�b�v�u���b�N�Ǘ��w�b�_�[ [map_block.h]
//  Author : Ibuki Okusada
// 
//=============================================================================

#ifndef _MAPBLOCK_H_
#define _MAPBLOCK_H_		// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "waterstoneManager.h"

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
		SObsacleInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot, const MyLib::Vector3& _scale, int _type) : 
			pos(_pos), rot(_rot), scale(_scale), nType(_type) {}
	};

	CMapBlockInfo();
	~CMapBlockInfo();

	HRESULT Init();
	void Uninit();
	void SaveBin();	// �Z�[�u
	void Load(std::ifstream* pFile);	// ���[�h
	void ObstacleLoad(std::ifstream* pFile); // ��Q���ǂݍ���
	
	// ���X�g
	void SetObstacleInfo(const std::vector<SObsacleInfo>& info) { m_ObstacleList = info; }
	std::vector<SObsacleInfo> GetObstacleInfo() { return m_ObstacleList; }
	void SetCheckpointInfo(const std::vector<float>& list) { m_CheckpointList = list; }
	std::vector<float> GetCheckpointInfo() { return m_CheckpointList; }
	void SetWaterStoneInfo(const std::vector<CWaterStone_Manager::SStoneInfo>& list) { m_WaterStoneList = list; }	// ������
	std::vector<CWaterStone_Manager::SStoneInfo> GetWaterStoneInfo() { return m_WaterStoneList; }					// ������

	void SetLevel(int level) { m_nLevel = level; }	// ���x���ݒ�
	int GetLevel() { return m_nLevel; }				// ���x���擾

private:

	// �z�u��񃊃X�g
	std::vector<SObsacleInfo> m_ObstacleList;	// ��Q��
	std::vector<float> m_CheckpointList;	// �`�F�b�N�|�C���g
	std::vector<CWaterStone_Manager::SStoneInfo> m_WaterStoneList;	// ������
	int m_nLevel;	// ��Փx
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
	
	void Set(const int Idx, const MyLib::Vector3& startpos, float startlength);
	CListManager<CMap_Obstacle> GetObstacleList() { return m_ObstacleList; }
	CListManager<CCheckpoint> GetCheckpointList() { return m_CheckpointList; }

	void AddInfo();	// ���ǉ�
	void SubInfo();	// ���폜

	//=============================
	// �ÓI�֐�
	//=============================
	static CListManager<CMapBlockInfo>* Load();	// �ǂݍ���
	static void SaveBin();	// �Z�[�u
	static void SaveBin_CheckPoint();	// �`�F�b�N�|�C���g�Z�[�u
	static void SaveBin_Obstacle();		// ��Q���Z�[�u
	static void SaveBin_WaterStone();	// ������Z�[�u
	static void SaveBin_Level();		// ���x���Z�[�u
	static void LoadBin();	// ���[�h
	static void Kill();	// �J��
	static CListManager<CMapBlock> GetList() { return m_List; }
	static void SetInfoList(const CListManager<CMapBlockInfo>& info) { m_InfoList = info; }	// �z�u��񃊃X�g�擾
	static CListManager<CMapBlockInfo> GetInfoList() { return m_InfoList; }	// �z�u��񃊃X�g�擾
	static CListManager<CMapBlockInfo>* GetInfoListPtr() { return &m_InfoList; }	// �z�u��񃊃X�g�擾
	static void Delete();

private:

	//=============================
	// �����o�֐�
	//=============================
	static std::vector<std::vector<float>> LoadBin_CheckPoint();							// �`�F�b�N�|�C���g�ǂݍ���
	static std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> LoadBin_Obstacle();		// ��Q���ǂݍ���
	static std::vector<std::vector<CWaterStone_Manager::SStoneInfo>> LoadBin_WaterStone();	// ������ǂݍ���
	static std::vector<int> LoadBin_Level(size_t courseSize);								// ���x���ǂݍ���
	void DataDelete();
	void InfoDelete();

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
