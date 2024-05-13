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

	//=============================
	// �񋓌^��`
	//=============================
	enum Sample
	{
		SAMPLE_WAO = 0,
		SAMPLE_WHO,
		SAMPLE_MAX
	};

	CCheckpoint(int nPriority = 6);
	~CCheckpoint();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// �����o�֐�
	void Kill();		// �폜

	//=============================
	// �ÓI�֐�
	//=============================
	static CCheckpoint* Create(MyLib::Vector3 pos);
	static int GetSaveID() { return m_nSaveID; }	// �Z�[�uID�擾
	static CListManager<CCheckpoint> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �֐����X�g
	//=============================
	typedef void(CCheckpoint::* CHECKPOINT_FUNC)();
	static CHECKPOINT_FUNC m_CheckpointFuncList[];	// �֐��̃��X�g

	//=============================
	// �����o�֐�
	//=============================
	// ��Ԋ֐�
	void SampleWho();	// �t�[
	void SampleWao();	// ���I

	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;				// ��ԃJ�E���^�[
	Sample m_state;					// ���
	static CListManager<CCheckpoint> m_List;	// ���X�g

	static int m_nAll;		// ����
	static int m_nSaveID;	// save id
	int m_MyIndex;			// ������ID
};


#endif