//=============================================================================
//
// カメラモーション処理 [camera_motion.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CAMERA_MOTION_H_		// このマクロ定義がされていなかったら
#define _CAMERA_MOTION_H_		// 二重インクルード防止のマクロを定義する

#include "scene.h"

//==========================================================================
// クラス定義
//==========================================================================
// カメラモーションクラス
class CCameraMotion
{
public:

	enum MOTION
	{
		MOTION_PASS = 0,	// パスモーション
		MOTION_MAX
	};

	CCameraMotion();
	~CCameraMotion();

	// 関数
	HRESULT Init();
	void Uninit();
	void Update();


private:

	// 構造体定義
	struct MotionKey
	{
		MyLib::Vector3 posRDest;	// 目標の注視点
		MyLib::Vector3 rotDest;		// 目標の向き
		float distance;				// 距離
	};

	struct MotionInfo
	{
		float playTime;				// 再生フレーム
		std::vector<MotionKey> Key;	// キー情報
	};

	// メンバ関数


	// メンバ変数
	std::vector<MotionInfo> m_vecMotionInfo;	// モーション情報
	int m_nNowMotionIdx;	// 現在のモーションインデックス
	int m_nNowKeyIdx;	// 現在のキーインデックス
	float m_fMotionTimer;	// モーションタイマー
};

#endif