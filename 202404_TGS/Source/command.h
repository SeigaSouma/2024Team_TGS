//=============================================================================
//
// �R�}���h�w�b�_�[ [command.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _COMMAND_H_
#define _COMMAND_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�R���t�B�O
class CCommand
{
public:

	CCommand();
	virtual ~CCommand() {}	// �f�X�g���N�^

	// ��{����
	void Uninit();	// �I������
	virtual bool GetCommand() = 0;	// ���͊m�F

protected:

	// �����o�֐�
	void SetCommand(int nKey);	// �R�}���h�L�[�ݒ�
	void Reset();

	// �擾�n�����o�֐�
	int GetNowFlame() { return m_nNowFlame; }	// ���݂̃t���[���擾
	int GetMaxFlame() { return m_nMaxFlame; }	// �ő�t���[���擾

	// �ݒ�n�����o�֐�
	void SetNowFlame(int nFlame);	// ���݂̃t���[���ݒ�	
	void SetMaxFlame(int nFlame) { m_nMaxFlame = nFlame; }	// �ő�t���[���ݒ�

	// �����o�ϐ�
	std::map<int, bool> m_Info;	// ���

private:

	// �����o�ϐ�
	int m_nNowFlame;	// ���݂̃t���[��
	int m_nMaxFlame;	// ���͎�t�ő�t���[��
};

//==========================================================================
// �h���N���X��`
//==========================================================================
// �Q�[���p�b�h
class CCommandPad : public CCommand
{
private:

	// ���͕�����
	enum DIRECTION
	{
		STICK_NONE = 0,		// �����ĂȂ�
		STICK_UP,			// ��
		STICK_DOWN,			// ��
		STICK_LEFT,			// ��
		STICK_RIGHT,		// �E
		STICK_LEFTUP,		// ����	
		STICK_RIGHTUP,		// �E��
		STICK_LEFTDOWN,		// ����
		STICK_RIGHTDOWN,	// �E��
		STICK_MAX
	};

public:

	CCommandPad() {}	// �R���X�g���N�^
	~CCommandPad() override {}	// �f�X�g���N�^

	// �����o�֐�
	bool GetCommand() override;	// ���͊m�F
	DIRECTION GetDirection();	// ���͕����擾

private:
};

#endif