//=============================================================================
// 
//  �l�}�l�[�W���w�b�_�[ [peoplemanager.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _PEOPLEMANAGER_H_
#define _PEOPLEMANAGER_H_	// ��d�C���N���[�h�h�~

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
	struct SEnemyData
	{
		int nType;			// �L�����N�^�[���
		MyLib::Vector3 pos;	// �ʒu
	};

	struct SPattern
	{
		int nNumEnemy;	// �G�̐�
		std::vector<SEnemyData> enemydata;
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

protected:


private:

	std::vector<SPattern> m_vecPattern;				// �z�u�p�^�[��
	std::vector<std::string> m_vecMotionFileName;	// ���[�V�����t�@�C����
};



#endif