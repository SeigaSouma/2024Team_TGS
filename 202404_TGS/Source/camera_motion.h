//=============================================================================
//
// �J�������[�V�������� [camera_motion.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CAMERA_MOTION_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_MOTION_H_		// ��d�C���N���[�h�h�~�̃}�N�����`����

#include "scene.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �J�������[�V�����N���X
class CCameraMotion
{
public:

	//=============================
	// �񋓌^��`
	//=============================
	enum MOTION
	{
		MOTION_PASS = 0,	// �p�X���[�V����
		MOTION_MAX
	};

	// �␳���
	enum EASING
	{
		Linear = 0,
		EaseIn,
		EaseOut,
		EaseInOut,
		MAX
	};

	CCameraMotion();
	~CCameraMotion();

	//=============================
	// �����o�֐�
	//=============================
	HRESULT Init();		// ������
	void LoadText();	// �e�L�X�g�ǂݍ���
	void Uninit();		// �I��
	void Update();		// �X�V
	void SetMotion(int motion, EASING EasingType);	// ���[�V�����ݒ�

	// ��񑀍�
	void SetPosition(const MyLib::Vector3& pos) { m_pos = pos; }	// �ʒu�ݒ�
	MyLib::Vector3 GetPosition() { return m_pos; }					// �ʒu�擾

	// �ÓI�֐�
	static CCameraMotion* Create();	// ��������

private:

	//=============================
	// �\���̒�`
	//=============================
	struct MotionKey
	{
		MyLib::Vector3 posRDest;	// �ڕW�̒����_
		MyLib::Vector3 rotDest;		// �ڕW�̌���
		float distance;				// ����
	};

	struct MotionInfo
	{
		float playTime;				// �Đ��t���[��
		std::vector<MotionKey> Key;	// �L�[���
	};

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<MotionInfo> m_vecMotionInfo;	// ���[�V�������
	EASING m_EasingType;	// �␳�̎��
	MyLib::Vector3 m_pos;	// �ʒu
	int m_nNowMotionIdx;	// ���݂̃��[�V�����C���f�b�N�X
	int m_nNowKeyIdx;		// ���݂̃L�[�C���f�b�N�X
	float m_fMotionTimer;	// ���[�V�����^�C�}�[
	bool m_bFinish;			// �I������
};

#endif