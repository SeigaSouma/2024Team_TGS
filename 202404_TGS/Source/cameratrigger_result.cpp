//=============================================================================
// 
// カメラモーション処理 [cameratrigger_result.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "cameratrigger_result.h"
#include "manager.h"
#include "camera.h"
#include "subtitle.h"
#include "serifeffect.h"
#include "sound.h"
#include "resultmanager.h"

//==========================================================================
// 関数ポインタ
//==========================================================================
CCameraTrigger_Result::TRIGGER_FUNC CCameraTrigger_Result::m_TriggerFunc[] =
{
	&CCameraTrigger_Result::CameraShake,	// カメラ揺れ
	&CCameraTrigger_Result::OpenScroll,		// 巻き物開封
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCameraTrigger_Result::CCameraTrigger_Result() : CCameraMotion_Trigger()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraTrigger_Result::~CCameraTrigger_Result()
{

}

//==========================================================================
// カメラ揺れ
//==========================================================================
void CCameraTrigger_Result::CameraShake()
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	pCamera->SetShake(18, 25.0f, 25.0f);
}

//==========================================================================
// 巻き物開封
//==========================================================================
void CCameraTrigger_Result::OpenScroll()
{
	// リザルト画面
	CResultManager* pResultManager = CResultManager::GetInstance();
	pResultManager->CreateResultScreen();
}