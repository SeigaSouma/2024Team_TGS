//=============================================================================
// 
// �^�C�g�����S���� [titlelogo.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

class CObject2D;

//==========================================================================
// �N���X��`
//==========================================================================
// �^�C�g�����S
class CTitleLogo : public CObject
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum State
	{
		STATE_NONE = 0,			// �Ȃɂ��Ȃ�
		STATE_FADEIN_WATER,		// ���t�F�[�h�C��
		STATE_FADEIN_PLAYER_and_NAME,	// �v���C���[�Ɩ��O�t�F�[�h�C��
		STATE_BRESS,			// ��
		STATE_AFTERMOVEMENT,	// ��̓���
		STATE_MAX
	};

	enum LOGO_LOVE	// LOVE����
	{
		L = 0,
		O,
		V,
		E,
		MAX
	};

	CTitleLogo(float fadetime, int nPriority = 0);
	~CTitleLogo();
	
	//  �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void SetState(State state);	// ��Ԑݒ�
	static CTitleLogo* Create(float fadetime);	// ��������

private:

	//=============================
	// �֐��|�C���^
	//=============================
	typedef void(CTitleLogo::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// �����o�֐�
	//=============================
	// ���
	void UpdateState();	// ��ԍX�V
	void StateNone();			// �Ȃɂ��Ȃ�
	void StateFadeIn_Water();	// ���t�F�[�h�C��
	void StateFadeIn_PlayerAndName();	// �v���C���[�Ɩ��O�t�F�[�h�C��
	void StateBress();			// ��
	void StateAfterMovement();	// ��̓���

	// ���̑�
	void CreateWater();		// ������
	void CreatePlayer();	// �v���C���[����
	void CreateBress();		// ������
	void CreateLOVE();		// LOVE����
	void CreateSUITON();	// SUITON����

	//=============================
	// �����o�ϐ�
	//=============================
	State m_state;		// ���
	float m_fStateTime;	// ��ԃJ�E���^�[

	CObject2D* m_pWater;	// ��
	CObject2D* m_pPlayer;	// �v���C���[
	CObject2D* m_pBress;	// ��
	CObject2D* m_pLOVE[LOGO_LOVE::MAX];	// LOVE
	CObject2D* m_pSUITON;	// SUITON

	const float m_fFadeOutTime;	// �t�F�[�h�ɂ����鎞��

};

#endif