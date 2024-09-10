//=============================================================================
//
// �L�[�R���t�B�O�ݒ�w�b�_�[ [keyconfig_setting.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _KEYCONFIG_SETTING_H_
#define _KEYCONFIG_SETTING_H_	// ��d�C���N���[�h�h�~

#include "keyconfig.h"

// �O���錾
class CControlKeyDisp;
class CScroll;

//==========================================================================
// �N���X��`
//==========================================================================
// �L�[�R���t�B�O
class CKeyConfigSetting
{
private:

	// �L�[���
	struct KEYINFO
	{
		CObject2D* s_p2D;				// 2D�|���S��
		CObject2D* s_p2DFront;			// �\��
		CControlKeyDisp* s_pKeyDisp;	// �L�[�R���t�B�O�\��

		// �R���X�g���N�^
		KEYINFO() { s_p2D = nullptr; s_pKeyDisp = nullptr; s_p2DFront = nullptr; }
	};

	// �؂�ւ����
	struct CHENGEINFO
	{
		CObject2D* s_p2D;				// 2D�|���S��
		CObject2D* s_p2DFront;			// �\��
		CControlKeyDisp* s_pKeyDispOK;	// �L�[�R���t�B�O�\��
		CControlKeyDisp* s_pKeyDispNO;	// �L�[�R���t�B�O�\��

		// �R���X�g���N�^
		CHENGEINFO() { s_p2D = nullptr; s_pKeyDispOK = nullptr; s_pKeyDispNO = nullptr; s_p2DFront = nullptr; }
	};

public:

	// �R���X�g���N�^
	CKeyConfigSetting();
	~CKeyConfigSetting() {}

	//=============================
	// �����o�֐�
	//============================= 
	static CKeyConfigSetting* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	bool IsDeath(void) { return m_bDeath; }
	bool IsChange(void) { return m_bNowChange; }

private:

	// �����o�֐�
	void Chenge();
	void SetAlpha();

	// �����o�ϐ�
	float m_Alpha;
	float m_fTime;
	int m_SelectKey;	// �I��
	bool m_bNowChange;	// �ύX�����ۂ�
	bool m_bDeath;		// ���S�t���O
	KEYINFO m_aKeyConfig[INGAME::ACT_MAX];	// �L�[�R���t�B�O�p
	CHENGEINFO m_checkconfig;	// ����߂�ύX�p
	CObject2D* m_pTitle2D;		// �����p�|���S��
	CScroll* m_pScroll;
};

#endif