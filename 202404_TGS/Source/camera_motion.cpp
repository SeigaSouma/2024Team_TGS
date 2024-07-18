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
	const std::string FILENAME = "data\\TEXT\\camera\\motion_manager.txt";	// �ǂݍ��ރt�@�C����
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
	m_pos = 0.0f;				// �ʒu
	m_nNowMotionIdx = 0;		// ���݂̃��[�V�����C���f�b�N�X
	m_fMotionTimer = 0.0f;		// ���[�V�����^�C�}�[
	m_bFinish = false;			// �I������
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCameraMotion::~CCameraMotion()
{

}

//==========================================================================
// ��������
//==========================================================================
CCameraMotion* CCameraMotion::Create()
{
	// �������̊m��
	CCameraMotion* pObj = DEBUG_NEW CCameraMotion;

	if (pObj == nullptr)
	{
		return nullptr;
	}

	// ����������
	pObj->Init();

	return pObj;
}

//==========================================================================
// �J�����̏���������
//==========================================================================
HRESULT CCameraMotion::Init()
{
	// �e��ϐ�������
	m_bFinish = true;	// �I������

	// �e�L�X�g�ǂݍ���
	LoadText();

	return S_OK;
}

//==========================================================================
// �e�L�X�g�ǂݍ���
//==========================================================================
void CCameraMotion::LoadText()
{
	// �t�@�C�����J��
	std::ifstream File(FILENAME);
	if (!File.is_open()) {
		return;
	}

	// �R�����g�p
	std::string hoge;

	// �t�@�C����
	std::string passname, filename;

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// �X�g���[���쐬
		std::istringstream lineStream(line);

		if (line.find("PASS") != std::string::npos)
		{// PASS�œǂݍ��ރp�X�ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				passname;	// �p�X��

			continue;
		}

		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// MOTION_FILENAME�Ń��[�V�����t�@�C���ǂݍ���

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>		// ��
				filename;	// ���[�V�����t�@�C����





			continue;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
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
	if (m_bFinish)
	{// ���[�V�������̂�
		return;
	}

	// �L�[�{�[�h���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// ���݂̃��[�V�������
	MotionInfo nowInfo = m_vecMotionInfo[m_nNowMotionIdx];
	int keySize = static_cast<int>(nowInfo.Key.size());


	// ���[�V�����^�C�}�[���Z
	m_fMotionTimer += CManager::GetInstance()->GetDeltaTime();
	if (m_fMotionTimer >= nowInfo.playTime)
	{
		// �L�[�X�V
		m_fMotionTimer = 0.0f;
		m_nNowKeyIdx = (m_nNowKeyIdx + 1) % keySize;

		if (m_nNowKeyIdx == 0)
		{// ���

			// �I������ON
			m_bFinish = true;
		}
	}


	// ���̃L�[�C���f�b�N�X
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
	MyLib::Vector3 posR, rot;
	float distance = pCamera->GetDistance();

	// ���`�␳
	switch (m_EasingType)
	{
	case CCameraMotion::Linear:
		posR = UtilFunc::Correction::EasingLinear(nowKey.posRDest, nextKey.posRDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
		rot = UtilFunc::Correction::EasingLinear(nowKey.rotDest, nextKey.rotDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
		distance = UtilFunc::Correction::EasingLinear(nowKey.distance, nextKey.distance, 0.0f, nowInfo.playTime, m_fMotionTimer);
		break;

	case CCameraMotion::EaseIn:
		posR = UtilFunc::Correction::EasingEaseIn(nowKey.posRDest, nextKey.posRDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
		rot = UtilFunc::Correction::EasingEaseIn(nowKey.rotDest, nextKey.rotDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
		distance = UtilFunc::Correction::EasingEaseIn(nowKey.distance, nextKey.distance, 0.0f, nowInfo.playTime, m_fMotionTimer);
		break;

	case CCameraMotion::EaseOut:
		posR = UtilFunc::Correction::EasingEaseOut(nowKey.posRDest, nextKey.posRDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
		rot = UtilFunc::Correction::EasingEaseOut(nowKey.rotDest, nextKey.rotDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
		distance = UtilFunc::Correction::EasingEaseOut(nowKey.distance, nextKey.distance, 0.0f, nowInfo.playTime, m_fMotionTimer);
		break;

	case CCameraMotion::EaseInOut:
		posR = UtilFunc::Correction::EasingEaseInOut(nowKey.posRDest, nextKey.posRDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
		rot = UtilFunc::Correction::EasingEaseInOut(nowKey.rotDest, nextKey.rotDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
		distance = UtilFunc::Correction::EasingEaseInOut(nowKey.distance, nextKey.distance, 0.0f, nowInfo.playTime, m_fMotionTimer);
		break;
	}

	// �J�������ݒ�
	pCamera->SetPositionR(m_pos + posR);
	pCamera->SetRotation(rot);
	pCamera->SetDistance(distance);
}

//==========================================================================
// ���[�V�����ݒ�
//==========================================================================
void CCameraMotion::SetMotion(int motion, EASING EasingType)
{
	m_nNowMotionIdx = motion;
	m_fMotionTimer = 0.0f;
	m_bFinish = false;
	m_EasingType = EasingType;
}
