//=============================================================================
// 
//  �R�[�X�}�l�[�W���w�b�_�[ [courseManager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _COURSEMANAGER_H_
#define _COURSEMANAGER_H_		// ��d�C���N���[�h�h�~

#include "listmanager.h"
class CMapBlock;

//==========================================================================
// �N���X��`
//==========================================================================
// �R�[�X�}�l�[�W���N���X
class CCourseManager
{
public:

	CCourseManager();
	~CCourseManager();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	//=============================
	// �����o�֐�
	//=============================
	void Kill();	// �폜

	// ���o��
	void Save();	// �Z�[�u
	void Load();	// ���[�h

	std::vector<std::vector<MyLib::Vector3>> GetSegmentPos() { return m_vecAllSegmentPos; }
	std::vector<MyLib::Vector3> GetSegmentPos(int idx) { return m_vecAllSegmentPos[idx]; }
	void SetSegmentPos(const std::vector<std::vector<MyLib::Vector3>>& vec) { m_vecAllSegmentPos = vec; }
	void SetSegmentPos(const std::vector<MyLib::Vector3>& vec, int idx) { m_vecAllSegmentPos[idx] = vec; }
	int GetSegmentSize() { return static_cast<int>(m_vecAllSegmentPos.size()); }	// ��_�̃T�C�Y


	//=============================
	// �ÓI�֐�
	//=============================
	static CCourseManager* Create();	// ����
	static CCourseManager* GetInstance() { return m_ThisPtr; }	// �C���X�^���X�擾
	static const float GetBlockLength() { return m_fBlockLength; }	// �u���b�N�̒����擾
private:

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<std::vector<MyLib::Vector3>> m_vecAllSegmentPos;	// �S�Ă̋N�_�̈ʒu
	static CCourseManager* m_ThisPtr;		// ���g�̃|�C���^
	static const float m_fBlockLength;	// �u���b�N�̒���
};


#endif