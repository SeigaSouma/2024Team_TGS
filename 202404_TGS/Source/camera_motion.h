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

	enum MOTION
	{
		MOTION_PASS = 0,	// �p�X���[�V����
		MOTION_MAX
	};

	CCameraMotion();
	~CCameraMotion();

	// �֐�
	HRESULT Init();
	void Uninit();
	void Update();


private:

	// �\���̒�`
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

	// �����o�֐�


	// �����o�ϐ�
	std::vector<MotionInfo> m_vecMotionInfo;	// ���[�V�������
	int m_nNowMotionIdx;	// ���݂̃��[�V�����C���f�b�N�X
	int m_nNowKeyIdx;	// ���݂̃L�[�C���f�b�N�X
	float m_fMotionTimer;	// ���[�V�����^�C�}�[
};

#endif