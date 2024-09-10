//=============================================================================
// 
// �J�������[�V�������� [cameratrigger_result.cpp]
// Author : ���n�Ή�
// 
//=============================================================================
#include "cameratrigger_result.h"
#include "manager.h"
#include "camera.h"
#include "subtitle.h"
#include "serifeffect.h"
#include "sound.h"

//==========================================================================
// �֐��|�C���^
//==========================================================================
CCameraTrigger_Result::TRIGGER_FUNC CCameraTrigger_Result::m_TriggerFunc[] =
{
	&CCameraTrigger_Result::CameraShake,	// �J�����h��
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCameraTrigger_Result::CCameraTrigger_Result() : CCameraMotion_Trigger()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCameraTrigger_Result::~CCameraTrigger_Result()
{

}

//==========================================================================
// �J�����h��
//==========================================================================
void CCameraTrigger_Result::CameraShake()
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	pCamera->SetShake(18, 25.0f, 25.0f);
}
