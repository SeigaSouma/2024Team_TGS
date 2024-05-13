//=============================================================================
// 
//  プレイヤー処理 [player.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "player.h"
#include "game.h"
#include "camera.h"
#include "manager.h"
#include "debugproc.h"
#include "renderer.h"
#include "input.h"
#include "enemy.h"
#include "calculation.h"
#include "Xload.h"
#include "model.h"
#include "elevation.h"
#include "shadow.h"
#include "particle.h"
#include "3D_Effect.h"
#include "ballast.h"
#include "impactwave.h"
#include "sound.h"
#include "stage.h"
#include "objectX.h"
#include "instantfade.h"
#include "fade.h"
#include "listmanager.h"
#include "collisionobject.h"
#include "limitarea.h"
#include "MyEffekseer.h"
#include "map.h"
#include "rankingmanager.h"
#include "edit_map.h"
#include "damagepoint.h"

#include "checkpoint.h"

// 使用クラス
#include "playercontrol.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string CHARAFILE = "data\\TEXT\\character\\player\\tyuuni\\setup_player.txt";	// キャラクターファイル
	const float JUMP = 20.0f * 1.5f;			// ジャンプ力初期値
	const float TIME_DMG = static_cast<float>(20) / static_cast<float>(mylib_const::DEFAULT_FPS);	// ダメージ時間
	const int INVINCIBLE_INT = 2;				// 無敵の間隔
	const int INVINCIBLE_TIME = 0;				// 無敵の時間
	const int DEADTIME = 120;					// 死亡時の時間
	const int FADEOUTTIME = 60;					// フェードアウトの時間
	const float MULTIPLIY_DASH = 2.0f;		// ダッシュの倍率
	const float TIME_DASHATTACK = 0.3f;			// ダッシュ攻撃に必要な時間
	const int DEFAULT_STAMINA = 200;			// スタミナのデフォルト値
	const float SUBVALUE_DASH = 0.1f;			// ダッシュの減算量
	const float SUBVALUE_AVOID = 25.0f;			// 回避の減算量

	// ステータス
	const float DEFAULT_RESPAWNHEAL = 0.45f;			// リスポーン時の回復割合
	const float DEFAULT_SUBVALUE_GUARD = 30.0f;			// ガードのスタミナ減算量
	const float DEFAULT_SUBVALUE_COUNTER = 5.0f;		// カウンターのスタミナ減算量
	const float DEFAULT_COUNTERHEAL = 0.0f;				// カウンターのスタミナ回復量
	const float DEFAULT_MULTIPLY_ATTACK = 1.0f;			// 攻撃倍率
	const float DEFAULT_CHARGETIME = 0.9f;				// チャージ時間
	const int DEFAULT_FRAME_EXTENSION_COUNTER = 14;		// カウンター猶予フレーム
	const float  DEFAULT_MULTIPLY_GUARD = 0.4f;			// カードの軽減
	const float DEFAULT_TIME_ADDDOWN = 3.0f;			// ダウン時間付与
	const bool DEFAULT_IS_CHARGEFLINCH = true;			// チャージ時怯みフラグ
	const int DEFAULT_RESPAWN_PERCENT = 20;			// 復活確率
	const float MULTIPLY_CHARGEATK = 2.0f;				// チャージ攻撃の倍率
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayer::STATE_FUNC CPlayer::m_StateFunc[] =
{
	&CPlayer::StateNone,		// なし
	&CPlayer::StateInvincible,	// 無敵
	&CPlayer::StateDamage,		// ダメージ
	&CPlayer::StateKnockBack,	// ノックバック
	&CPlayer::StateDown,		// ダウン
	&CPlayer::StateDead,		// 死亡
	&CPlayer::StateDeadWait,	// 死亡待機
	&CPlayer::StateFadeOut,		// フェードアウト
	&CPlayer::StateRespawn,		// リスポーン
	&CPlayer::StateCounter,		// カウンター中
	&CPlayer::StateAvoid,		// 回避
	&CPlayer::StatePrayer,		// 祈り
	&CPlayer::StateCharge,		// チャージ
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CPlayer> CPlayer::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CPlayer::CPlayer(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	// 共有変数
	m_bJump = false;				// ジャンプ中かどうか
	m_bLandOld = false;				// 過去の着地情報
	m_bHitStage = false;			// ステージの当たり判定
	m_bLandField = false;			// フィールドの着地判定
	m_bHitWall = false;				// 壁の当たり判定
	m_nCntWalk = 0;					// 歩行カウンター
	m_state = STATE_NONE;			// 状態

	SMotionFrag initFrag = {};
	m_sMotionFrag = initFrag;		// モーションのフラグ

	// プライベート変数
	m_Oldstate = STATE_NONE;						// 前回の状態
	m_mMatcol = mylib_const::DEFAULT_COLOR;			// マテリアルの色
	m_posKnokBack = mylib_const::DEFAULT_VECTOR3;	// ノックバックの位置
	m_KnokBackMove = mylib_const::DEFAULT_VECTOR3;	// ノックバックの移動量
	m_nCntState = 0;								// 状態遷移カウンター
	m_nCntPowerEmission = 0;						// パワーアップの発生物カウンター
	m_nComboStage = 0;								// コンボの段階
	m_nIdxRockOn = 0;								// ロックオン対象のインデックス番号
	m_bLockOnAtStart = false;						// カウンター開始時にロックオンしていたか
	m_bReadyDashAtk = false;						// ダッシュアタックのフラグ
	m_bAttacking = false;							// 攻撃中
	m_bCounterAccepting = false;					// カウンター受付中
	m_bDash = false;								// ダッシュ判定
	m_fDashTime = 0.0f;								// ダッシュ時間
	m_fChargeTime = 0.0f;							// チャージ時間
	m_bChargeCompletion = false;					// チャージ完了フラグ
	m_nRespawnPercent = 0;							// リスポーン確率
	m_bTouchBeacon = false;							// ビーコンに触れてる判定
	m_bMotionAutoSet = false;						// モーションの自動設定

	m_PlayerStatus = sPlayerStatus();				// プレイヤーステータス
	m_sDamageInfo = sDamageInfo();					// ダメージ情報

	m_nMyPlayerIdx = 0;								// プレイヤーインデックス番号
	m_pShadow = nullptr;								// 影の情報
	m_pHPGauge = nullptr;							// HPゲージのポインタ

	m_pControlMove = nullptr;						// 移動操作
	m_pControlAtk = nullptr;						// 攻撃操作
	m_pControlDefence = nullptr;					// 防御操作
	m_pControlAvoid = nullptr;						// 回避操作
	m_pGuard = nullptr;								// ガード
}

//==========================================================================
// デストラクタ
//==========================================================================
CPlayer::~CPlayer()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPlayer* CPlayer::Create(int nIdx)
{
	// メモリ確保
	CPlayer* pPlayer = DEBUG_NEW CPlayer;

	if (pPlayer != nullptr)
	{
		// プレイヤーインデックス番号
		pPlayer->m_nMyPlayerIdx = nIdx;

		// 初期化処理
		pPlayer->Init();
	}

	return pPlayer;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPlayer::Init()
{
	// 種類の設定
	SetType(TYPE_PLAYER);

	// ダメージ受け付け判定
	m_sDamageInfo.bReceived = true;
	m_sDamageInfo.reciveTime = 0.0f;

	m_state = STATE_NONE;	// 状態
	m_nCntState = 0;		// 状態遷移カウンター
	m_bLandOld = true;		// 前回の着地状態
	m_bMotionAutoSet = true;						// モーションの自動設定
	m_nRespawnPercent = DEFAULT_RESPAWN_PERCENT;	// リスポーン確率

	// キャラ作成
	HRESULT hr = SetCharacter(CHARAFILE);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// 割り当て
	m_List.Regist(this);

	// 操作関連
	ChangeMoveControl(DEBUG_NEW CPlayerControlMove());
	ChangeAtkControl(DEBUG_NEW CPlayerControlAttack());
	ChangeDefenceControl(DEBUG_NEW CPlayerControlDefence());
	ChangeAvoidControl(DEBUG_NEW CPlayerControlAvoid());
	ChangeGuardGrade(DEBUG_NEW CPlayerGuard());

	//// スキルポイント生成
	//m_pSkillPoint = CSkillPoint::Create();

	//// HPゲージ生成
	//m_pHPGauge = CHP_GaugePlayer::Create(MyLib::Vector3(640.0f, 640.0f, 0.0f), GetLifeOrigin());

	//// スタミナゲージ生成
	//m_pStaminaGauge = CStaminaGauge_Player::Create(MyLib::Vector3(640.0f, 680.0f, 0.0f), DEFAULT_STAMINA);

	return S_OK;
}

//==========================================================================
// 移動の操作変更
//==========================================================================
void CPlayer::ChangeMoveControl(CPlayerControlMove* control)
{
	delete m_pControlMove;
	m_pControlMove = control;
}

//==========================================================================
// 攻撃の操作変更
//==========================================================================
void CPlayer::ChangeAtkControl(CPlayerControlAttack* control)
{ 
	delete m_pControlAtk;
	m_pControlAtk = control;
}

//==========================================================================
// 防御の操作変更
//==========================================================================
void CPlayer::ChangeDefenceControl(CPlayerControlDefence* control)
{ 
	delete m_pControlDefence;
	m_pControlDefence = control;
}

//==========================================================================
// 回避の操作変更
//==========================================================================
void CPlayer::ChangeAvoidControl(CPlayerControlAvoid* control)
{ 
	delete m_pControlAvoid;
	m_pControlAvoid = control;
}

//==========================================================================
// ガード性能変更
//==========================================================================
void CPlayer::ChangeGuardGrade(CPlayerGuard* guard)
{
	delete m_pGuard;
	m_pGuard = guard;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayer::Uninit()
{
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// HPゲージ
	if (m_pHPGauge != nullptr)
	{
		m_pHPGauge = nullptr;
	}

	// 操作系
	if (m_pControlAtk != nullptr)
	{
		delete m_pControlAtk;
		m_pControlAtk = nullptr;
	}
	if (m_pControlDefence != nullptr)
	{
		delete m_pControlDefence;
		m_pControlDefence = nullptr;
	}
	if (m_pControlAvoid != nullptr)
	{
		delete m_pControlAvoid;
		m_pControlAvoid = nullptr;
	}
	if (m_pGuard != nullptr)
	{
		delete m_pGuard;
		m_pGuard = nullptr;
	}

	// 終了処理
	CObjectChara::Uninit();

	// 削除
	m_List.Delete(this);
}

//==========================================================================
// 終了処理
//==========================================================================
void CPlayer::Kill()
{
	//// 遷移なしフェード追加
	//CManager::GetInstance()->GetInstantFade()->SetFade(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 40);

	//// 遷移状態に変更
	//CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SCENE_REASPAWN);

	CManager::GetInstance()->GetFade()->SetFade(CScene::MODE::MODE_RANKING);

	my_particle::Create(GetPosition(), my_particle::TYPE_ENEMY_FADE);

	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// 操作系
	if (m_pControlAtk != nullptr)
	{
		delete m_pControlAtk;
		m_pControlAtk = nullptr;
	}
	if (m_pControlDefence != nullptr)
	{
		delete m_pControlDefence;
		m_pControlDefence = nullptr;
	}
	if (m_pControlAvoid != nullptr)
	{
		delete m_pControlAvoid;
		m_pControlAvoid = nullptr;
	}
	if (m_pGuard != nullptr)
	{
		delete m_pGuard;
		m_pGuard = nullptr;
	}

	// ロックオン設定
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->SetRockOn(0.0f, false);
	}

	// リストループ
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;
	while (enemyList.ListLoop(&pEnemy))
	{
		pEnemy->SetEnableRockOn(false);
	}

}

//==========================================================================
// 更新処理
//==========================================================================
void CPlayer::Update()
{
	if (IsDeath())
	{
		return;
	}

	// キーボード情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	if (pInputKeyboard->GetTrigger(DIK_F5))
	{// F5でリセット
		SetPosition(MyLib::Vector3(0.0f, 0.0f, -1000.0f));
		SetMove(MyLib::Vector3(0.0f, 0.0f, 0.0f));
	}

	// エディット中は抜ける
	if (CGame::GetInstance()->GetElevation()->IsEdit())
	{
		return;
	}

	// エディット中は抜ける
	if (CGame::GetInstance()->GetEditType() != CGame::GetInstance()->EDITTYPE_OFF)
	{
		return;
	}

	// 過去の位置保存
	SetOldPosition(GetPosition());

	// フラグリセット
	ResetFrag();

	// 親の更新処理
	CObjectChara::Update();

	// 操作
	Controll();

	// モーションの設定処理
	MotionSet();

	// 状態更新
	UpdateState();

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// カメラの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera->GetStateCameraR() == CCamera::POSR_STATE_ROCKON)
	{
		pCamera->SetTargetPosition(pos);
		pCamera->SetTargetRotation(rot);
	}
	else if(pCamera->GetStateCameraR() == CCamera::POSR_STATE_NORMAL)
	{
		MyLib::Vector3 camerapos = pos;
		pCamera->SetTargetPosition(camerapos);
		pCamera->SetTargetRotation(rot);
	}

	// 影の位置更新
	if (m_pShadow != nullptr)
	{
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

	// 位置の制限
	LimitPos();

#if 1

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[プレイヤーの操作]------------------\n"
		"位置：【X：%f, Y：%f, Z：%f】 【W / A / S / D】\n"
		"向き：【X：%f, Y：%f, Z：%f】 【Z / C】\n"
		"移動量：【X：%f, Y：%f, Z：%f】\n"
		"モーション：【%d】\n"
		"体力：【%d】\n"
		"状態：【%d】\n"
		"コンボステージ：【%d】\n"
		"ダッシュ時間：【%f】\n"
		, pos.x, pos.y, pos.z, rot.x, rot.y, rot.y, move.x, move.y, move.z, GetMotion()->GetType(), GetLife(), m_state, m_nComboStage, m_fDashTime);

#endif

}

//==========================================================================
// 操作処理
//==========================================================================
void CPlayer::Controll()
{

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

	// ゲームパッド情報取得
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();\

	// 経過時間取得
	float fCurrentTime = CManager::GetInstance()->GetDeltaTime();

	if (CGame::GetInstance()->GetGameManager()->IsControll())
	{// 行動できるとき

		// 操作関数
		if (m_state != STATE_KNOCKBACK &&
			m_state != STATE_DEAD &&
			m_state != STATE_DEADWAIT &&
			m_state != STATE_FADEOUT)
		{
			m_pControlAtk->Attack(this);		// 攻撃操作
			m_pControlDefence->Defence(this);	// 防御操作
			m_pControlAvoid->Avoid(this);		// 回避操作
		}

		// 移動操作
		m_pControlMove->Move(this);

	}



	// 位置取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 newPosition = GetPosition();
	MyLib::Vector3 sakiPos = GetPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 移動量加算
	newPosition.x += move.x;
	newPosition.z += move.z;


	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	sakiPos.x = newPosition.x + sinf(D3DX_PI + rot.y) * GetRadius();
	sakiPos.z = newPosition.z + cosf(D3DX_PI + rot.y) * GetRadius();

	// 現在と目標の差分を求める
	float fRotDiff = GetRotDest() - rot.y;

	// 角度の正規化
	UtilFunc::Transformation::RotNormalize(fRotDiff);

	// 角度の補正をする
	rot.y += fRotDiff * 0.25f;
	UtilFunc::Transformation::RotNormalize(rot.y);

	// 向き設定
	SetRotation(rot);


	// 重力処理
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DEAD && m_state != STATE_FADEOUT && m_state != STATE_DEADWAIT)
	{
		move.y -= mylib_const::GRAVITY;

		// 位置更新
		newPosition.y += move.y;
		sakiPos.y = newPosition.y;
	}

	//**********************************
	// 当たり判定
	//**********************************
	bool bLandStage = Collision(sakiPos, move);

	bool bMove = false;
	if (m_bLandOld == false && bLandStage)
	{// 前回は着地していなくて、今回は着地している場合

		bMove = false;
	}

	if (m_bLandOld && bLandStage)
	{// 前回も今回も着地している場合
		bMove = true;
	}

	if (m_bHitWall == false && 
		(bLandStage == false || bMove || m_bLandField || m_bJump || m_sMotionFrag.bKnockBack || m_sMotionFrag.bDead))
	{
		pos.x = newPosition.x;
		pos.z = newPosition.z;
		pos.y = sakiPos.y;
		Collision(pos, move);

		// 前回は乗ってたことにする
		m_bLandOld = true;
	}
	else
	{
		MyLib::Vector3 posOld = GetOldPosition();
		pos.x = posOld.x;
		pos.z = posOld.z;
		pos = posOld;
		pos.y -= mylib_const::GRAVITY * 7.0f;

		if (m_bJump == false)
		{
			m_bJump = true;
		}

		Collision(pos, move);
	}

	// 慣性補正
	if (m_state == STATE_DMG)
	{
		move.x += (0.0f - move.x) * 0.1f;
		move.z += (0.0f - move.z) * 0.1f;
	}
	else if (m_state != STATE_KNOCKBACK && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{
		move.x += (0.0f - move.x) * 0.25f;
		move.z += (0.0f - move.z) * 0.25f;
	}


	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// ロックオン対象切り替え
	if (CManager::GetInstance()->GetCamera()->IsRockOn())
	{
		SwitchRockOnTarget();
	}

	if (pInputGamepad->GetTrigger(CInputGamepad::BUTTON_RSTICKPUSH, m_nMyPlayerIdx))
	{
		if (CManager::GetInstance()->GetCamera()->IsRockOn())
		{// ロックオン解除

			// ロックオン設定
			CManager::GetInstance()->GetCamera()->SetRockOn(0.0f, false);

			// リストループ
			CListManager<CEnemy> enemyList = CEnemy::GetListObj();
			CEnemy* pEnemy = nullptr;
			while (enemyList.ListLoop(&pEnemy))
			{
				pEnemy->SetEnableRockOn(false);
			}
		}
		else
		{
			// ロックオン処理
			RockOn();
		}
	}

	// リストループ
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;
	while (enemyList.ListLoop(&pEnemy))
	{
		if (CGame::GetInstance()->GetRockOnDistance() <= UtilFunc::Calculation::GetPosLength3D(pos, pEnemy->GetPosition()))
		{
			if (pEnemy->IsRockOnAccept())
			{
				// ロックオン設定
				CManager::GetInstance()->GetCamera()->SetRockOn(0.0f, false);
			}

			pEnemy->SetEnableRockOn(false);
		}
	}

	////if (m_pWeaponHandle != nullptr)
	//{
	//	// 武器の位置
	//	MyLib::Vector3 weponpos = UtilFunc::Transformation::WorldMtxChangeToPosition(GetModel()[16]->GetWorldMtx());

	//	// 武器のマトリックス取得
	//	D3DXMATRIX weaponWorldMatrix = GetModel()[16]->GetWorldMtx();

	//	// 軌跡のマトリックス設定
	//	CMyEffekseer::GetInstance()->SetMatrix(m_WeaponHandle, weaponWorldMatrix);
	//	CMyEffekseer::GetInstance()->SetPosition(m_WeaponHandle, weponpos);
	//}

	// デバッグ用
#if _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_LEFT))
	{
		CCollisionObject::Create(GetPosition(), mylib_const::DEFAULT_VECTOR3, 100000.0f, 3, 10000, CCollisionObject::TAG_PLAYER);
	}

	if (pInputKeyboard->GetRepeat(DIK_RIGHT, 4)){
		//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_NORMALATK_HIT2);
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_COUNTER_TURN, false);

		CPlayer::Hit(10000, CGameManager::AttackType::ATTACK_NORMAL);
	}

	static float fff = 1.0f;
	if (pInputKeyboard->GetTrigger(DIK_UP))
	{
		fff += 0.1f;
		CManager::GetInstance()->GetSound()->SetFrequency(CSound::LABEL_BGM_GAME, fff);

		MyLib::Vector3 weponpos = 0.0f;
		weponpos.y += 150.0f;
		weponpos.z += 800.0f;
		CMyEffekseer::GetInstance()->SetEffect(
			CMyEffekseer::EFKLABEL_COUNTERLINE2,
			weponpos, 0.0f, 0.0f, 60.0f);

	}
	if (pInputKeyboard->GetTrigger(DIK_DOWN))
	{
		fff -= 0.1f;
		CManager::GetInstance()->GetSound()->SetFrequency(CSound::LABEL_BGM_GAME, fff);
	}

	if (pInputKeyboard->GetPress(DIK_J))
	{
		CDamagePoint::Create(GetPosition(), UtilFunc::Transformation::Random(1, 99));
	}
#endif
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::SetMotion(int motionIdx)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx);
}

//==========================================================================
// モーションの設定
//==========================================================================
void CPlayer::MotionSet()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// モーションの自動設定
	if (!m_bMotionAutoSet)
	{
		return;
	}

	if (m_state == STATE_DEAD ||
		m_state == STATE_DEADWAIT ||
		m_state == STATE_DOWN ||
		m_state == STATE_CHARGE ||
		m_state == STATE_FADEOUT ||
		m_state == STATE_KNOCKBACK ||
		m_state == STATE_PRAYER)
	{
		return;
	}

	if (pMotion->IsFinish())
	{// 終了していたら

		// 現在の種類取得
		int nType = pMotion->GetType();
		int nOldType = pMotion->GetOldType();

		if (m_sMotionFrag.bMove && m_sMotionFrag.bKnockBack == false && m_bJump == false &&
			m_sMotionFrag.bATK == false && m_sMotionFrag.bATK == false)
		{// 移動していたら

			m_sMotionFrag.bMove = false;	// 移動判定OFF

			// 移動モーション
			if (m_bDash)
			{
				pMotion->Set(MOTION_DASH);
			}
			else
			{
				pMotion->Set(MOTION_WALK);
			}
		}
		else if (m_sMotionFrag.bJump && m_sMotionFrag.bATK == false && m_sMotionFrag.bKnockBack == false && m_sMotionFrag.bDead == false)
		{// ジャンプ中

			// ジャンプのフラグOFF
			m_sMotionFrag.bJump = false;

			// ジャンプモーション
			pMotion->Set(MOTION_JUMP);
		}
		else if (m_bJump && m_sMotionFrag.bJump == false && m_sMotionFrag.bATK == false && m_sMotionFrag.bKnockBack == false && m_sMotionFrag.bDead == false)
		{// ジャンプ中&&ジャンプモーションが終わってる時

			// 落下モーション
			pMotion->Set(MOTION_FALL);
		}
		else
		{
			// ニュートラルモーション
			pMotion->Set(MOTION_DEF);
		}
	}
}

//==========================================================================
// モーション別の状態設定
//==========================================================================
void CPlayer::MotionBySetState()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int nType = pMotion->GetType();

	switch (nType)
	{
	case MOTION_DASH:

		// ダッシュ時間加算
		m_fDashTime += CManager::GetInstance()->GetDeltaTime();
		break;

	default:
		m_fDashTime = 0.0f;
		break;
	}

	// ダッシュアタックのフラグ
	if (m_fDashTime >= TIME_DASHATTACK)
	{
		m_bReadyDashAtk = true;
	}
	else
	{
		m_bReadyDashAtk = false;
	}

	

	// インプット情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

}

//==========================================================================
// フラグリセット
//==========================================================================
void CPlayer::ResetFrag()
{
	m_bCounterAccepting = false;	// カウンター受付中
}

//==========================================================================
// ロックオン
//==========================================================================
void CPlayer::RockOn()
{
	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 敵のリスト取得
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;

	float fNearLen = CGame::GetInstance()->GetRockOnDistance();
	int nMaxIdx = 0;
	MyLib::Vector3 targetpos(0.0f);

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// リストループ
	int i = 0;
	MyLib::Vector3 enemypos(0.0f);
	while (enemyList.ListLoop(&pEnemy))
	{
		// 敵の位置取得
		enemypos = pEnemy->GetPosition();

		if (pCamera->IsOnScreen(enemypos))
		{
			float len = UtilFunc::Calculation::GetPosLength3D(pos, enemypos);
			if (fNearLen > len)
			{
				fNearLen = len;
				nMaxIdx = i;
			}
		}

		// インデックス加算
		i++;
	}

	if (fNearLen < CGame::GetInstance()->GetRockOnDistance())
	{// ロックオン距離内なら

		// ロックオン設定
		//pCamera->SetRockOn(enemyList.GetData(nMaxIdx)->GetPosition(), true);
		enemyList.GetData(nMaxIdx)->SetEnableRockOn(true);

		// インデックス番号設定
		m_nIdxRockOn = nMaxIdx;
	}
}

//==========================================================================
// ロックオン対象切り替え
//==========================================================================
void CPlayer::SwitchRockOnTarget()
{
	// カメラ取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// ゲームパッド情報取得
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// 敵のリスト取得
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;

	bool bSwitch = true;
	if (pInputGamepad->GetRStickTrigger(CInputGamepad::STICK_X))
	{// 左右どちらかに切り替え

		bool bSwitch = true;

		// リストループ
		int i = 0, nMaxIdx = m_nIdxRockOn;
		float fNearLen = CGame::GetInstance()->GetRockOnDistance();
		pEnemy = nullptr;
		MyLib::Vector3 enemypos(0.0f);
		while (enemyList.ListLoop(&pEnemy))
		{
			// 敵の位置取得
			enemypos = pEnemy->GetPosition();

			if (pCamera->IsOnScreen(enemypos))
			{
				float len = UtilFunc::Calculation::GetPosLength3D(pos, enemypos);
				if (fNearLen > len &&
					m_nIdxRockOn != i)
				{
					fNearLen = len;
					nMaxIdx = i;
				}
			}

			// インデックス加算
			i++;
		}

		if (enemyList.GetData(m_nIdxRockOn) != nullptr)
		{
			// 今までロックオンしてた対象リセット
			enemyList.GetData(m_nIdxRockOn)->SetEnableRockOn(false);
		}

		// ロックオン設定
		CEnemy* pSetEnemy = enemyList.GetData(nMaxIdx);
		//CManager::GetInstance()->GetCamera()->SetRockOn(pSetEnemy->GetPosition(), true);
		pSetEnemy->SetEnableRockOn(true);

		// インデックス番号設定
		m_nIdxRockOn = nMaxIdx;
	}
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CPlayer::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	int nType = pMotion->GetType();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), ATKInfo);
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();

	switch (nType)
	{
	case MOTION::MOTION_WALK:
		/*if (nCntATK == 0)
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_WALK1);
		}
		else{

			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_WALK2);
		}*/
		break;

	case MOTION::MOTION_DASH:
		/*if (nCntATK == 0)
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_DASH1);
		}
		else {

			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_DASH2);
		}*/
		break;

	default:
		break;
	}
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CPlayer::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	// モーション取得
	CMotion* pMotion = GetMotion();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	CEffect3D* pEffect = nullptr;

	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(
		weponpos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		pATKInfo->fRangeSize, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	if (pATKInfo->bEndAtk)
	{
		return;
	}

	// 敵のリスト取得
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;

	// リストループ
	while (enemyList.ListLoop(&pEnemy))
	{
		// コライダーの数繰り返し
		std::vector<SphereCollider> colliders = pEnemy->GetSphereColliders();
		for (const auto& collider : colliders)
		{
			MyLib::HitResult hitresult = UtilFunc::Collision::SphereRange(weponpos, collider.center, pATKInfo->fRangeSize, collider.radius);
			if (hitresult.ishit)
			{// 球の判定

				int damage = static_cast<int>(static_cast<float>(pATKInfo->nDamage) * m_PlayerStatus.attackMultiply);

				

				if (pEnemy->Hit(damage, GetPosition()))
				{// 当たってたら

					pATKInfo->bEndAtk = true;

					// 位置
					MyLib::Vector3 pos = GetPosition();
					MyLib::Vector3 enemypos = pEnemy->GetPosition();

					if (!pEnemy->IsActiveSuperArmor())
					{
						// ターゲットと敵との向き
						float fRot = enemypos.AngleXZ(pos);
						UtilFunc::Transformation::RotNormalize(fRot);

						pEnemy->SetMove(MyLib::Vector3(sinf(fRot) * 8.0f, 0.0f, cosf(fRot) * 8.0f));
					}

					CMyEffekseer::GetInstance()->SetEffect(
						CMyEffekseer::EFKLABEL_HITMARK_RED,
						hitresult.hitpos, 0.0f, 0.0f, 50.0f);

					// ダメージ表記
					enemypos.y += pEnemy->GetHeight() * 0.5f;
					enemypos += UtilFunc::Transformation::GetRandomPositionSphere(enemypos, collider.radius * 0.5f);

					
					break;
				}
			}
		}
	}
}

//==========================================================================
// 位置制限
//==========================================================================
void CPlayer::LimitPos()
{
	return;

	MyLib::Vector3 pos = GetPosition();

	// エリア制限情報取得
	CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
	CLimitArea* pLimitArea = nullptr;

	while (limitareaList.ListLoop(&pLimitArea))
	{
		CLimitArea::sLimitEreaInfo info = pLimitArea->GetLimitEreaInfo();

		// 大人の壁を適用
		if (pos.x + GetRadius() >= info.fMaxX) { pos.x = info.fMaxX - GetRadius(); }
		if (pos.x - GetRadius() <= info.fMinX) { pos.x = info.fMinX + GetRadius(); }
		if (pos.z + GetRadius() >= info.fMaxZ) { pos.z = info.fMaxZ - GetRadius(); }
		if (pos.z - GetRadius() <= info.fMinZ) { pos.z = info.fMinZ + GetRadius(); }
	}

	// 円の押し戻し
	if (pos.LengthXZ() > mylib_const::RADIUS_STAGE)
	{// 補正
		pos = pos.Normal() * mylib_const::RADIUS_STAGE;
	}
	SetPosition(pos);

	CollisionMapObject();
}

//==========================================================================
// チェックポイント復活
//==========================================================================
void CPlayer::ReaspawnCheckPoint()
{
	// チェックポイントのID取得
	int saveID = CCheckpoint::GetSaveID();

	// チェックポイント取得
	CListManager<CCheckpoint> checkpointList = CCheckpoint::GetListObj();
	CCheckpoint* pCheckPoint = checkpointList.GetData(saveID);

	// 位置取得
	MyLib::Vector3 pos = pCheckPoint->GetPosition();
	SetPosition(pos);
}

//==========================================================================
// 当たり判定
//==========================================================================
bool CPlayer::Collision(MyLib::Vector3 &pos, MyLib::Vector3 &move)
{
	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// 着地したかどうか
	bool bLand = false;
	float fHeight = 0.0f;
	m_bLandField = false;
	m_bHitWall = false;			// 壁の当たり判定

	// 高さ取得
	if (m_state != STATE_KNOCKBACK && m_state != STATE_DEAD && m_state != STATE_FADEOUT)
	{
		fHeight = CManager::GetInstance()->GetScene()->GetElevation()->GetHeight(pos, &bLand);
	}
	else
	{
		fHeight = pos.y;
	}

	if (fHeight > pos.y)
	{// 地面の方が自分より高かったら

		// 地面の高さに補正
		pos.y = fHeight;
		m_bLandField = true;

		if (bLand)
		{// 着地してたら

			// ジャンプ使用可能にする
			m_bJump = false;
			move.y = 0.0f;
			m_bLandOld = true;
		}
	}


	// Xファイルとの判定
	CStage *pStage = CGame::GetInstance()->GetStage();
	if (pStage == nullptr)
	{// nullptrだったら
		return false;
	}

	bool bNowLand = false;

	// ステージに当たった判定
	m_bHitStage = false;
	for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
	{
		// オブジェクト取得
		CObjectX *pObjX = pStage->GetObj(nCntStage);

		if (pObjX == nullptr)
		{// nullptrだったら
			continue;
		}

		// 高さ取得
		bool bLand = false;
		fHeight = pObjX->GetHeight(pos, bLand);

		if (bLand && fHeight > pos.y)
		{// 地面の方が自分より高かったら

			// 地面の高さに補正
			if (pos.y + 50.0f <= fHeight)
			{// 自分より壁が高すぎる
				m_bHitWall = true;
			}
			else
			{
				pos.y = fHeight;
			}

			m_bHitStage = true;
			m_bLandField = false;

			if (bLand)
			{// 着地してたら

				if ((m_sMotionFrag.bKnockBack || m_bJump) && GetPosition().y >= fHeight)
				{
					m_bLandOld = true;
				}

				if (m_bJump)
				{// ジャンプ中だったら

					// モーション取得
					CMotion* pMotion = GetMotion();
					pMotion->ToggleFinish(true);
				}

				// ジャンプ使用可能にする
				m_bJump = false;
				move.y = 0.0f;
				bNowLand = true;
				m_sMotionFrag.bJump = false;
			}
		}
	}

	// リストループ
	CListManager<CEnemy> enemyList = CEnemy::GetListObj();
	CEnemy* pEnemy = nullptr;
	MyLib::Vector3 enemypos;
	float radius = GetRadius();
	float enemyradius = 0.0f;

	while (enemyList.ListLoop(&pEnemy))
	{
		enemypos = pEnemy->GetCenterPosition();
		enemyradius = pEnemy->GetRadius();
		enemyradius *= 0.5f;

		if (UtilFunc::Collision::CircleRange3D(pos, enemypos, radius, enemyradius))
		{
			// ターゲットと敵との向き
			float fRot = atan2f((pos.x - enemypos.x), (pos.z - enemypos.z));
			float totalradius = radius + enemyradius;

			pos.x = enemypos.x + sinf(fRot) * totalradius;
			pos.z = enemypos.z + cosf(fRot) * totalradius;
		}
	}

	// 向き設定
	SetRotation(rot);

	return bNowLand;
}


//==========================================================================
// マップオブジェクトとの当たり判定
//==========================================================================
void CPlayer::CollisionMapObject()
{
	// マップオブジェクト取得
	CListManager<CObjectX> mapList = CEdit_Map::GetListObj();
	CObjectX* pObj = nullptr;

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posOld = GetOldPosition();
	float radius = GetRadius();

	/*CEffect3D::Create(
		pos,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		radius, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);*/
	MyLib::Vector3 crossPos = 0.0f;

	pObj = mapList.GetData(0);
	CObjectX* pObj1 = mapList.GetData(1);

	if (pObj == nullptr || pObj1 == nullptr)return;

	if (UtilFunc::Collision::IsAABBCollidingWithBox(pObj->GetAABB(), pObj->GetWorldMtx(), pObj1->GetAABB(), pObj1->GetWorldMtx()))
	{
		CEffect3D::Create(
			pos,
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
			400.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
	}

}

//==========================================================================
// ヒット処理
//==========================================================================
MyLib::HitResult_Character CPlayer::Hit(const int nValue, CGameManager::AttackType atkType)
{
	MyLib::HitResult_Character hitresult = {};

	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	
	
	// 共通のヒット処理
	hitresult = ProcessHit(nValue, 0.0f);

	// 当たった判定を返す
	return hitresult;
}

//==========================================================================
// ヒット処理
//==========================================================================
MyLib::HitResult_Character CPlayer::Hit(const int nValue, CEnemy* pEnemy, CGameManager::AttackType atkType)
{

	MyLib::HitResult_Character hitresult = {};


	// 共通のヒット処理
	hitresult = ProcessHit(nValue, pEnemy->GetPosition());

	return hitresult;
}

//==========================================================================
// 共通のヒット処理
//==========================================================================
MyLib::HitResult_Character CPlayer::ProcessHit(const int nValue, const MyLib::Vector3& hitpos)
{

	MyLib::HitResult_Character hitresult = {};

	// 体力取得
	int nLife = GetLife();

	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (!m_sDamageInfo.bReceived)
	{// ダメージ受付中のみ
		return hitresult;
	}

	if (m_state == STATE_COUNTER ||
		m_state == STATE_AVOID)
	{// ダメージ受けない状態
		return hitresult;
	}

	if (m_state != STATE_DMG &&
		m_state != STATE_KNOCKBACK &&
		m_state != STATE_INVINCIBLE &&
		m_state != STATE_DEAD &&
		m_state != STATE_DEADWAIT &&
		m_state != STATE_RESPAWN &&
		m_state != STATE_FADEOUT)
	{// ダメージ受付状態の時

		// 当たった
		hitresult.ishit = true;
		hitresult.isdamage = true;

		// 体力減らす
		nLife -= nValue;

		// ダッシュ判定OFF
		m_bDash = false;

		// ゲームパッド情報取得
		CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
		pInputGamepad->SetVibration(CInputGamepad::VIBRATION_STATE_DMG, 0);

		// ノックバックする時
		//m_KnokBackMove.y += 18.0f;
		m_bHitStage = false;

		// 体力設定
		SetLife(nLife);

		if (nLife <= 0)
		{// 体力がなくなったら

			// ノックバック判定にする
			m_sMotionFrag.bKnockBack = true;

			// ノックバック状態にする
			m_state = STATE_KNOCKBACK;

			DeadSetting(&hitresult);

			// ダメージ音
			//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_PLAYERDMG_STRONG);
			return hitresult;
		}

		// 過去の状態保存
		m_Oldstate = m_state;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// ノックバックの位置更新
		MyLib::Vector3 pos = GetPosition();
		m_posKnokBack = pos;
		
		float hitAngle = pos.AngleXZ(hitpos);

		// 衝撃波生成
		CImpactWave::Create
		(
			MyLib::Vector3(pos.x, pos.y + 80.0f, pos.z),	// 位置
			MyLib::Vector3(D3DX_PI * 0.5f, D3DX_PI + hitAngle, D3DX_PI),				// 向き
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// 色
			80.0f,										// 幅
			80.0f,										// 高さ
			0.0f,										// 中心からの間隔
			20,											// 寿命
			10.0f,										// 幅の移動量
			CImpactWave::TYPE_GIZAWHITE,				// テクスチャタイプ
			false										// 加算合成するか
		);


		{

			m_sDamageInfo.reciveTime = TIME_DMG;
			m_sDamageInfo.bReceived = false;

			MyLib::Vector3 move;
			move.x = sinf(D3DX_PI + hitAngle) * -60.0f;
			move.z = cosf(D3DX_PI + hitAngle) * -60.0f;
			SetMove(move);

			// やられモーション
			if (!m_sDamageInfo.bActiveSuperArmor)
			{
				// ダメージ状態にする
				m_state = STATE_DMG;

				GetMotion()->Set(MOTION_DMG);

				// ダメージ音
				CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_PLAYERDMG_NORMAL);
				CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_WINGS);
				CManager::GetInstance()->GetSound()->StopSound(CSound::LABEL_SE_BOOST);
			}
		}

		// 振動
		pCamera->SetShake(8, 18.0f, 0.0f);
	}

	return hitresult;
}

//==========================================================================
// ダウン時の設定
//==========================================================================
void CPlayer::DownSetting(const MyLib::Vector3& hitpos)
{
	// ノックバック判定にする
	m_sMotionFrag.bKnockBack = true;

	// ノックバック状態にする
	m_state = STATE_KNOCKBACK;


	m_KnokBackMove.y = 10.0f;


	MyLib::Vector3 pos = GetPosition();

	// 移動量設定
	float hitAngle = pos.AngleXZ(hitpos);
	MyLib::Vector3 move;
	move.x = sinf(D3DX_PI + hitAngle) * -7.0f;
	move.z = cosf(D3DX_PI + hitAngle) * -7.0f;
	SetMove(move);

	// ノックバックモーション
	GetMotion()->Set(MOTION_KNOCKBACK);

	// 振動
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->SetShake(18, 25.0f, 0.0f);
	}
}

//==========================================================================
// 死亡時の設定
//==========================================================================
void CPlayer::DeadSetting(MyLib::HitResult_Character* result)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 死状態
	m_state = STATE_DEAD;

	m_KnokBackMove.y = 8.0f;

	// 遷移カウンター設定
	m_nCntState = DEADTIME;

	// 体力設定
	SetLife(0);

	// ノックバック判定にする
	m_sMotionFrag.bKnockBack = true;

	// やられモーション
	GetMotion()->Set(MOTION_KNOCKBACK);

	// ノックバックの位置更新
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	m_posKnokBack = pos;

	// 衝撃波生成
	CImpactWave::Create
	(
		MyLib::Vector3(pos.x, pos.y + 80.0f, pos.z),	// 位置
		MyLib::Vector3(D3DX_PI * 0.5f, D3DX_PI + rot.y, D3DX_PI),				// 向き
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f),			// 色
		80.0f,										// 幅
		80.0f,										// 高さ
		0.0f,										// 中心からの間隔
		20,											// 寿命
		10.0f,										// 幅の移動量
		CImpactWave::TYPE_GIZAWHITE,				// テクスチャタイプ
		false										// 加算合成するか
	);

	CManager::GetInstance()->SetEnableHitStop(18);

	// 振動
	pCamera->SetShake(21, 30.0f, 0.0f);

	// 死んだ
	result->isdeath = true;
}

//==========================================================================
// 状態更新
//==========================================================================
void CPlayer::UpdateState()
{

	// モーション別の状態設定
	MotionBySetState();

	// ダメージ受付時間更新
	UpdateDamageReciveTimer();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ダメージ受付時間更新
//==========================================================================
void CPlayer::UpdateDamageReciveTimer()
{
	// ダメージ受け付け時間減算
	m_sDamageInfo.reciveTime -= CManager::GetInstance()->GetDeltaTime();
	if (m_sDamageInfo.reciveTime <= 0.0f)
	{
		// スーパーアーマーがない時はダメージモーション終了
		if (!m_sDamageInfo.bActiveSuperArmor &&
			!m_sDamageInfo.bReceived)
		{
			// なにもない状態にする
			m_state = STATE_NONE;

			// モーション取得
			CMotion* pMotion = GetMotion();
			if (pMotion == nullptr)
			{
				return;
			}
			pMotion->ToggleFinish(true);
		}

		if (!m_sDamageInfo.bReceived)
		{
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_WINGS);
		}

		// ダメージ受け付け判定
		m_sDamageInfo.bReceived = true;
		m_sDamageInfo.reciveTime = 0.0f;
	}
}

//==========================================================================
// なし
//==========================================================================
void CPlayer::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//==========================================================================
// 無敵状態
//==========================================================================
void CPlayer::StateInvincible()
{
	// 状態遷移カウンター減算
	m_nCntState--;

	if (m_nCntState % INVINCIBLE_INT == 0)
	{// 規定間隔
		
		// 色設定
		if (m_mMatcol.a == 1.0f)
		{
			m_mMatcol.a = 0.4f;
		}
		else
		{
			m_mMatcol.a = 1.0f;
		}
	}

	if (m_nCntState <= 0)
	{// 遷移カウンターが0になったら

		// なにもない状態にする
		m_state = STATE_NONE;
		m_nCntState = 0;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

}

//==========================================================================
// ダメージ状態
//==========================================================================
void CPlayer::StateDamage()
{
	//// 状態遷移カウンター減算
	//m_nCntState++;

	//// 色設定
	//m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//if (m_nCntState >= TIME_DMG)
	//{
	//	m_state = STATE_INVINCIBLE;
	//	m_nCntState = INVINCIBLE_TIME;

	//	// モーション取得
	//	CMotion* pMotion = GetMotion();
	//	if (pMotion == nullptr)
	//	{
	//		return;
	//	}
	//	pMotion->ToggleFinish(true);
	//}
}

//==========================================================================
// ノックバック
//==========================================================================
void CPlayer::StateKnockBack()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// 目標の向き取得
	float fRotDest = GetRotDest();

	// 移動量取得
	float fMove = GetVelocity();

	// 現在と目標の差分
	float fRotDiff = 0.0f;

	// 距離の判定
	bool bLen = false;

	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	int nType = pMotion->GetType();

	if (nType == MOTION_KNOCKBACK)
	{
		// 状態遷移カウンター減算
		m_nCntState++;

		// 位置更新
		pos.y = (-0.3f * (float)(m_nCntState * m_nCntState) + m_KnokBackMove.y * (float)m_nCntState) + m_posKnokBack.y;
		pos.x += move.x;
		pos.z += move.z;
	}

	// 起伏との判定
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage))
	{// 地面と当たっていたら

		/*m_state = STATE_INVINCIBLE;
		m_nCntState = INVINCIBLE_TIME;*/
		m_KnokBackMove.y = 0.0f;	// 移動量ゼロ
		m_bLandOld = true;
		pos.y = 0.0f;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ノックバック判定消す
		m_sMotionFrag.bKnockBack = false;

		pMotion->Set(MOTION_KNOCKBACK_PASSIVE);

		// Xファイルとの判定
		CStage* pStage = CGame::GetInstance()->GetStage();
		if (pStage == nullptr)
		{// nullptrだったら
			return;
		}

		// ステージに当たった判定
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// オブジェクト取得
			CObjectX* pObjX = pStage->GetObj(nCntStage);

			if (pObjX == nullptr)
			{// nullptrだったら
				continue;
			}

			// 高さ取得
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);

	// 向き設定
	SetRotation(rot);

	// 目標の向き設定
	SetRotDest(fRotDest);


	if (nType == MOTION_KNOCKBACK_PASSIVE &&
		pMotion->IsFinish())
	{// 受け身もノックバックも終了

		m_state = STATE_DOWN;
		return;
	}
}

//==========================================================================
// ダウン
//==========================================================================
void CPlayer::StateDown()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(MOTION_DOWN);

}

//==========================================================================
// 死亡状態
//==========================================================================
void CPlayer::StateDead()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 位置更新
	if (m_nCntState > 0)
	{
		int nCnt = DEADTIME - m_nCntState;
		pos.y = (-0.1f * (float)(nCnt * nCnt) + m_KnokBackMove.y * (float)nCnt) + m_posKnokBack.y;
		pos.x += move.x;
	}

	// 状態遷移カウンター減算
	m_nCntState--;

	// 起伏との判定
	if ((CManager::GetInstance()->GetScene()->GetElevation()->IsHit(pos) || m_bHitStage) && m_nCntState >= 10)
	{// 地面と当たっていたら

		m_state = STATE_DEADWAIT;	// 死亡待機状態
		m_state = STATE::STATE_FADEOUT;	// 死亡待機状態
		m_nCntState = FADEOUTTIME;
		m_KnokBackMove.y = 0.0f;	// 移動量ゼロ
		m_bLandOld = true;
		move.x = 0.0f;

		// 色設定
		m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ノックバック判定消す
		m_sMotionFrag.bKnockBack = false;
		m_sMotionFrag.bDead = true;
		//pMotion->ToggleFinish(true);

		// ぶっ倒れモーション
		//pMotion->Set(MOTION_DEAD);

		// Xファイルとの判定
		CStage *pStage = CGame::GetInstance()->GetStage();
		if (pStage == nullptr)
		{// nullptrだったら
			return;
		}

		// ステージに当たった判定
		for (int nCntStage = 0; nCntStage < pStage->GetNumAll(); nCntStage++)
		{
			// オブジェクト取得
			CObjectX *pObjX = pStage->GetObj(nCntStage);

			if (pObjX == nullptr)
			{// nullptrだったら
				continue;
			}

			// 高さ取得
			bool bLand = false;
			pos.y = pObjX->GetHeight(pos, bLand);
		}
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// 死亡待機
//==========================================================================
void CPlayer::StateDeadWait()
{
	// ぶっ倒れモーション
	GetMotion()->Set(MOTION_DEADWAIT);
}

//==========================================================================
// フェードアウト状態
//==========================================================================
void CPlayer::StateFadeOut()
{
	// 状態遷移カウンター減算
	m_nCntState--;

	// 色設定
	m_mMatcol.a = (float)m_nCntState / (float)FADEOUTTIME;

	// ぶっ倒れモーション
	GetMotion()->Set(MOTION_DEAD);

	if (m_nCntState <= 0)
	{// 遷移カウンターが0になったら

		// 死亡処理
		Kill();

		// 終了処理
		Uninit();
		return;
	}
}

//==========================================================================
// リスポーン
//==========================================================================
void CPlayer::StateRespawn()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	
}

//==========================================================================
// カウンター状態
//==========================================================================
void CPlayer::StateCounter()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	CEnemy* pEnemy = CEnemy::GetListObj().GetData(m_nIdxRockOn);
	if (pEnemy == nullptr)
	{
		return;
	}

	MyLib::Vector3 enemypos = pEnemy->GetPosition();
	SetRotDest(pos.AngleXZ(enemypos));

	
}

//==========================================================================
// 回避
//==========================================================================
void CPlayer::StateAvoid()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	
}

//==========================================================================
// 祈り
//==========================================================================
void CPlayer::StatePrayer()
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	
}

//==========================================================================
// チャージ
//==========================================================================
void CPlayer::StateCharge()
{
	

	
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayer::Draw()
{

	// 描画処理
	if (m_state == STATE_DMG)
	{
		CObjectChara::Draw(m_mMatcol);
	}
	else if (m_state == STATE_INVINCIBLE || m_state == STATE_FADEOUT)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else
	{
		CObjectChara::Draw();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CPlayer::SetState(STATE state, int nCntState)
{
	m_state = state;
	m_nCntState = nCntState;
}

//==========================================================================
// 状態取得
//==========================================================================
CPlayer::STATE CPlayer::GetState()
{
	return m_state;
}
