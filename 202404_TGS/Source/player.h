//=============================================================================
// 
//  プレイヤーヘッダー [player.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_	// 二重インクルード防止

#include "gamemanager.h"
#include "objectChara.h"
#include "listmanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CShadow;
class CHP_GaugePlayer;
class CEnemy;
class CBaggage;

class CPlayerControlMove;		// 移動
class CPlayerControlBaggage;	// 荷物操作
class CPlayerControlAttack;	// 攻撃
class CPlayerControlDefence;// 防御
class CPlayerControlAvoid;	// 回避
class CPlayerGuard;			// ガード

//==========================================================================
// クラス定義
//==========================================================================
// プレイヤークラス定義
class CPlayer : public CObjectChara
{
public:
	//=============================
	// 列挙型定義
	//=============================
	// モーション列挙
	enum MOTION
	{
		MOTION_DEF = 0,				// ニュートラルモーション
		MOTION_WALK,				// 移動
		MOTION_DASH,				// ダッシュ
		MOTION_AVOID,				// 回避
		MOTION_ATK,					// 攻撃
		MOTION_ATK2,				// 攻撃(派生1)
		MOTION_ATK3,				// 攻撃(派生2)
		MOTION_ATK4,				// 攻撃(派生3)
		MOTION_JUMP,				// ジャンプ
		MOTION_FALL,				// 落下中
		MOTION_DMG,					// ダメージ
		MOTION_KNOCKBACK,			// ノックバック
		MOTION_KNOCKBACK_PASSIVE,	// ノックバック受け身
		MOTION_DOWN,				// ダウン
		MOTION_DEADWAIT,			// 死亡待機
		MOTION_DEAD,				// 死亡
		MOTION_RESPAWN,				// 復活
		MOTION_PRAYER,				// 祈り
		MOTION_PRAYERLOOP,			// 祈りループ
		MOTION_GUARD,				// ガード
		MOTION_GUARD_DMG,			// ガードダメージ
		MOTION_COUNTER_ACCEPT,		// 反撃受け付け
		MOTION_COUNTER_TURN,		// 反撃受け流し
		MOTION_COUNTER_ATTACK,		// 反撃
		MOTION_DASHATK,				// ダッシュ攻撃
		MOTION_ATK4_FINISH,			// 4段階目攻撃のチャージ
		MOTION_MAX
	};

	// 状態定義
	enum STATE
	{
		STATE_NONE = 0,		// なにもない
		STATE_INVINCIBLE,	// 無敵
		STATE_DMG,			// ダメージ
		STATE_KNOCKBACK,	// ノックバック
		STATE_DOWN,			// ダウン
		STATE_DEAD,			// 死
		STATE_DEADWAIT,		// 死亡待機
		STATE_FADEOUT,		// フェードアウト
		STATE_RESPAWN,		// 復活
		STATE_COUNTER,		// 反撃
		STATE_AVOID,		// 回避
		STATE_PRAYER,		// 祈り
		STATE_CHARGE,		// チャージ
		STATE_MAX
	};

	//=============================
	// 構造体
	//=============================
	// プレイヤーステータス
	struct sPlayerStatus
	{
		float respawnHeal;				// リスポーン時の回復量
		float guardStaminaSubValue;		// ガード時のスタミナ減少量
		float counterStaminaSubValue;	// カウンター時のスタミナ減少量
		float counterStaminaHealValue;	// カウンター時のスタミナ回復量
		float attackMultiply;			// 攻撃の倍率
		float chargeTime;				// チャージ時間
		float counterExtensionFrame;	// カウンター猶予フレーム
		float damageMitigation;			// ダメージ軽減率
		float addDownTime;				// ダウン時間付与
		bool bChargeFlinch;				// 怯みフラグ

		sPlayerStatus() : respawnHeal(0.0f), guardStaminaSubValue(0.0f), counterStaminaSubValue(0.0f),
			counterStaminaHealValue(0.0f), attackMultiply(0.0f), chargeTime(0.0f),
			counterExtensionFrame(0.0f), damageMitigation(0.0f), addDownTime(0.0f), bChargeFlinch(false) {}

		sPlayerStatus(
			float resheal, float guardvalue, float countervalue, 
			float counterheal, float attackmultiply, float chargetime, 
			float countertime, float damagemitigation, float downtime, bool bFlinsh) : 
			respawnHeal(resheal), guardStaminaSubValue(guardvalue), counterStaminaSubValue(countervalue),
			counterStaminaHealValue(counterheal), attackMultiply(attackmultiply), chargeTime(chargetime),
			counterExtensionFrame(countertime), damageMitigation(damagemitigation), addDownTime(downtime), bChargeFlinch(bFlinsh) {}
	};

	// ダメージ情報
	struct sDamageInfo
	{
		bool bActiveSuperArmor;	// スーパーアーマー
		bool bReceived;			// ダメージ受け付け判定
		float reciveTime;		// ダメージ受付時間

		sDamageInfo() : bActiveSuperArmor(false), bReceived(false), reciveTime(0.0f) {}
	};

	// モーションの判定
	struct SMotionFrag
	{
		bool bJump;			// ジャンプ中
		bool bATK;			// 攻撃中
		bool bGuard;		// ガード
		bool bCounter;		// カウンター中
		bool bKnockBack;	// ノックバック中
		bool bDead;			// 死亡中
		bool bMove;			// 移動中
	};

	CPlayer(int nPriority = 2);
	~CPlayer();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	MyLib::HitResult_Character Hit(const int nValue, CGameManager::AttackType atkType = CGameManager::ATTACK_NORMAL);	// ヒット処理
	MyLib::HitResult_Character Hit(const int nValue, CEnemy* pEnemy, CGameManager::AttackType atkType = CGameManager::ATTACK_NORMAL);	// ヒット処理
	MyLib::HitResult_Character ProcessHit(const int nValue, const MyLib::Vector3& hitpos);

	STATE GetState();		// 状態取得
	void SetState(STATE state, int nCntState = 0);	// 状態設定
	virtual void Kill();			// 死亡処理
	void SwitchRockOnTarget();		// ロック対象切り替え

	// モーション
	void SetMotion(int motionIdx);									// モーションの設定
	void SetEnableDash(bool bDash)	{ m_bDash = bDash; }			// ダッシュ状況設定
	bool IsDash()					{ return m_bDash; }				// ダッシュ判定
	void SetEnableJump(bool bJump)	{ m_bJump = bJump; }			// ジャンプ状況設定
	bool IsJump()					{ return m_bJump; }				// ジャンプ判定
	bool IsAttacking()				{ return m_bAttacking; }		// 攻撃中フラグ取得
	float GetDashTime()				{ return m_fDashTime; }			// ダッシュ時間
	void SetComboStage(int stage)	{ m_nComboStage = stage; }		// コンボの段階設定
	int GetComboStage()				{ return m_nComboStage; }		// コンボの段階取得
	bool IsReadyDashAtk()			{ return m_bReadyDashAtk; }		// ダッシュアタックの準備取得
	void SetMotionFrag(SMotionFrag frag) { m_sMotionFrag = frag; }	// モーションのフラグ設定
	SMotionFrag GetMotionFrag() { return m_sMotionFrag; }			// モーションのフラグ取得
	void SetDamageInfo(sDamageInfo info) { m_sDamageInfo = info; }	// ダメージ情報設定
	sDamageInfo GetDamageInfo() { return m_sDamageInfo; }			// ダメージ情報取得

	//=============================
	// 操作
	//=============================
	void ChangeMoveControl(CPlayerControlMove* control);		// 移動の操作変更
	void ChangeBaggageControl(CPlayerControlBaggage* control);	// 荷物の操作変更
	void ChangeAtkControl(CPlayerControlAttack* control);		// 攻撃の操作変更
	void ChangeDefenceControl(CPlayerControlDefence* control);	// 防御の操作変更
	void ChangeAvoidControl(CPlayerControlAvoid* control);		// 回避の操作変更
	void ChangeGuardGrade(CPlayerGuard* guard);					// ガード性能変更

	//=============================
	// その他
	//=============================
	void SetMyPlayerIdx(int idx) { m_nMyPlayerIdx = idx; }	// 自分のインデックス設定
	int GetMyPlayerIdx() { return m_nMyPlayerIdx; }			// 自分のインデックス取得

	// ダウン
	void DownSetting(const MyLib::Vector3& hitpos);

	// ヒット系
	void DeadSetting(MyLib::HitResult_Character* result);

	static CPlayer* Create(int nIdx);	// 生成
	static CListManager<CPlayer> GetListObj() { return m_List; }	// リスト取得

protected:

	bool Collision(MyLib::Vector3 &pos, MyLib::Vector3 &move);	// 当たり判定
	void MotionSet();	// モーションの設定

	bool m_bJump;				// ジャンプ中かどうか
	bool m_bLandOld;			// 過去の着地情報
	bool m_bHitStage;			// ステージの当たり判定
	bool m_bLandField;			// フィールドの着地判定
	bool m_bHitWall;			// 壁の当たり判定
	int m_nMyPlayerIdx;			// プレイヤーインデックス番号
	int m_nCntWalk;				// 歩行カウンター
	STATE m_state;				// 状態
	CShadow *m_pShadow;			// 影の情報
	SMotionFrag m_sMotionFrag;	// モーションのフラグ
private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CPlayer::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void UpdateState();		// 状態更新
	void StateNone();		// なし
	void StateInvincible();	// 無敵
	void StateDamage();		// ダメージ
	void StateDown();		// ダウン
	void StateKnockBack();	// ノックバック
	void StateDead();		// 死亡
	void StateDeadWait();	// 死亡待機
	void StateFadeOut();	// フェードアウト
	void StateRespawn();	// リスポーン
	void StateCounter();	// カウンター中
	void StateAvoid();		// 回避
	void StatePrayer();		// 祈り
	void StateCharge();		// チャージ

	// その他関数
	virtual void Controll();		// 操作
	void LimitPos();				// 位置制限
	void ReaspawnCheckPoint();		// チェックポイント復活
	void CollisionMapObject();		// マップオブジェクトとの当たり判定
	void MotionBySetState();		// モーション別の状態設定
	void ResetFrag();				// フラグリセット
	void RockOn();					// ロックオン
	void UpdateDamageReciveTimer();	// ダメージ受付時間更新
	void RetryCheck();				// リトライするか確認

	// モーション系関数
	void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//=============================
	// メンバ変数
	//=============================
	STATE m_Oldstate;				// 前回の状態
	D3DXCOLOR m_mMatcol;			// マテリアルの色
	MyLib::Vector3 m_posKnokBack;	// ノックバックの位置
	MyLib::Vector3 m_KnokBackMove;	// ノックバックの移動量
	int m_nCntState;				// 状態遷移カウンター
	int m_nCntPowerEmission;		// パワーアップの発生物カウンター
	int m_nComboStage;				// コンボの段階
	int m_nIdxRockOn;				// ロックオン対象のインデックス番号
	bool m_bLockOnAtStart;			// カウンター開始時にロックオンしていたか
	bool m_bReadyDashAtk;			// ダッシュアタックのフラグ
	bool m_bAttacking;				// 攻撃中
	bool m_bCounterAccepting;		// カウンター受付中
	bool m_bDash;					// ダッシュ判定
	float m_fDashTime;				// ダッシュ時間
	float m_fChargeTime;			// チャージ時間
	bool m_bChargeCompletion;		// チャージ完了フラグ
	int m_nRespawnPercent;			// リスポーン確率
	bool m_bTouchBeacon;			// ビーコンに触れてる判定
	bool m_bMotionAutoSet;			// モーションの自動設定
	Effekseer::Handle m_WeaponHandle;

	sPlayerStatus m_PlayerStatus;	// プレイヤーステータス
	sDamageInfo m_sDamageInfo;		// ダメージ情報
	CHP_GaugePlayer* m_pHPGauge;	// HPゲージのポインタ
	CBaggage* m_pBaggage;			// 荷物のポインタ
	
	// パターン用インスタンス
	CPlayerControlMove* m_pControlMove;			// 移動操作
	CPlayerControlBaggage* m_pControlBaggage;	// 荷物操作
	CPlayerControlAttack* m_pControlAtk;		// 攻撃操作
	CPlayerControlDefence* m_pControlDefence;	// 防御操作
	CPlayerControlAvoid* m_pControlAvoid;		// 回避操作
	CPlayerGuard* m_pGuard;						// ガード操作

	//リトライカウンタ
	int m_nCntRetry;

	static CListManager<CPlayer> m_List;	// リスト
};


#endif