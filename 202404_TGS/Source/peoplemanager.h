//=============================================================================
// 
//  �l�}�l�[�W���w�b�_�[ [peoplemanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PEOPLEMANAGER_H_
#define _PEOPLEMANAGER_H_	// ��d�C���N���[�h�h�~

#include "judge.h"

//==========================================================================
// �O���錾
//==========================================================================
class CPeople;

//==========================================================================
// �N���X��`
//==========================================================================
// �l�}�l�[�W���N���X
class CPeopleManager
{
public:

	// �\���̒�`
	struct SPeopleData
	{
		int nType;			// �L�����N�^�[���
		MyLib::Vector3 pos;	// �ʒu
	};

	struct SPattern
	{
		int nNum;	// ����
		int nRank;	// �����N
		std::vector<SPeopleData> data;
	};

	// �񋓌^��`
	enum STATE
	{
		STATE_NONE = 0,		// �����Ȃ����
		STATE_MAX
	};

	CPeopleManager();
	~CPeopleManager();

	HRESULT Init();
	void Uninit();
	void Update();

	static CPeopleManager *Create();
	HRESULT ReadText(const std::string& filename);	// �O���t�@�C���ǂݍ��ݏ���
	void SetPeople(MyLib::Vector3 pos, MyLib::Vector3 rot, int nPattern);	// �G�z�u
	void SetByRank();	// �����N���Ƃ̃Z�b�g����

	void SetRank(const CJudge::JUDGE& judge) { m_Rank = judge; }			// �����N�ݒ�

	static CPeopleManager* GetInstance() { return m_ThisPtr; }				// ���g�̃|�C���^


private:

	// �����o�ϐ�
	CJudge::JUDGE m_Rank;							// ���݂̃����N
	CJudge::JUDGE m_OldRank;						// �O��̃����N
	std::vector<SPattern> m_vecPattern;				// �z�u�p�^�[��
	std::vector<SPattern> m_PatternByRank[CJudge::JUDGE::JUDGE_MAX];			// �z�u�p�^�[��
	std::vector<std::string> m_vecMotionFileName;	// ���[�V�����t�@�C����
	static CPeopleManager* m_ThisPtr;				// ���g�̃|�C���^
};



#endif