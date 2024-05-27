//=============================================================================
//
// �L�[�R���t�B�O�w�b�_�[ [keyconfig.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_H_
#define _KEYCONFIG_H_	// ��d�C���N���[�h�h�~

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�R���t�B�O
class CKeyConfig
{
public:

	// �����ޗ�
	enum Control
	{
		CONTROL_INPAD = 0,	// �C���Q�[���p�b�h
		CONTROL_OUTPAD,		// �A�E�g�Q�[���p�b�h
		CONTROL_INKEY,		// �C���Q�[���L�[�{�[�h
		CONTROL_OUTKEY,		// �A�E�g�Q�[���L�[�{�[�h
		CONTROL_MAX
	};

public:
	// �R���X�g���N�^
	CKeyConfig(Control type);

	//=============================
	// �����o�֐�
	//=============================
	
	// �ÓI�����o�֐�
	static CKeyConfig* GetKeyConfig(Control type);
	static void Release();
	virtual void Uninit() = 0;
	virtual bool GetPress(const int type) = 0;
	virtual bool GetTrigger(const int type) = 0;
	virtual bool GetRelease(const int type) = 0;
	virtual bool GetRepeat(const int type) = 0;

private:

	// �ÓI�����o�ϐ�
	static CKeyConfig* m_apKeyConfig[CKeyConfig::Control::CONTROL_MAX];	// ���i�[�ꏊ
};

#endif