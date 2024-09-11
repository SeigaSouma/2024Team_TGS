//=============================================================================
// 
// �^�C�g���I�� [title_select.h]
// Author : Ibuki Okusada
// 
//=============================================================================
#ifndef _TITLE_SELECT_H_
#define _TITLE_SELECT_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object2D.h"

class CObject2D;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g���G���^�[
class CTitle_Select
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum STATE
	{
		STATE_NONE = 0,			// �Ȃɂ��Ȃ�
		STATE_FADEIN,			// �t�F�[�h�C��
		STATE_FADEOUT,			// �t�F�[�h�A�E�g
		STATE_TUTORIAL_FADEOUT,	// �`���[�g���A���m�F�̃t�F�[�h�A�E�g
		STATE_NOACTIVE,			// �������Ȃ�
		STATE_SETTING,			// �ݒ蒆
		STATE_MAX
	};

	enum SELECT
	{
		SELECT_START,	// �J�n
		SELECT_OPTION,	// �ݒ�
		SELECT_MAX
	};

	CTitle_Select(float fadetime);
	~CTitle_Select() {}

	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void SetDraw(const bool bDraw = true);

	void SetState(STATE state);
	STATE GetState() { return m_state; }

	static CTitle_Select* Create(float fadetime);	// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTitle_Select::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	void StateNone();			// �Ȃɂ��Ȃ�
	void StateFadeIn();			// �t�F�[�h�C��
	void StateFadeOut();			// �t�F�[�h�A�E�g
	void StateTutorial_FadeOut();		// �`���[�g���A���m�F�̃t�F�[�h�A�E�g
	void StateNoActive();		// �������Ȃ�
	void StateSetting();		// �������Ȃ�

	//=============================
	// �����o�ϐ�
	//=============================
	STATE m_state;				// ���
	float m_fStateTime;			// ��ԃJ�E���^�[
	const float m_fFadeOutTime;	// �t�F�[�h�ɂ����鎞��
	CObject2D* m_ap2D[SELECT_MAX];	// �I����
	CObject2D* m_pSelect;			// �w�i�M
	int m_nSelect;				// 
	bool m_bPress;
};

#endif