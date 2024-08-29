//=============================================================================
// 
//  �ו����� [baggage.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "baggage.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "camera.h"
#include "map_obstacle.h"
#include "renderer.h"
#include "glassclush.h"
#include "countdown_start.h"
#include "blackframe.h"
#include "splashwater.h"
#include "guide.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string MODEL[] =
	{
		"data\\MODEL\\map_object\\rock_02.x",	// �z
	};
	const float LIMIT_HEIGHT = 800.0f;	// �������
	const float VELOCITY = 1.0f;
	float PITCH_RATIO = 0.00025f;	// �s�b�`����]�{��
	float PITCH_INER = 0.075f;	// �s�b�`����]����
	float ROLL_FSTSPD = 0.70f;	// ���[������]����
	float ROLL_INER = 0.075f;		// ���[������]����
	float DEVIATION_WIDTH = 300.0f;	// �Ԃꕝ
	float DEVIATION_SPEED = 0.02f * D3DX_PI;	// �Ԃꑬ�x
	float DEADANGLE_HIT = D3DX_PI * 0.2f;		// �q�b�g���̎��S����
}

namespace StateTime
{
	const float DAMAGE = 0.2f;		// �_���[�W
	const float DEAD = 0.7f;		// ���S
	const float APPEARANCE = 0.4f;	// �o��
	const float PASS = 1.5f;		// �p�X
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CBaggage::STATE_FUNC CBaggage::m_StateFunc[] =
{
	&CBaggage::StateNone,		// �Ȃ�
	&CBaggage::StateDamage,		// �_���[�W
	&CBaggage::StateDead,		// ���S
	&CBaggage::StateAppearance_Wait,	// �o��
	&CBaggage::StateAppearance,	// �o��
	&CBaggage::StatePass,		// �p�X
	&CBaggage::StateGoal,		// �S�[��
	&CBaggage::StateSend,		// �͂���
	&CBaggage::StateReturn,		// ����
	&CBaggage::StateReceive,	// receive
	&CBaggage::StateFall,

};

//==========================================================================
// �u���[�\��(�}���`�^�[�Q�b�g�����_�����O)�p�萔��`
//==========================================================================
namespace MULTITARGET
{
	// �J�n��
	const float START_ALPHA = (0.7f);		// �ڕW�����x
	const float START_MULTI = (1.0f);		// �ڕW�{��
	const float START_TIMER = (0.0f);		// �J�ڃ^�C�}�[
}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CBaggage> CBaggage::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CBaggage::CBaggage(int nPriority) : CObjectQuaternion(nPriority)
{
	// �l�̃N���A
	m_type = TYPE::TYPE_CLOTH;		// ���
	m_state = STATE::STATE_NONE;	// ���
	m_fStateTimer = 0.0f;			// ��ԃ^�C�}�[
	m_fWeight = 0.0f;	// �d��
	m_bLand = false;	// ���n����
	m_bEnd = false;		// �I������
	m_bfall = false;
	m_velorot = MyLib::Vector3(0.0f, 0.0f, 0.0f);
	m_baggageInfo = {};
	m_fDeviation = 0.0f;
	m_nMapBlock = 0;	// �}�b�v�̃u���b�N
	m_nLife = 0;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CBaggage::~CBaggage()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CBaggage *CBaggage::Create(TYPE type, SBaggageInfo info)
{
	// �������̊m��
	CBaggage* pObj = DEBUG_NEW CBaggage;

	if (pObj != nullptr)
	{
		// �������
		pObj->m_type = type;
		pObj->m_baggageInfo = info;

		// ����������
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CBaggage::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ����������
	HRESULT hr = CObjectQuaternion::Init(m_baggageInfo.path);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// �p�����[�^�ݒ�
	m_fWeight = 1.8f;
	m_nLife = m_baggageInfo.life;

	CreateCollisionBox();
	CObjectX::SetState(CObjectX::STATE::STATE_EDIT);

	// �X�P�[���[��
	SetScale(0.0f);
	m_state = STATE::STATE_APPEARANCE_WAIT;
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CBaggage::Uninit()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectQuaternion::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CBaggage::Kill()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectQuaternion::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CBaggage::Update()
{
	// ��Ԋ֐�
	UpdateState();

	if (CGame::GetInstance()->GetGameManager()->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH) {
		return;
	}

	// �e�N���X�̍X�V����
	CObjectQuaternion::Update();

#ifdef _DEBUG	// �f�o�b�O���ό`
	DebugTransform();
#endif
}

//==========================================================================
// ����
//==========================================================================
void CBaggage::Controll()
{
	// ���擾
	MyLib::Vector3 posOrigin = GetOriginPosition();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
#ifdef _DEBUG
	ImGui::DragFloat("weight", &m_fWeight, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("PITCH_RATIO", &PITCH_RATIO, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("PITCH_INER", &PITCH_INER, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("ROLL_FSTSPD", &ROLL_FSTSPD, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("ROLL_INER", &ROLL_INER, 0.1f, 0.0f, 0.0f, "%.2f");
#endif

	// �ʒu�X�V
	pos += move;
	pos += m_force;

	// ��](�s�b�`��
	if (move.y > 0.0f) m_velorot.y += fabsf(move.y) * 0.00025f;
	else m_velorot.y += (0.0f - m_velorot.y) * PITCH_INER;

	m_velorot.x += (0.0f - m_velorot.x) * ROLL_INER;

	// �N�H�[�^�j�I�����蓖��
	BindQuaternion(MyLib::Vector3(0.0f, 1.0f, 0.0f), m_velorot.y);

	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 vec = MyLib::Vector3(sinf(rot.y), 0.0f, cosf(rot.y));
	BindQuaternion(MyLib::Vector3(vec.z, 0.0f, -vec.x), m_velorot.x);

	// �d�͉��Z
	move.y -= mylib_const::GRAVITY * m_fWeight;

	static float limitMoveY = 30.0f;
#ifdef _DEBUG
	ImGui::DragFloat("Limit MoveY", &limitMoveY, 1.0f, 0.0f, 0.0f, "%.2f");
#endif

	if (move.y >= limitMoveY)
	{
		move.y = limitMoveY;
	}

	// ���̈ʒu����
	if (pos.y <= GetOriginPosition().y)
	{
		pos.y = GetOriginPosition().y;
		move.y = 0.0f;
		m_bLand = true;
	}
	else
	{
		m_bLand = false;
	}

	// �����␳
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	// �Ԃ�ݒ�
	m_fDeviation += m_baggageInfo.cycle;
	UtilFunc::Transformation::RotNormalize(m_fDeviation);
	pos.x += sinf(m_fDeviation) * m_baggageInfo.width * ((pos.y - GetOriginPosition().y) / 1000.0f);

	// ���ݒ�
	SetPosition(pos);
	SetMove(move);
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CBaggage::UpdateState()
{

	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// �Ȃ�
//==========================================================================
void CBaggage::StateNone()
{
	// ��Q���Ƃ̏Փ˔���
	if (Hit())
	{
		m_velorot.x += ROLL_FSTSPD;	// �Փ˂����烍�[�����ɉ�]���x��^����
		m_nLife--;

		if (m_nLife <= 0)
		{
			// ���S���
			m_state = STATE::STATE_DEAD;

			// ������уX�^�[�g�n�_
			m_posAwayStart = GetPosition();
			SetForce(0.0f);
		}
	}

	m_fStateTimer = 0.0f;

	// ����
	Controll();
}

//==========================================================================
// �_���[�W
//==========================================================================
void CBaggage::StateDamage()
{
	if (StateTime::DAMAGE <= m_fStateTimer)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE::STATE_NONE;
	}

	// ����
	Controll();
}

//==========================================================================
// ���S
//==========================================================================
void CBaggage::StateDead()
{
	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 posV = pCamera->GetPositionVDest();
	MyLib::Vector3 posR = pCamera->GetPositionRDest();
	MyLib::Vector3 ray = (posR - posV).Normal();


	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posDest = posV + (200.0f * ray);

	pos = UtilFunc::Correction::EasingEaseIn(m_posAwayStart, posDest, 0.0f, StateTime::DEAD, m_fStateTimer);

	if (m_fStateTimer >= StateTime::DEAD)
	{
		pos = posDest;
		if (!m_bEnd)
		{
			//// �t�B�[�h�o�b�N�G�t�F�N�gON
			//CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
			//	MULTITARGET::START_ALPHA,
			//	MULTITARGET::START_MULTI,
			//	MULTITARGET::START_TIMER);

			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_CRACK_GRASS);

			// ��ʊ���
			CGlassclush::Create();
		}
		m_bEnd = true;
	}

	SetPosition(pos);
}

//==========================================================================
// �o���҂�
//==========================================================================
void CBaggage::StateAppearance_Wait()
{
	m_fStateTimer = 0.0f;
	SetMove(0.0f);
}

//==========================================================================
// �o��
//==========================================================================
void CBaggage::StateAppearance()
{
	// �g��
	float scale = UtilFunc::Correction::EasingLinear(0.0f, 1.0f, 0.0f, StateTime::APPEARANCE, m_fStateTimer);
	SetScale(scale);

	if (StateTime::APPEARANCE <= m_fStateTimer)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE::STATE_APPEARANCE_WAIT;
		SetScale(1.0f);
	}
}

//==========================================================================
// �p�X
//==========================================================================
void CBaggage::StatePass()
{
	// �������ړ�
	MyLib::Vector3 pos = UtilFunc::Calculation::GetParabola3D(
		m_posAwayStart,
		GetOriginPosition(),
		200.0f,
		m_fStateTimer / StateTime::PASS);

	if (StateTime::PASS <= m_fStateTimer)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE::STATE_APPEARANCE_WAIT;
		pos = GetOriginPosition();

		// �J�E���g�_�E���J�n
		CCountdown_Start::Create();

		// ���t���[���폜
		CBlackFrame::GetInstance()->SetState(CBlackFrame::STATE::STATE_OUT);

		// �J�E���g�_�E���ɑJ��
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_COUNTDOWN);

		// �Q�b�g�G�t�F�N�g����
		CEffekseerObj::Create(
			CMyEffekseer::EFKLABEL::EFKLABEL_BAGGAGEGET,
			pos, MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 90.0f, true);
	}

	SetPosition(pos);
}

//==========================================================================
// �S�[��
//==========================================================================
void CBaggage::StateGoal()
{
	
}

//==========================================================================
// �͂���
//==========================================================================
void CBaggage::StateSend()
{

}

//==========================================================================
// ��������
//==========================================================================
void CBaggage::StateFall()
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();

	move.y -= 0.7f;
	move.z -= 0.2f;
	move.x += 0.5f;

	m_velorot.x += (0.0f - m_velorot.x) * ROLL_INER;

	// �N�H�[�^�j�I�����蓖��
	BindQuaternion(MyLib::Vector3(0.0f, 1.0f, 0.0f), m_velorot.y);

	pos += move;

	SetPosition(pos);
	SetMove(move);

	if (pos.y <= 0)
	{
		if (!m_bfall)
		{// �����Ԃ��̐���

			CEffekseerObj::Create(
				CMyEffekseer::EFKLABEL::EFKLABEL_SPRAYWATER,
				pos, MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 40.0f, true);
		}

		m_bfall = true;
	}
}

//==========================================================================
// ����
//==========================================================================
void CBaggage::StateReturn()
{
	SetMove(0.0f);

	// �J�������擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 posV = pCamera->GetPositionV();
	MyLib::Vector3 posR = pCamera->GetPositionR();
	MyLib::Vector3 ray = (posR - posV).Normal();

	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posDest = posV + (200.0f * ray);

	pos = UtilFunc::Correction::EasingEaseIn(m_posAwayStart, posDest, 0.0f, StateTime::DEAD, m_fStateTimer);

	if (m_fStateTimer >= StateTime::DEAD)
	{
		pos = posDest;
		if (!m_bEnd)
		{
			//// �t�B�[�h�o�b�N�G�t�F�N�gON
			//CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
			//	MULTITARGET::START_ALPHA,
			//	MULTITARGET::START_MULTI,
			//	MULTITARGET::START_TIMER);

			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_CRACK_GRASS);

			CGlassclush::Create();
		}
		m_bEnd = true;
	}

	SetPosition(pos);
}

//==========================================================================
// �f�o�b�O���ό`
//==========================================================================
void CBaggage::DebugTransform()
{

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Transform"))
	{
		float windowWidth = 100.0f;
		const float  POS_MOVE = 0.5f;
		//=============================
		// �ʒu�ݒ�
		//=============================
		MyLib::Vector3 pos = GetPosition();
		ImGui::Text("pos");
		ImGui::SameLine();
		// X
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();
		// Y
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();
		// Z
		ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();

		// �ʒu�ݒ�
		SetPosition(pos);
		SetMove(0.0f);
		ImGui::TreePop();
	}
}

//==========================================================================
// �͒ǉ�
//==========================================================================
void CBaggage::AddForce(const MyLib::Vector3& power, const MyLib::Vector3& ActPos)
{
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	float angle = pos.AngleXY(ActPos);
	m_force.x = (power.x);
	m_force.x = fabsf(m_force.x);

	move.y += power.y;

	SetMove(move);

}

//==========================================================================
// �`�揈��
//==========================================================================
void CBaggage::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// �Q�ƒl�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// �o�b�t�@�ւ̒l�ɑ΂��Ẵ}�X�N�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// �X�e���V���e�X�g�̔�r���@�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	// �e�X�g���ʂɑ΂��Ă̔��f�ݒ�
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z+�X�e���V������
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z+�X�e���V�����s
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Z�e�X�g�̂ݎ��s

	// �X�e���V���`��
	CObjectQuaternion::Draw();

	// �X�e���V���o�b�t�@����
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ���ʂ̕`��
	CObjectQuaternion::Draw();
}

//==========================================================================
// ��Q���Ƃ̔���
//==========================================================================
bool CBaggage::Hit()
{
//#ifndef DEBUG
//	return false;
//#endif // DEBUG

	

	m_nMapBlock = 0;
	float distanceX = GetPosition().x;
	while (1)
	{
		// �Ԋu�����Z
		distanceX -= CMap_Obstacle::GetDistance_CollisionBlock();
		if (distanceX <= 0.0f)
		{
			break;
		}

		// �u���b�N���Z
		m_nMapBlock++;
	}


	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListByBlock(m_nMapBlock);

	// �擪��ۑ�
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	MyLib::Vector3 MyPos = GetPosition();
	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;
		MyLib::Vector3 ObjPos = pObj->GetPosition();

		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();
		for (const auto& collider : info.boxcolliders)
		{
			if (UtilFunc::Collision::IsAABBCollidingWithBox(GetAABB(), GetWorldMtx(), MyLib::AABB(collider.vtxMin, collider.vtxMax), collider.worldmtx))
			{
				// �����擾
				float angle = MyPos.AngleXY(ObjPos);

				bool bDead = UtilFunc::Collision::CollisionRangeAngle(angle, DEADANGLE_HIT + (-D3DX_PI * 0.5f), -DEADANGLE_HIT + (-D3DX_PI * 0.5f));
				
				// �ړ��ʎ擾
				MyLib::Vector3 move = GetMove();

				// ��Q���̃q�b�g����
				pObj->Hit();

				if (bDead)
				{
					ImGui::Text("dead");

					// ���S���
					m_state = STATE::STATE_DEAD;

					MyLib::Vector3 hitmove = (MyPos - ObjPos) * 1.0f;
					move.x += hitmove.x;
					move.z += hitmove.z;

					SetForce(0.0f);

					// ������уX�^�[�g�n�_
					m_posAwayStart = MyPos;
				}
				else
				{
					ImGui::Text("no dead");

					move.y *= -1.0f;

					// �_���[�W
					m_state = STATE::STATE_DAMAGE;

					// �q�b�g�G�t�F�N�g
					CEffekseerObj::Create(
						CMyEffekseer::EFKLABEL::EFKLABEL_IMPACT,
						GetWorldMtx().GetWorldPosition(), MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 30.0f, true);
					m_bHit = true;
				}
				SetMove(move);

				// SE�Đ�
				CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_SHOCK_WOOD);

				return true;
			}
		}
	}

	m_bHit = false;
	return false;
}

//==========================================================================
// ���胊�Z�b�g
//==========================================================================
void CBaggage::Reset()
{
	m_bHit = false;
	m_bEnd = false;
	m_bLand = false;
	m_velorot = MyLib::Vector3(0.0f, 0.0f, 0.0f);
	m_fDeviation = 0.0f;
	m_fStateTimer = 0.0f;
	m_state = STATE::STATE_NONE;
	m_nLife = m_baggageInfo.life;
}

//==========================================================================
// ��Ԑݒ�
//==========================================================================
void CBaggage::SetState(STATE state)
{
	m_state = state;
	m_fStateTimer = 0.0f;
}
