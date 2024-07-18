//=============================================================================
// 
// カメラモーション処理 [camera_motion.cpp]
// Author : 相馬靜雅
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
// マクロ定義
//==========================================================================
namespace
{
	
}

//==========================================================================
// 関数ポインタ
//==========================================================================
//CCameraMotion::ROCKON_STATE_FUNC CCameraMotion::m_RockOnStateFunc[] =	// カウンター状態
//{
//	&CCameraMotion::RockOnStateNormal,	// 通常
//	&CCameraMotion::RockOnStateCounter,	// カウンター
//};

//==========================================================================
// コンストラクタ
//==========================================================================
CCameraMotion::CCameraMotion()
{
	m_vecMotionInfo.clear();	// モーション情報
	m_nNowMotionIdx = 0;		// 現在のモーションインデックス
	m_fMotionTimer = 0.0f;		// モーションタイマー

	//m_posR = 0.0f;		// 注視点(見たい場所)
	//m_posRDest = 0.0f;	// 目標の注視点
	//m_posV = 0.0f;		// 視点(カメラの位置)
	//m_posVDest = 0.0f;	// 目標の視点
	//m_rot = 0.0f;		// 向き
	//m_rotDest = 0.0f;	// 目標の向き
}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraMotion::~CCameraMotion()
{

}

//==========================================================================
// カメラの初期化処理
//==========================================================================
HRESULT CCameraMotion::Init()
{
	
	

	return S_OK;
}

//==========================================================================
// カメラの終了処理
//==========================================================================
void CCameraMotion::Uninit()
{
	
}

//==========================================================================
// カメラの更新処理
//==========================================================================
void CCameraMotion::Update()
{
	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// 現在のモーション情報
	MotionInfo nowInfo = m_vecMotionInfo[m_nNowMotionIdx];

	// 次のキーインデックス
	int keySize = static_cast<int>(nowInfo.Key.size());
	int nextKeyID = (m_nNowKeyIdx + 1) & keySize;
	if (nextKeyID == 0)
	{// 終端
		nextKeyID = keySize - 1;
	}

	// キー情報
	MotionKey nowKey = nowInfo.Key[m_nNowKeyIdx];
	MotionKey nextKey = nowInfo.Key[nextKeyID];


	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 posR = pCamera->GetPositionR();
	MyLib::Vector3 rot = pCamera->GetRotation();
	float distance = pCamera->GetDistance();

	// 線形補正
	posR = UtilFunc::Correction::EasingLinear(nowKey.posRDest, nextKey.posRDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
	rot = UtilFunc::Correction::EasingLinear(nowKey.rotDest, nextKey.rotDest, 0.0f, nowInfo.playTime, m_fMotionTimer);
	m_fMotionTimer = UtilFunc::Correction::EasingLinear(nowKey.distance, nextKey.distance, 0.0f, nowInfo.playTime, m_fMotionTimer);



}
