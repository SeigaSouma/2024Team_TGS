//=============================================================================
// 
//  人ヘッダー [people.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _ENEMY_H_
#define _ENEMY_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"

// 前方宣言
class CShadow;

//==========================================================================
// クラス定義
//==========================================================================
// 人クラス
class CPeople : public CObjectChara
{
public:
	
	// 敵種類
	enum TYPE
	{
		TYPE_NORMAL = 0,	// 通常
		TYPE_HORSE,			// ウマ
		TYPE_MAX
	};

	// 状態列挙
	enum STATE
	{
		STATE_NONE = 0,		// なにもない
		STATE_FADEOUT,		// フェードアウト
		STATE_MAX
	};

	enum MOTION
	{
		MOTION_DEF = 0,			// ニュートラル
		MOTION_WALK,			// 移動
		MOTION_ATTACK_NORMAL,	// 通常攻撃
		MOTION_ATTACK_STRONG,	// 強攻撃
		MOTION_DMG,				// ダメージ
		MOTION_DOWN,			// ダウン
		MOTION_RETURNDOWN,		// ダウン
		MOTION_KNOCKBACK,		// やられ
		MOTION_FADEOUT,			// フェードアウト
		MOTION_MAX
	};


	CPeople(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CPeople();


	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// 削除

	void SetState(STATE state);		// 状態設定
	STATE GetState() { return m_state; }
	void SetStateTime(float time) { m_fStateTime = time; }	// 状態時間設定

	// モーション
	void SetMotion(int motionIdx);	// モーションの設定

	HRESULT RoadText(const char *pFileName);
	CPeople *GetPeople();
	TYPE GetType() { return m_type; }	// 種類取得


	static CListManager<CPeople> GetListObj() { return m_List; }	// リスト取得
	static CPeople* Create(const char* pFileName, MyLib::Vector3 pos, TYPE type = TYPE_NORMAL);

protected:

	//=============================
	// 構造体定義
	//=============================
	// モーションの判定
	struct SMotionFrag
	{
		bool bJump;			// ジャンプ中かどうか
		bool bATK;			// 攻撃中かどうか
		bool bKnockback;	// ノックバック中かどうか
		bool bMove;			// 移動中かどうか
		bool bCharge;		// チャージ中かどうか
		SMotionFrag() : bJump(false), bATK(false), bKnockback(false), bMove(false), bCharge(false) {}
	};

	//=============================
	// メンバ関数
	//=============================
	// 状態更新系
	virtual void StateNone();		// 何もない状態
	virtual void StateFadeOut();	// フェードアウト

	// その他関数
	virtual void ProcessLanding();	// 着地時処理
	virtual void AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK) override;		// 攻撃時処理
	virtual void AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK) override;	// 攻撃判定中処理

	//=============================
	// メンバ変数
	//=============================
	STATE m_state;							// 状態
	STATE m_Oldstate;						// 前回の状態
	float m_fStateTime;						// 状態カウンター
	SMotionFrag m_sMotionFrag;				// モーションのフラグ
	D3DXCOLOR m_mMatcol;					// マテリアルの色
	MyLib::Vector3 m_TargetPosition;		// 目標の位置


private:
	
	//=============================
	// 関数リスト
	//=============================
	typedef void(CPeople::* STATE_FUNC)();
	static STATE_FUNC m_StateFunc[];	// 状態関数リスト

	void UpdateState();					// 状態更新処理
	void Collision();					// 当たり判定
	virtual void MotionSet() = 0;		// モーションの設定
	void LimitArea(); // 大人の壁判定

	//=============================
	// メンバ変数
	//=============================
	TYPE m_type;			// 種類
	CShadow *m_pShadow;			// 影の情報
	static CListManager<CPeople> m_List;	// リスト
};



#endif