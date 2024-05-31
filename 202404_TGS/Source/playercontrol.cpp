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
#include "game.h"
#include "debugproc.h"
#include "keyconfig_gamepad.h"
#include "map_obstacle.h"
#include "collisionLine_Box.h"
#include "keyconfig.h"

namespace
{
	const float MULTIPLIY_DASH = 2.0f;		// ダッシュの倍率
	const float STAMINA_AVOID = 30.0f;		// 回避のスタミナ消費量
	const float LENGTH_AUTOFACE = 200.0f;	// 自動で向く長さ
	const float LENGTH_COLLISIONRANGE = 500.0f;		// 当たり判定する範囲の長さ
	const float RATIO_COLLISIONRANGE = 0.3f;		// 範囲の長さの最小割合
	const float LENGTH_COLLISIONHEIGHT = 1000.0f;	// 当たり判定する高さ上限
	float ADD_HEIGHT = 25.0f;					// 高さの加算量
	const float MIN_HEIGHT = 100.0f;
	const float HEIGHT_VELOCITY = 10.0f;
	float MAX_SURHEIGHT = 100.0f;
	float SURHEIGHT_VELOCITY = (10.0f);
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

#if _DEBUG
	if (!pInputGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_BACK, 0))
#endif
	{
		// 移動量設定
		player->SetMove(move);
	}

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
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CGameManager* pGameMgr = CGame::GetInstance()->GetGameManager();

	if (pGameMgr->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH &&
		(CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN) ||
			pKeyConfigPad->GetTrigger(INGAME::ACTION::ACT_AIR)))
	{// 空気送り待ちで空気発射

		// メインに移行
		pGameMgr->SetType(CGameManager::SceneType::SCENE_MAIN);
	}

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT)) {
		std::thread th(&CKeyConfig::Setting, pKeyConfigPad, INGAME::ACTION::ACT_AIR);
		th.detach();
	}


	// 情報取得
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();


	if (m_pBressRange == nullptr)
	{
		m_pBressRange = CDebugBressRange::Create();
	}
	if (m_pBressHeight == nullptr)
	{
		m_pBressHeight = CDebugBressRange::Create();
		m_pBressHeight->SetColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.3f));
	}

	static bool fall = true;

	static float up = 8.3f, power = 9.0f;
	//static float up = 8.3f, power = 6.8f;
	//static float up = 2.5f, power = 2.0f;
	ImGui::DragFloat("up", &up, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("power", &power, 0.01f, 0.0f, 0.0f, "%.2f");

	ImGui::DragFloat("Add Height", &ADD_HEIGHT, 1.0f, 0.0f, 0.0f, "%.2f");

	static float starttimeDownheight = 2.0f;	// 降下が始まるまでの時間
	static float timeDownheight = 2.0f;			// 落ちきるまでの時間
	static float ratioMinDownheight = 0.2f;		// 落ちきった時の再下底割合
	ImGui::DragFloat("Start Time DownHeight", &starttimeDownheight, 0.05f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Time DownHeight", &timeDownheight, 0.05f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Ratio Min DownHeight", &ratioMinDownheight, 0.01f, 0.0f, 0.0f, "%.2f");	

	// 荷物の高さで割合設定
	float ratio = (posBaggage.y - posBaggageOrigin.y) / LENGTH_COLLISIONHEIGHT;
	float ratioHeight = 1.0f - ratio;
	ratioHeight = UtilFunc::Transformation::Clamp(ratioHeight, 0.5f, 1.0f);

	// 割合
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.3f, 1.0f);

	// 息の届く横範囲
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

	// 息の範囲生成
	if (m_pBressRange != nullptr)
	{
		m_pBressRange->SetRange(leftup, rightup, leftdw, rightdw);
		m_pBressRange->SetPosition(pos);
	}
#endif

#if GEKIMUZU

#if _DEBUG
	if (!pInputGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_BACK, 0))
#endif
	{
		// 高さ制限
		if (posBaggage.y <= posBaggageOrigin.y)
		{
			posBaggage.y = posBaggageOrigin.y;
			player->Hit(1);
		}
		else
		{
			player->SetLife(player->GetLifeOrigin());
		}
	}

	// 位置設定
	pBaggage->SetPosition(MyLib::Vector3(pos.x, posBaggage.y, pos.z));


	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// 向き設定
	MyLib::Vector3 setRot;
	setRot.x += sinf(D3DX_PI * 0.5f + 0.0f) * 1.0f;
	setRot.z += cosf(D3DX_PI * 0.5f + 0.0f) * 1.0f;
	pBaggage->SetRotation(MyLib::Vector3(0.0f, setRot.AngleXZ(0.0f), 0.0f));
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
	
	bool bKantsu = CollisionObstacle(player, pBaggage);
	if (CInputKeyboard::GetInstance()->GetPress(DIK_RETURN) ||
		pKeyConfigPad->GetPress(INGAME::ACT_AIR))
	{
		// 高さの降下時間加算
		m_fTimeDownHeight += CManager::GetInstance()->GetDeltaTime();


		if (fall) 
		{// 落下中

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
				d, 0.0f, 0.0f, 90.0f, true);
		}

		m_fHeightVelocity += (0.0f - m_fHeightVelocity) * 0.2f;
		m_fHeight += ADD_HEIGHT + m_fHeightVelocity;
		m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

		if (posBaggage.y <= posBaggageOrigin.y + m_fHeight &&
			posBaggage.x <= pos.x + range &&
			posBaggage.x >= pos.x - range)
		{// 範囲内

			if (bKantsu)
			{// 障害物の空気貫通判定

#if GEKIMUZU
				// 荷物へ空気移動量加算
				pBaggage->AddForce(MyLib::Vector3(0.0f, up * ratioHeight, 0.0f), player->GetPosition() + move);
#else
				pBaggage->SetMove(MyLib::Vector3(move.x, pBaggage->GetMove().y, move.z));
				pBaggage->AddForce(MyLib::Vector3(ratio * power, up * ratioHeight, 0.0f), player->GetPosition() + move);
#endif
			}
		}
	}
	else
	{
		// 降下状態
		fall = true;

		// 高さの降下時間減算
		m_fTimeDownHeight = 0.0f;
		//m_fTimeDownHeight -= CManager::GetInstance()->GetDeltaTime();

		m_fHeight -= ADD_HEIGHT * 2.0f;
		m_fHeightVelocity += (m_fHeightVelocity - HEIGHT_VELOCITY) * 0.1f;
		m_fHeightVelocity = UtilFunc::Transformation::Clamp(m_fHeightVelocity, 0.0f, HEIGHT_VELOCITY);
	}

	// 高さの降下時間補正
	m_fTimeDownHeight = UtilFunc::Transformation::Clamp(m_fTimeDownHeight, 0.0f, starttimeDownheight + timeDownheight);

	// 息の高さ補正
	m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

	// 息の届く最大の高さが降下していく
	if (m_fTimeDownHeight >= starttimeDownheight)
	{
		float timeratio = (m_fTimeDownHeight - starttimeDownheight) / timeDownheight;
		timeratio = UtilFunc::Transformation::Clamp(timeratio, 0.0f, 1.0f);
		m_fHeight = (1.0f - timeratio) * LENGTH_COLLISIONHEIGHT;
		m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, LENGTH_COLLISIONHEIGHT * ratioMinDownheight, LENGTH_COLLISIONHEIGHT);
	}


	if (posBaggage.y <= MIN_HEIGHT)
	{
		fall = true;
	}

	if (fall && m_BressHandle != nullptr)
	{
		//CMyEffekseer::GetInstance()->Stop(m_BressHandle);
		CMyEffekseer::GetInstance()->SetTrigger(*m_BressHandle, 0);
	}


	MyLib::Vector3 d = pos;
	d.y = posBaggageOrigin.y;

	// 位置設定
	if (m_BressHandle != nullptr)
	{
		CMyEffekseer::GetInstance()->SetPosition(*m_BressHandle, d);

		// デバッグ表示
		CManager::GetInstance()->GetDebugProc()->Print(
			"------------------[プレイヤーの操作]------------------\n"
			"移動中のエフェクト 【%d】\n"
			, *m_BressHandle);
	}
}

//==========================================================================
// 障害物判定
//==========================================================================
bool CPlayerControlBaggage::CollisionObstacle(CPlayer* player, CBaggage* pBaggage)
{
	// 障害物のリスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	// 情報取得
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();

	float range = LENGTH_COLLISIONRANGE * RATIO_COLLISIONRANGE;
	MyLib::AABB bressAABB = MyLib::AABB(MyLib::Vector3(-range, 0.0f, -range), MyLib::Vector3(range, LENGTH_COLLISIONHEIGHT, range));

#if _DEBUG
	// ボックス生成
	if (m_pBox == nullptr)
	{
		m_pBox = CCollisionLine_Box::Create(bressAABB, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}

	// 位置設定
	MyLib::Matrix mtx;
	MyLib::Vector3 trans = posBaggage;
	trans.y = posBaggageOrigin.y;

	mtx.Translation(trans);

	if (m_pBox != nullptr)
	{
		m_pBox->SetPosition(trans);
	}
#endif

	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;

		// 情報取得
		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();

		if (info.setup.isAir == 0) continue;	// 空気貫通判定

		MyLib::Vector3 ObjPos = pObj->GetPosition();

		if (posBaggage.y <= ObjPos.y) continue;	// 障害物と荷物の高さ判定

		for (const auto& collider : info.boxcolliders)
		{
			if (UtilFunc::Collision::IsAABBCollidingWithBox(bressAABB, mtx, MyLib::AABB(collider.vtxMin, collider.vtxMax), collider.worldmtx))
			{
				m_pBressRange->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.3f));
				return false;
			}
		}
	}

	m_pBressRange->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f));
	return true;
}


//==========================================================================
// 浮上操作
//==========================================================================
float CPlayerControlSurfacing::Surfacing(CPlayer* player)
{
	// インプット情報取得
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	// 浮上判定
	bool bUp = false;

	// モーション情報取得
	CMotion* pMotion = player->GetMotion();

	//if (pMotion->) {	// 浮上可能ではない
	//	return m_fHeight;
	//}

	if (CInputKeyboard::GetInstance()->GetPress(DIK_W) ||
		pKeyConfigPad->GetPress(INGAME::ACTION::ACT_UPDOWN))
	{// 入力している
		bUp = true;
	}

	ImGui::DragFloat("SurHeight", &MAX_SURHEIGHT, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("SurSpeed", &SURHEIGHT_VELOCITY, 0.1f, 0.0f, 0.0f, "%.2f");


	if (bUp) {	// 上昇
		m_fHeight += SURHEIGHT_VELOCITY;

		if (m_fHeight > MAX_SURHEIGHT) {
			m_fHeight = MAX_SURHEIGHT;
		}
	}
	else {	// 下降
		m_fHeight -= SURHEIGHT_VELOCITY;

		if (m_fHeight < 0.0f) {
			m_fHeight = 0.0f;
		}
	}

	return m_fHeight;
}