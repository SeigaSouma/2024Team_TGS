//=============================================================================
// 
//  �`�F�b�N�|�C���g[checkpoint.h]
//  Author : �����V����̃v���O����(���؂肵�Ă܂���)
// 
//=============================================================================

#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
//�`�F�b�N�|�C���g�N���X
class CCheckpoint : public CObjectX
{
public:

	CCheckpoint(int nPriority = 6);
	~CCheckpoint();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// �����o�֐�
	void Kill();		// �폜
	float GetLength() { return m_fLength; }	// �����擾

	//=============================
	// �ÓI�֐�
	//=============================
	static CCheckpoint* Create(const float length);
	static void Load(const std::string filename);
	static int GetSaveID() { return m_nSaveID; }	// �Z�[�uID�擾
	static void ResetSaveID() { m_nSaveID = -1; }	// �Z�[�uID���Z�b�g
	static CListManager<CCheckpoint> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================

	//=============================
	// �����o�֐�
	//=============================
	void SetLength(const float length);	// �����ݒ�

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;				// ��ԃJ�E���^�[
	float m_fLength;				// ����
	static CListManager<CCheckpoint> m_List;	// ���X�g

	static int m_nAll;		// ����
	static int m_nSaveID;	// save id
	int m_MyIndex;			// ������ID
};


#endif