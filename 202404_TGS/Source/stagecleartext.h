//=============================================================================
// 
// �X�e�[�W�N���A�e�L�X�g���� [stagecleartext.h]
// Author : ���n�Ή�, �Ό��D�n
// 
//=============================================================================

#ifndef _STAGECLEARTEXT_H_
#define _STAGECLEARTEXT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "object2D.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�[�\���N���X��`
class CStageClearText : public CObject2D
{
public:

	CStageClearText(int nPriority = 8);
	~CStageClearText();

	static CStageClearText *Create(const MyLib::Vector3 pos);
	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

private:

	// ��ԗ�
	enum eState
	{
		STATE_EXPANSION = 0,
		STATE_EXPNONE,
		STATE_FADEOUT,
		STATE_MAX
	};

	// �����o�֐�
	void StateExpansion();	// �g��
	void StateExpNone();	// �g��㉽�����Ȃ�
	void StateFadeOut();	// �t�F�[�h�A�E�g���

	// �����o�ϐ�
	eState m_state;		// ���
	float m_fStateTimer;	// ��ԃ^�C�}�[
	bool m_bCreateResultWindow;	// ���U���g��ʂ̌Ăяo���t���O

	typedef void(CStageClearText::*STATE_FUNC)();	// ��ԏ����̃��X�g
	static STATE_FUNC m_StateFuncList[];
};

#endif