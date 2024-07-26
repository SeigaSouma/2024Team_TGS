//=============================================================================
// 
// カメラモーション処理 [cameratrigger_pass.cpp]
// Author : 相馬靜雅
// 
//=============================================================================
#include "cameratrigger_pass.h"
#include "subtitle.h"
#include "sound.h"

//==========================================================================
// 関数ポインタ
//==========================================================================
CCameraTrigger_Pass::TRIGGER_FUNC CCameraTrigger_Pass::m_TriggerFunc[] =
{
	&CCameraTrigger_Pass::waiting,		// 字幕1
	& CCameraTrigger_Pass::newMission,		// 字幕1
	& CCameraTrigger_Pass::carry,		// 字幕1
	& CCameraTrigger_Pass::carefull,		// 字幕1
	& CCameraTrigger_Pass::goodluck,		// 字幕1
};

//==========================================================================
// コンストラクタ
//==========================================================================
CCameraTrigger_Pass::CCameraTrigger_Pass() : CCameraMotion_Trigger()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraTrigger_Pass::~CCameraTrigger_Pass()
{

}

//==========================================================================
// 字幕
//==========================================================================
void CCameraTrigger_Pass::waiting()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\subtitle_006.png");
	pSubTitle->SetSizeByHeight(40.0f);

	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_SEGA_000);
}
void CCameraTrigger_Pass::newMission()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\subtitle_000.png");
	pSubTitle->SetSizeByHeight(40.0f);
}
void CCameraTrigger_Pass::carry()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\subtitle_001.png");
	pSubTitle->SetSizeByHeight(40.0f);

	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_OKSD_000);
}
void CCameraTrigger_Pass::carefull()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 2.0f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\subtitle_002.png");
	pSubTitle->SetSizeByHeight(40.0f);
}
void CCameraTrigger_Pass::goodluck()
{
	CSubTitle* pSubTitle = CSubTitle::Create(MyLib::Vector3(640.0f, 670.0f, 0.0f), 1.2f);
	pSubTitle->BindSubtitle("data\\TEXTURE\\subtitle\\subtitle_003.png");
	pSubTitle->SetSizeByHeight(40.0f);

	CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_HNSW_002);
}


