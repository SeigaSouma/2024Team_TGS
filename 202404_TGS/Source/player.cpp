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
#include "retryui.h"

#include "checkpoint.h"
#include "baggage.h"

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
	const int DEFAULT_RESPAWN_PERCENT = 20;				// 復活確率
	const float MULTIPLY_CHARGEATK = 2.0f;				// チャージ攻撃の倍率
	const float MAX_HEIGHT = 200.0f;					// 最大高さ
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CPlayer::STATE_FUNC CPlayer::m_StateFunc[] =
{
	&CPlayer::StateNone,		// なし
	&CPlayer::StateInvincible,	// 無敵
	&CPlayer::StateDamage,		// ダメージ
	&CPlayer::StateDead,		// 死亡
	&CPlayer::StateDeadWait,	// 死亡待機
	&CPlayer::StateReturn,		// チェックポイントに戻る
	&CPlayer::StateRestart,		// スタートに戻る
	&CPlayer::StateRespawn,		// リスポーン
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
	m_bDash = false;								// ダッシュ判定
	m_fDashTime = 0.0f;								// ダッシュ時間
	m_bMotionAutoSet = false;						// モーションの自動設定

	m_PlayerStatus = sPlayerStatus();				// プレイヤーステータス
	m_sDamageInfo = sDamageInfo();					// ダメージ情報

	m_nMyPlayerIdx = 0;								// プレイヤーインデックス番号
	m_pShadow = nullptr;							// 影の情報
	m_pHPGauge = nullptr;							// HPゲージのポインタ
	m_pBaggage = nullptr;							// 荷物のポインタ
	m_pRetryUI = nullptr;							// リトライUIのポインタ

	m_pControlMove = nullptr;						// 移動操作
	m_pControlBaggage = nullptr;					// 荷物操作
	m_pControlSurfacing = nullptr;					// 浮上操作
	m_pControlTrick = nullptr;
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
	ChangeBaggageControl(DEBUG_NEW CPlayerControlBaggage);
	ChangeSurfacingControl(DEBUG_NEW CPlayerControlSurfacing);
	ChangeTrickControl(DEBUG_NEW CPlayerControlTrick);

	// 荷物生成
	m_pBaggage = CBaggage::Create(CBaggage::TYPE::TYPE_CLOTH);

	// 武器の位置
	CMotion* pMotion = GetMotion();
	m_posCylinder = GetMotion()->GetAttackPosition(GetModel(), *pMotion->GetInfo(MOTION::MOTION_WALK).AttackInfo[0]);

	MyLib::Vector3 pos = GetPosition();
	m_pBaggage->SetPosition(m_posCylinder);
	m_pBaggage->SetOriginPosition(m_pBaggage->GetPosition());

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
// 荷物の操作変更
//==========================================================================
void CPlayer::ChangeBaggageControl(CPlayerControlBaggage* control)
{
	delete m_pControlBaggage;
	m_pControlBaggage = control;
}

//==========================================================================
// 浮上の操作変更
//==========================================================================
void CPlayer::ChangeSurfacingControl(CPlayerControlSurfacing* control)
{
	delete m_pControlSurfacing;
	m_pControlSurfacing = control;
}

//==========================================================================
// 浮上の操作変更
//==========================================================================
void CPlayer::ChangeTrickControl(CPlayerControlTrick* control)
{
	delete m_pControlTrick;
	m_pControlTrick = control;
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
	DeleteControl();

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
	my_particle::Create(GetPosition(), my_particle::TYPE_ENEMY_FADE);

	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}

	// 荷物削除
	if (m_pBaggage != nullptr)
	{
		m_pBaggage->Kill();
		m_pBaggage = nullptr;
	}

	// 操作系
	DeleteControl();

	// ロックオン設定
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	if (pCamera != nullptr)
	{
		pCamera->SetRockOn(0.0f, false);
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
		camerapos.y = (m_pBaggage->GetPosition().y + pos.y) * 0.5f;
		camerapos.y = UtilFunc::Transformation::Clamp(camerapos.y, 200.0f, 600.0f);

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


	// 武器の位置
	CMotion* pMotion = GetMotion();
	m_posCylinder = pMotion->GetAttackPosition(GetModel(), *pMotion->GetInfo(MOTION::MOTION_WALK).AttackInfo[0]);

#if 1

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	if (pMotion == nullptr)return;

	// デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print(
		"------------------[プレイヤーの操作]------------------\n"
		"位置：【X：%f, Y：%f, Z：%f】 【W / A / S / D】\n"
		"向き：【X：%f, Y：%f, Z：%f】 【Z / C】\n"
		"移動量：【X：%f, Y：%f, Z：%f】\n"
		"モーション：【%d】\n"
		"体力：【%d】\n"
		"状態：【%d】\n"
		"ダッシュ時間：【%f】\n"
		, pos.x, pos.y, pos.z, rot.x, rot.y, rot.y, move.x, move.y, move.z, pMotion->GetType(), GetLife(), m_state, m_fDashTime);

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
	CInputGamepad *pInputGamepad = CInputGamepad::GetInstance();

	// 経過時間取得
	float fCurrentTime = CManager::GetInstance()->GetDeltaTime();

	if (CGame::GetInstance()->GetGameManager()->IsControll())
	{// 行動できるとき

		// 操作関数
		if (m_state != STATE_DEAD &&
			m_state != STATE_DEADWAIT &&
			m_state != STATE_RETURN &&
			m_state != STATE_RESTART &&
			m_state != STATE::STATE_RESPAWN)
		{
			// 移動操作
			m_pControlMove->Move(this);

			{ // 浮上操作
				float fHeight = m_pControlSurfacing->Surfacing(this);

				MyLib::Vector3 pos = m_pBaggage->GetPosition();
				m_pBaggage->SetOriginPosition(MyLib::Vector3(0.0f, m_posCylinder.y + fHeight, 0.0f));
			}
			m_pControlBaggage->Action(this, m_pBaggage);		// 荷物操作

			{// トリック操作
				int idx = -1; bool value = false;
				m_pControlTrick->Trick(this, idx, value);

				if (value) SetMotion(MOTION_ATK3 + idx);
			}
		}

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
	if (m_state != STATE_DEAD && m_state != STATE_RETURN && m_state != STATE_RESTART && m_state != STATE_DEADWAIT)
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
	else if (m_state != STATE_DEAD && m_state != STATE_RETURN && m_state != STATE_RESTART)
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
	

	// デバッグ用
#if _DEBUG

	if (pInputKeyboard->GetTrigger(DIK_LEFT))
	{
		CCollisionObject::Create(GetPosition(), mylib_const::DEFAULT_VECTOR3, 100000.0f, 3, 10000, CCollisionObject::TAG_PLAYER);
	}

	if (pInputKeyboard->GetRepeat(DIK_RIGHT, 4)){
		//CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_NORMALATK_HIT2);
		CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL::LABEL_SE_COUNTER_TURN, false);

		CPlayer::Hit(10000);
	}

	static float fff = 1.0f;
	if (pInputKeyboard->GetTrigger(DIK_UP))
	{
		fff += 0.1f;
		CManager::GetInstance()->GetSound()->SetFrequency(CSound::LABEL_BGM_GAME, fff);


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
// 操作関連削除
//==========================================================================
void CPlayer::DeleteControl()
{
	if (m_pControlMove != nullptr) {
		delete m_pControlMove;
		m_pControlMove = nullptr;
	}

	if (m_pControlBaggage != nullptr) {
		delete m_pControlBaggage;
		m_pControlBaggage = nullptr;
	}

	if (m_pControlSurfacing != nullptr) {
		delete m_pControlSurfacing;
		m_pControlSurfacing = nullptr;
	}

	if (m_pControlTrick != nullptr) {
		m_pControlTrick->Uninit();
		delete m_pControlTrick;
		m_pControlTrick = nullptr;
	}
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
		m_state == STATE_RETURN ||
		m_state == STATE_RESTART)
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
	

	// インプット情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

}

//==========================================================================
// フラグリセット
//==========================================================================
void CPlayer::ResetFrag()
{

}

//==========================================================================
// ロックオン対象切り替え
//==========================================================================
void CPlayer::SwitchRockOnTarget()
{
	
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

	// カメラ瞬間移動
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->WarpCamera(pos + MyLib::Vector3(0.0f, 150.0f, 0.0f));

	//タイマーストップ
	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_WAIT_AIRPUSH);

	// リスポーン設定
	ReaspawnSetting();
}

//==========================================================================
// スタート復活（いったん決め打ちなのでいじれるようにして）
//==========================================================================
void CPlayer::RespawnStart()
{
	MyLib::Vector3 pos = MyLib::Vector3(0.0f, 10.0f, 0.0f);

	// カメラ瞬間移動
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	pCamera->WarpCamera(pos + MyLib::Vector3(0.0f, 150.0f, 0.0f));

	// 位置設定
	SetPosition(pos);

	//タイマーストップ
	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_WAIT_AIRPUSH);

	// リスポーン設定
	ReaspawnSetting();
}

//==========================================================================
// リスポーン設定
//==========================================================================
void CPlayer::ReaspawnSetting()
{
	SetMove(0.0f);

	MyLib::Vector3 pos = GetPosition();
	m_pBaggage->SetPosition(MyLib::Vector3(pos.x, MAX_HEIGHT, pos.z));

	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	pCamera->SetLenDest(pCamera->GetOriginDistance(), 0, 1.0f, 1.0f);
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
	if (m_state != STATE_DEAD && m_state != STATE_RETURN && m_state != STATE_RESTART)
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

	if (pos.y <= 0.0f)
	{
		pos.y = 0.0f;

		// ジャンプ使用可能にする
		m_bJump = false;
		move.y = 0.0f;
		bNowLand = true;
		m_sMotionFrag.bJump = false;
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
MyLib::HitResult_Character CPlayer::Hit(const int nValue)
{
	MyLib::HitResult_Character hitresult = {};

	if (m_state == STATE::STATE_DEAD ||
		m_state == STATE::STATE_DEADWAIT ||
		m_state == STATE::STATE_RETURN ||
		m_state == STATE::STATE_RESTART ||
		m_state == STATE::STATE_RESPAWN)
	{
		hitresult.isdeath = true;
		return hitresult;
	}

	// 体力取得
	int nLife = GetLife();

	// 振動
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (nLife % 4 == 0)
	{
		float ratioDest = 1.0f - static_cast<float>(nLife) / GetLifeOrigin();
		float ratio = ratioDest;
		UtilFunc::Transformation::Clamp(ratioDest, 0.0f, 0.7f);
		UtilFunc::Transformation::Clamp(ratio, 0.1f, 1.0f);
		pCamera->SetShake(3,  50.0f * ratio, 0.0f);	// 振動
	}

	nLife -= nValue;

	UtilFunc::Transformation::Clamp(nLife, 0, GetLifeOrigin());

	// 体力設定
	SetLife(nLife);

	if (nLife <= 0)
	{// 体力がなくなったら

		// 死亡時の設定
		DeadSetting(&hitresult);
		pCamera->SetLenDest(600.0f, 200, -25.0f, 0.375f);	// 距離を近づける
		pCamera->SetStateCameraV(new CStateCameraV_Distance);

		// フィードバックエフェクトOFF
		CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(false);
	}
	else if (nLife <= static_cast<float>(GetLifeOrigin()) * 0.65f)
	{
		pCamera->SetLenDest(600.0f, 10, -20.0f, 0.07f);	// 距離を近づける
		CStateCameraV_Distance* pstate = new CStateCameraV_Distance;
		pstate->SetStartDistance(pCamera->GetDistance());
		pCamera->SetStateCameraV(pstate);

		// フィードバックエフェクトON
		CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(true, 0.6f, 1.02f, 120.0f);
	}
	

	// 過去の状態保存
	m_Oldstate = m_state;

	// 当たった判定を返す
	return hitresult;
}

//==========================================================================
// 死亡時の設定
//==========================================================================
void CPlayer::DeadSetting(MyLib::HitResult_Character* result)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	// 死状態
	m_state = STATE_DEAD;

	// 遷移カウンター設定
	m_nCntState = DEADTIME;

	// やられモーション
	GetMotion()->Set(MOTION_KNOCKBACK);

	// ノックバックの位置更新
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	m_posKnokBack = pos;

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
	// 状態遷移カウンター減算
	m_nCntState++;

	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	if (m_nCntState >= TIME_DMG)
	{
		m_state = STATE_INVINCIBLE;
		m_nCntState = INVINCIBLE_TIME;

		// モーション取得
		CMotion* pMotion = GetMotion();
		if (pMotion == nullptr)
		{
			return;
		}
		pMotion->ToggleFinish(true);
	}
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

		//m_state = STATE::STATE_FADEOUT;	// 死亡待機状態
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
		GetMotion()->Set(MOTION_DEAD);

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

		// リトライUIなければ生成
		if (m_pRetryUI == nullptr)
		{
			m_pRetryUI = CRetry_Ui::Create();
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
// チェックポイントに戻る状態
//==========================================================================
void CPlayer::StateReturn()
{
	// 状態遷移カウンター減算
	m_nCntState--;

	// 色設定
	//m_mMatcol.a = (float)m_nCntState / (float)FADEOUTTIME;

	// ぶっ倒れモーション
	//GetMotion()->Set(MOTION_DEAD);

	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	if (fadestate == CInstantFade::STATE::STATE_NONE)
	{
		pFade->SetFade(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 40);
	}
	else if (fadestate == CInstantFade::STATE::STATE_FADECOMPLETION)
	{
#if 0
		// 死亡処理
		Kill();

		// 終了処理
		Uninit();
#else
		// チェックポイントに復活
		ReaspawnCheckPoint();
		m_state = STATE::STATE_RESPAWN;
		SetLife(GetLifeOrigin());

		// リトライUIを消す
		if (m_pRetryUI != nullptr)
		{
			m_pRetryUI->Uninit();
			m_pRetryUI = nullptr;
		}
#endif
	}

	//if (m_nCntState <= 0)
	//{// 遷移カウンターが0になったら

	//	// 死亡処理
	//	Kill();

	//	// 終了処理
	//	Uninit();
	//	return;
	//}
}

//==========================================================================
//スタートに戻る状態
//==========================================================================
void CPlayer::StateRestart()
{
	// 状態遷移カウンター減算
	m_nCntState--;

	// 色設定
	//m_mMatcol.a = (float)m_nCntState / (float)FADEOUTTIME;

	// ぶっ倒れモーション
	//GetMotion()->Set(MOTION_DEAD);

	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	if (fadestate == CInstantFade::STATE::STATE_NONE)
	{
		pFade->SetFade(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 40);
	}
	else if (fadestate == CInstantFade::STATE::STATE_FADECOMPLETION)
	{
#if 0
		// 死亡処理
		Kill();

		// 終了処理
		Uninit();
#else
		// チェックポイントに復活
		RespawnStart();
		m_state = STATE::STATE_RESPAWN;
		SetLife(GetLifeOrigin());

		// リトライUIを消す
		if (m_pRetryUI != nullptr)
		{
			m_pRetryUI->Uninit();
			m_pRetryUI = nullptr;
		}
#endif
	}
}

//==========================================================================
// リスポーン
//==========================================================================
void CPlayer::StateRespawn()
{
	CInstantFade* pFade = CManager::GetInstance()->GetInstantFade();
	CInstantFade::STATE fadestate = pFade->GetState();

	if (fadestate == CInstantFade::STATE::STATE_NONE)
	{
		m_state = STATE::STATE_NONE;
	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// 参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// バッファへの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	// テスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z+ステンシル成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z+ステンシル失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテストのみ失敗

	// ステンシル描画
	CObjectChara::Draw();

	// ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// 普通の描画
	if (m_state == STATE_DMG)
	{
		CObjectChara::Draw(m_mMatcol);
	}
	else if (m_state == STATE_INVINCIBLE)
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
