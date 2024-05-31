//=============================================================================
// 
//  プレイヤーコントロールヘッダー [playercontrol.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYERCONTROL_H_
#define _PLAYERCONTROL_H_	// 二重インクルード防止

#include "player.h"
#include "baggage.h"

#if _DEBUG
#include "debug_bressrange.h"
#endif

//==========================================================================
// プレイヤーコントロールクラス定義
//==========================================================================
//=============================
// 移動
//=============================
class CPlayerControlMove
{
public:

	// コンストラクタ
	CPlayerControlMove() {}

	virtual void Move(CPlayer* player);	// 移動

};

//=============================
// 荷物操作
//=============================
class CPlayerControlBaggage
{
public:

	// コンストラクタ
	CPlayerControlBaggage() 
	{
		m_BressHandle = nullptr;
	}

	virtual void Action(CPlayer* player, CBaggage* pBaggage);	// アクション

private:

	// メンバ関数
	bool CollisionObstacle(CPlayer* player, CBaggage* pBaggage);	// 障害物判定

	// メンバ変数
	Effekseer::Handle* m_BressHandle = nullptr;

#if _DEBUG
	CDebugBressRange* m_pBressRange = nullptr;		// 息範囲
	CDebugBressRange* m_pBressHeight = nullptr;		// 息の届く高さ範囲
#endif

	float m_fHeight = 0.0f;				// 現在の息高さ
	float m_fMaxHeight = 1000.0f;		// 息の届く最大の高さ
	float m_fTimeDownHeight = 0.0f;		// 高さの降下時間
	float m_fHeightVelocity = 10.0f;	// 息の加算量

#if _DEBUG
	CCollisionLine_Box* m_pBox = nullptr;	// デバッグ用当たり判定
#endif
};

//=============================
// 浮上操作
//=============================
class CPlayerControlSurfacing
{
public:

	// コンストラクタ
	CPlayerControlSurfacing() {}

	virtual float Surfacing(CPlayer* player);	// 移動

private:

	float m_fHeight = 0.0f;	// 浮上高さ
};

#endif