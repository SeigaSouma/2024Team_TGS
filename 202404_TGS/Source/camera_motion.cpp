//=============================================================================
// 
// �J�������[�V�������� [camera_motion.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "camera_motion.h"
#include "camera.h"
#include "debugproc.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "calculation.h"

//==========================================================================
// �}�N����`
//==========================================================================
namespace
{
	
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
//CCameraMotion::ROCKON_STATE_FUNC CCameraMotion::m_RockOnStateFunc[] =	// �J�E���^�[���
//{
//	&CCameraMotion::RockOnStateNormal,	// �ʏ�
//	&CCameraMotion::RockOnStateCounter,	// �J�E���^�[
//};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCameraMotion::CCameraMotion()
{
	m_vecMotionInfo.clear();	// ���[�V�������
	m_nNowMotionIdx = 0;		// ���݂̃��[�V�����C���f�b�N�X
	m_fMotionTimer = 0.0f;		// ���[�V�����^�C�}�[

	//m_posR = 0.0f;		// �����_(�������ꏊ)
	//m_posRDest = 0.0f;	// �ڕW�̒����_
	//m_posV = 0.0f;		// ���_(�J�����̈ʒu)
	//m_posVDest = 0.0f;	// �ڕW�̎��_
	//m_rot = 0.0f;		// ����
	//m_rotDest = 0.0f;	// �ڕW�̌���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCameraMotion::~CCameraMotion()
{

}

//==========================================================================
// �J�����̏���������
//==========================================================================
HRESULT CCameraMotion::Init()
{
	
	

	return S_OK;
}

//==========================================================================
// �J�����̏I������
//==========================================================================
void CCameraMotion::Uninit()
{
	
}

//==========================================================================
// �J�����̍X�V����
//==========================================================================
void CCameraMotion::Update()
{
	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// ���݂̃��[�V�������
	MotionInfo nowInfo = m_vecMotionInfo[m_nNowMotionIdx];

	// ���̃L�[�C���f�b�N�X
	int keySize = static_cast<int>(nowInfo.Key.size());
	int nextKeyID = (m_nNowKeyIdx + 1) & keySize;
	if (nextKeyID == 0)
	{// �I�[
		nextKeyID = keySize - 1;
	}

	// �L�[���
	MotionKey nowKey = nowInfo.Key[m_nNowKeyIdx];
	MotionKey nextKey = nowInfo.Key[nextKeyID];


	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 posR = pCamera->GetPositionR();
	MyLib::Vector3 rot = pCamera->GetRotation();
	float distance = pCamera->GetDistance();

	// ���`�␳
	posR = UtilFunc::Correction::EasingLinear(nowKey.posRDest, nextKey.posRDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
	rot = UtilFunc::Correction::EasingLinear(nowKey.rotDest, nextKey.rotDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
	m_fMotionTimer = UtilFunc::Correction::EasingLinear(nowKey.distance, nextKey.distance, 0.0f, nowInfo.playTime, m_fMotionTimer);



}
