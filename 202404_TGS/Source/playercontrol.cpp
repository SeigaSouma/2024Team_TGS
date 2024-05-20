//=============================================================================
// 
//  プレイヤーコントロール処理 [playercontrol.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "playercontrol.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "camera.h"
#include "enemy.h"
#include "game.h"

namespace
{
	const float MULTIPLIY_DASH = 2.0f;		// ダッシュの倍率
	const float STAMINA_AVOID = 30.0f;		// 回避のスタミナ消費量
	const float LENGTH_AUTOFACE = 200.0f;	// 自動で向く長さ
	const float LENGTH_COLLISIONRANGE = 500.0f;		// 当たり判定する範囲の長さ
	const float LENGTH_COLLISIONHEIGHT = 1000.0f;	// 当たり判定する高さ上限
	float ADD_HEIGHT = 25.0f;					// 高さの加算量
	const float MIN_HEIGHT = 100.0f;
	const float HEIGHT_VELOCITY = 10.0f;
}

#define GEKIMUZU (true)

//==========================================================================
// 通常移動
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player)
{
	// インプット情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();


	// ダッシュ判定
	bool bDash = false;
	if (pInputGamepad->GetPress(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()) &&
		pInputGamepad->IsTipStick())
	{// 左スティックが倒れてる場合
		bDash = true;
	}
	player->SetEnableDash(bDash);


	// 移動量取得
	float fMove = player->GetVelocity();
	if (bDash){
		// ダッシュ倍率掛ける
		fMove *= MULTIPLIY_DASH;
	}


	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();
	Camerarot.y -= pCamera->GetOriginRotation().y;

	// 目標の向き取得
	float fRotDest = player->GetRotDest();

	// 移動量取得
	MyLib::Vector3 move = player->GetMove();

	// モーション情報取得
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// 状態取得
	CPlayer::STATE state = player->GetState();

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		state != CPlayer::STATE::STATE_KNOCKBACK &&
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_DEADWAIT &&
		state != CPlayer::STATE::STATE_FADEOUT)
	{// 移動可能モーションの時

		move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * 0.5f);
		move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * 0.5f);

		if (pInputKeyboard->GetPress(DIK_A))
		{// 左移動

			// 移動中にする
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// 左上移動

				move.x += sinf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// 左下移動

				move.x += sinf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// 左移動

				move.x += sinf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{// 右移動

			// 移動中にする
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// 右上移動

				move.x += sinf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// 右下移動

				move.x += sinf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// 右移動

				move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{// 上移動

			// 移動中にする
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{// 下移動

			// 移動中にする
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 1.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
		else
		{
			motionFrag.bMove = false;
		}

		if (pInputGamepad->IsTipStick())
		{// 左スティックが倒れてる場合

			// 移動中にする
			motionFrag.bMove = true;

			// スティックの向き取得
			float stickrot = pInputGamepad->GetStickRotL(player->GetMyPlayerIdx());
			UtilFunc::Transformation::RotNormalize(stickrot);

#if 0
			// 移動量と向き設定
			move.x += sinf(stickrot + Camerarot.y) * fMove;
			move.z += cosf(stickrot + Camerarot.y) * fMove;
			fRotDest = D3DX_PI + stickrot + Camerarot.y;
#endif
			int angle = (stickrot <= 0.0f) ? -1 : 1;

			if (angle == -1) fMove *= 0.3f;

			move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * angle);
			move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * angle);
			fRotDest = angle * (-D3DX_PI * 0.5f) + Camerarot.y;
		}

		// ジャンプ状況取得
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// キャンセル可能 && 移動中

			// モーション終了判定
			pMotion->ToggleFinish(true);

			// 移動モーション
			if (bDash)
			{
				pMotion->Set(CPlayer::MOTION::MOTION_DASH);
			}
			else
			{
				pMotion->Set(CPlayer::MOTION::MOTION_WALK);
			}
		}

#if 0
		if (!bJump &&
			(pInputKeyboard->GetTrigger(DIK_SPACE) || pInputGamepad->GetTrigger(CInputGamepad::BUTTON_A, player->GetMyPlayerIdx())))
		{// ジャンプ

			bJump = true;
			motionFrag.bJump = true;
			move.y += 17.0f;

			pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

			// サウンド再生
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_JUMP);
		}
#endif

		// ジャンプ判定設定
		player->SetEnableJump(bJump);

	}
	else if (
		pMotion->IsGetMove(nMotionType) == 0 &&	// 移動可能なモーションか取得
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_FADEOUT)
	{
		if (pInputKeyboard->GetPress(DIK_A))
		{//←キーが押された,左移動

			if (pInputKeyboard->GetPress(DIK_W))
			{//A+W,左上移動
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//A+S,左下移動
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,左移動
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{//Dキーが押された,右移動

			if (pInputKeyboard->GetPress(DIK_W))
			{//D+W,右上移動
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//D+S,右下移動
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,右移動
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{//Wが押された、上移動
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{//Sが押された、下移動
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
	}

	// モーションフラグ設定
	player->SetMotionFrag(motionFrag);

	// 移動量設定
	player->SetMove(move);

	// 角度の正規化
	UtilFunc::Transformation::RotNormalize(fRotDest);
	player->SetRotDest(fRotDest);
}

//==========================================================================
// アクション
//==========================================================================
void CPlayerControlBaggage::Action(CPlayer* player, CBaggage* pBaggage)
{
	// インプット情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	CGameManager* pGameMgr = CGame::GetInstance()->GetGameManager();

	if (pGameMgr->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH &&
		(CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN) ||
			CInputGamepad::GetInstance()->GetTrigger(CInputGamepad::BUTTON::BUTTON_A, 0)))
	{// 空気送り待ちで空気発射

		// メインに移行
		pGameMgr->SetType(CGameManager::SceneType::SCENE_MAIN);
	}


	// 情報取得
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();


	/*if (m_pBressRange == nullptr)
	{
		m_pBressRange = CDebugBressRange::Create();
	}
	if (m_pBressHeight == nullptr)
	{
		m_pBressHeight = CDebugBressRange::Create();
		m_pBressHeight->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}*/

	static bool fall = true;

	static float up = 8.3f, power = 9.0f;
	//static float up = 8.3f, power = 6.8f;
	//static float up = 2.5f, power = 2.0f;
	ImGui::DragFloat("up", &up, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("power", &power, 0.01f, 0.0f, 0.0f, "%.2f");

	ImGui::DragFloat("Add Height", &ADD_HEIGHT, 1.0f, 0.0f, 0.0f, "%.2f");


	float ratio = (posBaggage.y - posBaggageOrigin.y) / LENGTH_COLLISIONHEIGHT;
	float ratioHeight = 1.0f - ratio;
	ratioHeight = UtilFunc::Transformation::Clamp(ratioHeight, 0.5f, 1.0f);

	ratio = UtilFunc::Transformation::Clamp(ratio, 0.3f, 1.0f);

	float range = ratio * LENGTH_COLLISIONRANGE;

#if _DEBUG
	MyLib::Vector3 a = pos, b = pos;
	a.x += range; b.x -= range;
	a.y = posBaggage.y; b.y = posBaggage.y;

	MyLib::Vector3 c = pos;
	c.y = posBaggageOrigin.y + LENGTH_COLLISIONHEIGHT;

	CEffect3D::Create(
		c,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		a,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	CEffect3D::Create(
		b,
		MyLib::Vector3(0.0f, 0.0f, 0.0f),
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
		20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	MyLib::Vector3 leftup = posBaggageOrigin, rightup = posBaggageOrigin, leftdw = posBaggageOrigin, rightdw = posBaggageOrigin;
	leftup.y += LENGTH_COLLISIONHEIGHT; leftup.x -= LENGTH_COLLISIONRANGE;
	rightup.y += LENGTH_COLLISIONHEIGHT; rightup.x += LENGTH_COLLISIONRANGE;
	leftdw.x -= LENGTH_COLLISIONRANGE * 0.3f;
	rightdw.x += LENGTH_COLLISIONRANGE * 0.3f;

	if (m_pBressRange != nullptr)
	{
		m_pBressRange->SetRange(leftup, rightup, leftdw, rightdw);
		m_pBressRange->SetPosition(pos);
	}
#endif

#if GEKIMUZU
	if (posBaggage.y <= posBaggageOrigin.y) posBaggage.y = posBaggageOrigin.y;
	pBaggage->SetPosition(MyLib::Vector3(pos.x, posBaggage.y, pos.z));
#endif

	if (m_pBressHeight != nullptr)
	{
		float ratiooo = m_fHeight / LENGTH_COLLISIONHEIGHT;

		float rangeee = ratiooo * LENGTH_COLLISIONRANGE;

		/*leftup.x = -rangeee - (LENGTH_COLLISIONRANGE * 0.3f);
		rightup.x = rangeee + (LENGTH_COLLISIONRANGE * 0.3f);*/
		leftup.y = posBaggageOrigin.y + m_fHeight;
		rightup.y = posBaggageOrigin.y + m_fHeight;
		m_pBressHeight->SetRange(leftup, rightup, leftdw, rightdw);
		m_pBressHeight->SetPosition(pos);
	}


	if (CInputKeyboard::GetInstance()->GetPress(DIK_RETURN) ||
		CInputGamepad::GetInstance()->GetPress(CInputGamepad::BUTTON::BUTTON_A, 0))
	{
		if (fall) {
			fall = false;
			pBaggage->SetForce(0.0f);

			if (m_BressHandle != nullptr)
			{
				CMyEffekseer::GetInstance()->SetTrigger(*m_BressHandle, 1);
			}

			MyLib::Vector3 d = pos;
			d.y = posBaggageOrigin.y;
			CMyEffekseer::GetInstance()->SetEffect(
				&m_BressHandle,
				CMyEffekseer::EFKLABEL::EFKLABEL_BRESS,
				d, 0.0f, 0.0f, 25.0f, true);
		}

		m_fHeightVelocity += (0.0f - m_fHeightVelocity) * 0.2f;
		m_fHeight += ADD_HEIGHT + m_fHeightVelocity;
		m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

		if (posBaggage.y <= posBaggageOrigin.y + m_fHeight &&
			posBaggage.x <= pos.x + range &&
			posBaggage.x >= pos.x - range)
		{

#if GEKIMUZU
			pBaggage->AddForce(MyLib::Vector3(0.0f, up * ratioHeight, 0.0f), player->GetPosition() + move);
#else
			pBaggage->SetMove(MyLib::Vector3(move.x, pBaggage->GetMove().y, move.z));
			pBaggage->AddForce(MyLib::Vector3(ratio * power, up * ratioHeight, 0.0f), player->GetPosition() + move);
#endif
		}
	}
	else
	{
		m_fHeight -= ADD_HEIGHT * 2.0f;
		m_fHeightVelocity += (m_fHeightVelocity - HEIGHT_VELOCITY) * 0.1f;
		m_fHeightVelocity = UtilFunc::Transformation::Clamp(m_fHeightVelocity, 0.0f, HEIGHT_VELOCITY);
	}
	m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

	if (CInputKeyboard::GetInstance()->GetRelease(DIK_RETURN) ||
		CInputGamepad::GetInstance()->GetRelease(CInputGamepad::BUTTON::BUTTON_A, 0))
	{
		// 降下状態
		fall = true;

	}


	if (posBaggage.y <= MIN_HEIGHT)
	{
		fall = true;
	}

	if (fall && m_BressHandle != nullptr)
	{
		//CMyEffekseer::GetInstance()->Stop(m_BressHandle);
		CMyEffekseer::GetInstance()->SetTrigger(*m_BressHandle, 1);
	}


	MyLib::Vector3 d = pos;
	d.y = posBaggageOrigin.y;

	// 位置設定
	if (m_BressHandle != nullptr)
	{
		CMyEffekseer::GetInstance()->SetPosition(*m_BressHandle, d);
	}

}

//==========================================================================
// 攻撃可能フラグ取得
//==========================================================================
bool CPlayerControlAttack::IsAttack(CPlayer* player)
{
	// インプット情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();

	// 現在の種類取得
	CMotion* pMotion = player->GetMotion();

	if ((pMotion->IsGetCombiable() || pMotion->IsGetCancelable()) &&
		!player->IsJump())
	{
		return true;
	}
	return false;
}

//==========================================================================
// 通常攻撃
//==========================================================================
void CPlayerControlAttack::Attack(CPlayer* player)
{
	
}

//==========================================================================
// 通常攻撃
//==========================================================================
void CPlayerControlAttack_Level1::Attack(CPlayer* player)
{

}



//==========================================================================
// 防御
//==========================================================================
// 基底
void CPlayerControlDefence::Defence(CPlayer* player)
{
	
}

// レベル1
void CPlayerControlDefence_Level1::Defence(CPlayer* player)
{
	
}

//==========================================================================
// 回避
//==========================================================================
void CPlayerControlAvoid::Avoid(CPlayer* player)
{

}




//==========================================================================
// ガード
//==========================================================================
// 受けた時の処理
void CPlayerGuard::HitProcess(CPlayer* player, MyLib::Vector3 enemypos)
{
	
	// 位置取得
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 rot = player->GetRotation();

	// 目標の角度設定
	float fRotDest = atan2f((pos.x - enemypos.x), (pos.z - enemypos.z));
	player->SetRotation(MyLib::Vector3(rot.x, fRotDest, rot.z));
	player->SetRotDest(fRotDest);

	player->SetMove(MyLib::Vector3(
		sinf(fRotDest) * 40.0f,
		0.0f,
		cosf(fRotDest) * 40.0f));
}