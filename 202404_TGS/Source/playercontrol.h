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
	Effekseer::Handle* m_BressHandle = nullptr;
	CDebugBressRange* m_pBressRange = nullptr;
	CDebugBressRange* m_pBressHeight = nullptr;
	float m_fHeight = 0.0f;
	float m_fHeightVelocity = 10.0f;
};

//=============================
// 攻撃
//=============================
class CPlayerControlAttack
{
public:

	CPlayerControlAttack() : m_bAttackReserved(false) {}

	virtual void Attack(CPlayer* player);	// 通常攻撃
	bool IsReserve() { return m_bAttackReserved; }

protected:

	virtual bool IsAttack(CPlayer* player);	// 攻撃可能フラグ取得

	virtual void StageByReset(CPlayer* player)	// 段階毎のリセット処理
	{
		// 段階取得
		int combostage = player->GetComboStage();

		// コンボの段階加算
		combostage++;
		
		player->SetComboStage(combostage);
	}

	bool m_bAttackReserved;	// 攻撃の予約判定
};

class CPlayerControlAttack_Level1 : public CPlayerControlAttack
{
public:

	CPlayerControlAttack_Level1() : m_bChargePossible(false) {}

	virtual void Attack(CPlayer* player) override;	// 通常攻撃

private:
	bool m_bChargePossible;	// チャージ可能フラグ
};


//=============================
// 防御
//=============================
class CPlayerControlDefence
{
public:
	CPlayerControlDefence() {}

	virtual void Defence(CPlayer* player);	// 防御
};

// 防御レベル1
class CPlayerControlDefence_Level1 : public CPlayerControlDefence
{
public:
	CPlayerControlDefence_Level1() {}

	virtual void Defence(CPlayer* player) override;	// 防御
};


//=============================
// 回避
//=============================
class CPlayerControlAvoid
{
public:
	CPlayerControlAvoid() {}

	virtual void Avoid(CPlayer* player);	// 回避
};


//=============================
// ガード
//=============================
class CPlayerGuard
{
public:
	CPlayerGuard() {}

	virtual void HitProcess(CPlayer* player, MyLib::Vector3 enemypos);	// 受けた時の処理
};

class CPlayerGuard_Level1 : public CPlayerGuard
{
public:
	CPlayerGuard_Level1() {}

	// 受けた時の処理
	virtual void HitProcess(CPlayer* player, MyLib::Vector3 enemypos) override
	{
		CPlayerGuard::HitProcess(player, enemypos);

		MyLib::Vector3 rot = player->GetRotation();
		player->SetMove(MyLib::Vector3(
			sinf(rot.y) * 20.0f,
			0.0f,
			cosf(rot.y) * 20.0f));
	}
};

class CPlayerGuard_Level2 : public CPlayerGuard
{
public:
	CPlayerGuard_Level2() {}

	// 受けた時の処理
	virtual void HitProcess(CPlayer* player, MyLib::Vector3 enemypos) override
	{
		CPlayerGuard::HitProcess(player, enemypos);

		MyLib::Vector3 rot = player->GetRotation();
		player->SetMove(MyLib::Vector3(
			sinf(rot.y) * 5.0f,
			0.0f,
			cosf(rot.y) * 5.0f));
	}
};



#endif