//=============================================================================
// 
//  �v���C���[�R���g���[������ [playercontrol.cpp]
//  Author : ���n�Ή�
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
	const float MULTIPLIY_DASH = 2.0f;		// �_�b�V���̔{��
	const float STAMINA_AVOID = 30.0f;		// ����̃X�^�~�i�����
	const float LENGTH_AUTOFACE = 200.0f;	// �����Ō�������
	const float LENGTH_COLLISIONRANGE = 500.0f;		// �����蔻�肷��͈͂̒���
	const float RATIO_COLLISIONRANGE = 0.3f;		// �͈͂̒����̍ŏ�����
	const float LENGTH_COLLISIONHEIGHT = 1000.0f;	// �����蔻�肷�鍂�����
	float ADD_HEIGHT = 25.0f;					// �����̉��Z��
	const float MIN_HEIGHT = 100.0f;
	const float HEIGHT_VELOCITY = 10.0f;
	float MAX_SURHEIGHT = 100.0f;
	float SURHEIGHT_VELOCITY = (10.0f);
}

#define GEKIMUZU (true)

//==========================================================================
// �ʏ�ړ�
//==========================================================================
void CPlayerControlMove::Move(CPlayer* player)
{
	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();


	// �_�b�V������
	bool bDash = false;
	if (pInputGamepad->GetPress(CInputGamepad::BUTTON_LB, player->GetMyPlayerIdx()) &&
		pInputGamepad->IsTipStick())
	{// ���X�e�B�b�N���|��Ă�ꍇ
		bDash = true;
	}
	player->SetEnableDash(bDash);


	// �ړ��ʎ擾
	float fMove = player->GetVelocity();
	if (bDash){
		// �_�b�V���{���|����
		fMove *= MULTIPLIY_DASH;
	}


	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();
	Camerarot.y -= pCamera->GetOriginRotation().y;

	// �ڕW�̌����擾
	float fRotDest = player->GetRotDest();

	// �ړ��ʎ擾
	MyLib::Vector3 move = player->GetMove();

	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();
	int nMotionType = pMotion->GetType();
	CPlayer::SMotionFrag motionFrag = player->GetMotionFrag();

	// ��Ԏ擾
	CPlayer::STATE state = player->GetState();

	if ((pMotion->IsGetMove(nMotionType) == 1 || pMotion->IsGetCancelable()) &&
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_DEADWAIT &&
		state != CPlayer::STATE::STATE_FADEOUT)
	{// �ړ��\���[�V�����̎�

		move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * 0.5f);
		move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * (fMove * 0.5f);

		if (pInputKeyboard->GetPress(DIK_A))
		{// ���ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// ����ړ�

				move.x += sinf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// �����ړ�

				move.x += sinf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// ���ړ�

				move.x += sinf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(-D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{// �E�ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;

			if (pInputKeyboard->GetPress(DIK_W))
			{// �E��ړ�

				move.x += sinf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.25f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{// �E���ړ�

				move.x += sinf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.75f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{// �E�ړ�

				move.x += sinf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				move.z += cosf(D3DX_PI * 0.5f + Camerarot.y) * fMove;
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{// ��ړ�

			// �ړ����ɂ���
			motionFrag.bMove = true;
			move.x += sinf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			move.z += cosf(D3DX_PI * 0.0f + Camerarot.y) * fMove;
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{// ���ړ�

			// �ړ����ɂ���
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
		{// ���X�e�B�b�N���|��Ă�ꍇ

			// �ړ����ɂ���
			motionFrag.bMove = true;

			// �X�e�B�b�N�̌����擾
			float stickrot = pInputGamepad->GetStickRotL(player->GetMyPlayerIdx());
			UtilFunc::Transformation::RotNormalize(stickrot);

#if 0
			// �ړ��ʂƌ����ݒ�
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

		// �W�����v�󋵎擾
		bool bJump = player->IsJump();

		if (motionFrag.bMove &&
			!bJump)
		{// �L�����Z���\ && �ړ���

			// ���[�V�����I������
			pMotion->ToggleFinish(true);

			// �ړ����[�V����
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
		{// �W�����v

			bJump = true;
			motionFrag.bJump = true;
			move.y += 17.0f;

			pMotion->Set(CPlayer::MOTION::MOTION_JUMP);

			// �T�E���h�Đ�
			CManager::GetInstance()->GetSound()->PlaySound(CSound::LABEL_SE_JUMP);
		}
#endif

		// �W�����v����ݒ�
		player->SetEnableJump(bJump);

	}
	else if (
		pMotion->IsGetMove(nMotionType) == 0 &&	// �ړ��\�ȃ��[�V�������擾
		state != CPlayer::STATE::STATE_DEAD &&
		state != CPlayer::STATE::STATE_FADEOUT)
	{
		if (pInputKeyboard->GetPress(DIK_A))
		{//���L�[�������ꂽ,���ړ�

			if (pInputKeyboard->GetPress(DIK_W))
			{//A+W,����ړ�
				fRotDest = D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//A+S,�����ړ�
				fRotDest = D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//A,���ړ�
				fRotDest = D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_D))
		{//D�L�[�������ꂽ,�E�ړ�

			if (pInputKeyboard->GetPress(DIK_W))
			{//D+W,�E��ړ�
				fRotDest = -D3DX_PI * 0.75f + Camerarot.y;
			}
			else if (pInputKeyboard->GetPress(DIK_S))
			{//D+S,�E���ړ�
				fRotDest = -D3DX_PI * 0.25f + Camerarot.y;
			}
			else
			{//D,�E�ړ�
				fRotDest = -D3DX_PI * 0.5f + Camerarot.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_W))
		{//W�������ꂽ�A��ړ�
			fRotDest = D3DX_PI * 1.0f + Camerarot.y;
		}
		else if (pInputKeyboard->GetPress(DIK_S))
		{//S�������ꂽ�A���ړ�
			fRotDest = D3DX_PI * 0.0f + Camerarot.y;
		}
	}

	// ���[�V�����t���O�ݒ�
	player->SetMotionFrag(motionFrag);

#if _DEBUG
	if (!pInputGamepad->GetPress(CInputGamepad::BUTTON::BUTTON_BACK, 0))
#endif
	{
		// �ړ��ʐݒ�
		player->SetMove(move);
	}

	// �p�x�̐��K��
	UtilFunc::Transformation::RotNormalize(fRotDest);
	player->SetRotDest(fRotDest);
}

//==========================================================================
// �A�N�V����
//==========================================================================
void CPlayerControlBaggage::Action(CPlayer* player, CBaggage* pBaggage)
{
	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CGameManager* pGameMgr = CGame::GetInstance()->GetGameManager();

	if (pGameMgr->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH &&
		(CInputKeyboard::GetInstance()->GetTrigger(DIK_RETURN) ||
			pKeyConfigPad->GetTrigger(INGAME::ACTION::ACT_AIR)))
	{// ��C����҂��ŋ�C����

		// ���C���Ɉڍs
		pGameMgr->SetType(CGameManager::SceneType::SCENE_MAIN);
	}

	if (pInputKeyboard->GetTrigger(DIK_LSHIFT)) {
		std::thread th(&CKeyConfig::Setting, pKeyConfigPad, INGAME::ACTION::ACT_AIR);
		th.detach();
	}


	// ���擾
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

	static float starttimeDownheight = 2.0f;	// �~�����n�܂�܂ł̎���
	static float timeDownheight = 2.0f;			// ��������܂ł̎���
	static float ratioMinDownheight = 0.2f;		// �������������̍ĉ��ꊄ��
	ImGui::DragFloat("Start Time DownHeight", &starttimeDownheight, 0.05f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Time DownHeight", &timeDownheight, 0.05f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("Ratio Min DownHeight", &ratioMinDownheight, 0.01f, 0.0f, 0.0f, "%.2f");	

	// �ו��̍����Ŋ����ݒ�
	float ratio = (posBaggage.y - posBaggageOrigin.y) / LENGTH_COLLISIONHEIGHT;
	float ratioHeight = 1.0f - ratio;
	ratioHeight = UtilFunc::Transformation::Clamp(ratioHeight, 0.5f, 1.0f);

	// ����
	ratio = UtilFunc::Transformation::Clamp(ratio, 0.3f, 1.0f);

	// ���̓͂����͈�
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

	// ���͈̔͐���
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
		// ��������
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

	// �ʒu�ݒ�
	pBaggage->SetPosition(MyLib::Vector3(pos.x, posBaggage.y, pos.z));


	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 Camerarot = pCamera->GetRotation();

	// �����ݒ�
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
		// �����̍~�����ԉ��Z
		m_fTimeDownHeight += CManager::GetInstance()->GetDeltaTime();


		if (fall) 
		{// ������

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
		{// �͈͓�

			if (bKantsu)
			{// ��Q���̋�C�ђʔ���

#if GEKIMUZU
				// �ו��֋�C�ړ��ʉ��Z
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
		// �~�����
		fall = true;

		// �����̍~�����Ԍ��Z
		m_fTimeDownHeight = 0.0f;
		//m_fTimeDownHeight -= CManager::GetInstance()->GetDeltaTime();

		m_fHeight -= ADD_HEIGHT * 2.0f;
		m_fHeightVelocity += (m_fHeightVelocity - HEIGHT_VELOCITY) * 0.1f;
		m_fHeightVelocity = UtilFunc::Transformation::Clamp(m_fHeightVelocity, 0.0f, HEIGHT_VELOCITY);
	}

	// �����̍~�����ԕ␳
	m_fTimeDownHeight = UtilFunc::Transformation::Clamp(m_fTimeDownHeight, 0.0f, starttimeDownheight + timeDownheight);

	// ���̍����␳
	m_fHeight = UtilFunc::Transformation::Clamp(m_fHeight, MIN_HEIGHT, LENGTH_COLLISIONHEIGHT);

	// ���̓͂��ő�̍������~�����Ă���
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

	// �ʒu�ݒ�
	if (m_BressHandle != nullptr)
	{
		CMyEffekseer::GetInstance()->SetPosition(*m_BressHandle, d);

		// �f�o�b�O�\��
		CManager::GetInstance()->GetDebugProc()->Print(
			"------------------[�v���C���[�̑���]------------------\n"
			"�ړ����̃G�t�F�N�g �y%d�z\n"
			, *m_BressHandle);
	}
}

//==========================================================================
// ��Q������
//==========================================================================
bool CPlayerControlBaggage::CollisionObstacle(CPlayer* player, CBaggage* pBaggage)
{
	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	// ���擾
	MyLib::Vector3 move = player->GetMove();
	MyLib::Vector3 pos = player->GetPosition();
	MyLib::Vector3 posBaggage = pBaggage->GetPosition();
	MyLib::Vector3 posBaggageOrigin = pBaggage->GetOriginPosition();

	float range = LENGTH_COLLISIONRANGE * RATIO_COLLISIONRANGE;
	MyLib::AABB bressAABB = MyLib::AABB(MyLib::Vector3(-range, 0.0f, -range), MyLib::Vector3(range, LENGTH_COLLISIONHEIGHT, range));

#if _DEBUG
	// �{�b�N�X����
	if (m_pBox == nullptr)
	{
		m_pBox = CCollisionLine_Box::Create(bressAABB, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	}

	// �ʒu�ݒ�
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

		// ���擾
		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();

		if (info.setup.isAir == 0) continue;	// ��C�ђʔ���

		MyLib::Vector3 ObjPos = pObj->GetPosition();

		if (posBaggage.y <= ObjPos.y) continue;	// ��Q���Ɖו��̍�������

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
// ���㑀��
//==========================================================================
float CPlayerControlSurfacing::Surfacing(CPlayer* player)
{
	// �C���v�b�g���擾
	CInputKeyboard* pInputKeyboard = CInputKeyboard::GetInstance();
	CInputGamepad* pInputGamepad = CInputGamepad::GetInstance();
	CKeyConfigManager* pKeyConfigManager = CKeyConfigManager::GetInstance();
	CKeyConfig* pKeyConfigPad = pKeyConfigManager->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	// ���㔻��
	bool bUp = false;

	// ���[�V�������擾
	CMotion* pMotion = player->GetMotion();

	//if (pMotion->) {	// ����\�ł͂Ȃ�
	//	return m_fHeight;
	//}

	if (CInputKeyboard::GetInstance()->GetPress(DIK_W) ||
		pKeyConfigPad->GetPress(INGAME::ACTION::ACT_UPDOWN))
	{// ���͂��Ă���
		bUp = true;
	}

	ImGui::DragFloat("SurHeight", &MAX_SURHEIGHT, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("SurSpeed", &SURHEIGHT_VELOCITY, 0.1f, 0.0f, 0.0f, "%.2f");


	if (bUp) {	// �㏸
		m_fHeight += SURHEIGHT_VELOCITY;

		if (m_fHeight > MAX_SURHEIGHT) {
			m_fHeight = MAX_SURHEIGHT;
		}
	}
	else {	// ���~
		m_fHeight -= SURHEIGHT_VELOCITY;

		if (m_fHeight < 0.0f) {
			m_fHeight = 0.0f;
		}
	}

	return m_fHeight;
}