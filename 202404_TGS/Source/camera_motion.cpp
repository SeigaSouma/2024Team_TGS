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
	const std::string FILENAME = "data\\TEXT\\camera\\motion_manager.txt";	// 読み込むファイル名
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
	m_pos = 0.0f;				// 位置
	m_nNowMotionIdx = 0;		// 現在のモーションインデックス
	m_fMotionTimer = 0.0f;		// モーションタイマー
	m_bFinish = false;			// 終了判定
}

//==========================================================================
// デストラクタ
//==========================================================================
CCameraMotion::~CCameraMotion()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCameraMotion* CCameraMotion::Create()
{
	// メモリの確保
	CCameraMotion* pObj = DEBUG_NEW CCameraMotion;

	if (pObj == nullptr)
	{
		return nullptr;
	}

	// 初期化処理
	pObj->Init();

	return pObj;
}

//==========================================================================
// カメラの初期化処理
//==========================================================================
HRESULT CCameraMotion::Init()
{
	// 各種変数初期化
	m_bFinish = true;	// 終了判定

	// テキスト読み込み
	LoadText();

	return S_OK;
}

//==========================================================================
// テキスト読み込み
//==========================================================================
void CCameraMotion::LoadText()
{
	// ファイルを開く
	std::ifstream File(FILENAME);
	if (!File.is_open()) {
		return;
	}

	// コメント用
	std::string hoge;

	// ファイル名
	std::string passname, filename;

	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// ストリーム作成
		std::istringstream lineStream(line);

		if (line.find("PASS") != std::string::npos)
		{// PASSで読み込むパス読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				passname;	// パス名

			continue;
		}

		if (line.find("MOTION_FILENAME") != std::string::npos)
		{// MOTION_FILENAMEでモーションファイル読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>		// ＝
				filename;	// モーションファイル名





			continue;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
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
	if (m_bFinish)
	{// モーション中のみ
		return;
	}

	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();

	// 現在のモーション情報
	MotionInfo nowInfo = m_vecMotionInfo[m_nNowMotionIdx];
	int keySize = static_cast<int>(nowInfo.Key.size());


	// モーションタイマー加算
	m_fMotionTimer += CManager::GetInstance()->GetDeltaTime();
	if (m_fMotionTimer >= nowInfo.playTime)
	{
		// キー更新
		m_fMotionTimer = 0.0f;
		m_nNowKeyIdx = (m_nNowKeyIdx + 1) % keySize;

		if (m_nNowKeyIdx == 0)
		{// 一周

			// 終了判定ON
			m_bFinish = true;
		}
	}


	// 次のキーインデックス
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
	MyLib::Vector3 posR, rot;
	float distance = pCamera->GetDistance();

	// 線形補正
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

	// カメラ情報設定
	pCamera->SetPositionR(m_pos + posR);
	pCamera->SetRotation(rot);
	pCamera->SetDistance(distance);
}

//==========================================================================
// モーション設定
//==========================================================================
void CCameraMotion::SetMotion(int motion, EASING EasingType)
{
	m_nNowMotionIdx = motion;
	m_fMotionTimer = 0.0f;
	m_bFinish = false;
	m_EasingType = EasingType;
}
