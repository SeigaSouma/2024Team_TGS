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

		SPeopleData() : nType(0), pos(MyLib::Vector3()) {}
		SPeopleData(int _nType,MyLib::Vector3 _pos) : nType(_nType), pos(_pos) {}

		bool operator==(const SPeopleData& other) const
		{
			return (this->nType == other.nType && this->pos == other.pos) ? true : false;
		}
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
	void SetPeople(const MyLib::Vector3& pos, const MyLib::Vector3& rot, int nPattern);	// �G�z�u
	void SetByRank();	// �����N���Ƃ̃Z�b�g����
	void DespawnPeople();	// �͈͊O�̐l������
	void LateSpawn();		// �㐶�����̐���
	void SetRank(const CJudge::JUDGE& judge) { m_Rank = judge; }			// �����N�ݒ�
	void ResetLateSpawn() { m_lateSpawnPeople.clear(); }

	static CPeopleManager* GetInstance() { return m_ThisPtr; }				// ���g�̃|�C���^


private:

	// �����o�ϐ�
	CJudge::JUDGE m_Rank;							// ���݂̃����N
	CJudge::JUDGE m_OldRank;						// �O��̃����N
	std::vector<SPattern> m_vecPattern;				// �z�u�p�^�[��
	std::vector<SPattern> m_PatternByRank[CJudge::JUDGE::JUDGE_MAX];			// �z�u�p�^�[��
	std::list<SPeopleData> m_lateSpawnPeople;		// �㐶���̐l���X�g
	std::vector<std::string> m_vecMotionFileName;	// ���[�V�����t�@�C����
	static CPeopleManager* m_ThisPtr;				// ���g�̃|�C���^
};



#endif