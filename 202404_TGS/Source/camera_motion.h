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

	//=============================
	// 列挙型定義
	//=============================
	enum MOTION
	{
		MOTION_PASS = 0,	// パスモーション
		MOTION_MAX
	};

	// 補正種類
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
	// メンバ関数
	//=============================
	HRESULT Init();		// 初期化
	void LoadText();	// テキスト読み込み
	void Uninit();		// 終了
	void Update();		// 更新
	void SetMotion(int motion, EASING EasingType);	// モーション設定

	// 情報操作
	void SetPosition(const MyLib::Vector3& pos) { m_pos = pos; }	// 位置設定
	MyLib::Vector3 GetPosition() { return m_pos; }					// 位置取得

	// 静的関数
	static CCameraMotion* Create();	// 生成処理

private:

	//=============================
	// 構造体定義
	//=============================
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

	//=============================
	// メンバ変数
	//=============================
	std::vector<MotionInfo> m_vecMotionInfo;	// モーション情報
	EASING m_EasingType;	// 補正の種類
	MyLib::Vector3 m_pos;	// 位置
	int m_nNowMotionIdx;	// 現在のモーションインデックス
	int m_nNowKeyIdx;		// 現在のキーインデックス
	float m_fMotionTimer;	// モーションタイマー
	bool m_bFinish;			// 終了判定
};

#endif