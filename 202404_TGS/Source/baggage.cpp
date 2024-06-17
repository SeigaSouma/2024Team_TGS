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
#include "map_obstacle.h"

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
	m_type = TYPE::TYPE_CLOTH;	// ���
	m_fWeight = 0.0f;	// �d��
	m_bLand = false;	// ���n����
	m_velorot = MyLib::Vector3(0.0f, 0.0f, 0.0f);
	m_baggageInfo = {};
	m_fDeviation = 0.0f;
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

	CreateCollisionBox();
	SetState(CObjectX::STATE::STATE_EDIT);

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
	if (CGame::GetInstance()->GetGameManager()->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH) {
		return;
	}

	CObjectQuaternion::Update();

	// ��Q���Ƃ̏Փ˔���
	if(Hit()) m_velorot.x += ROLL_FSTSPD;	// �Փ˂����烍�[�����ɉ�]���x��^����

	// ���擾
	MyLib::Vector3 posOrigin = GetOriginPosition();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	ImGui::DragFloat("weight", &m_fWeight, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("PITCH_RATIO", &PITCH_RATIO, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("PITCH_INER", &PITCH_INER, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("ROLL_FSTSPD", &ROLL_FSTSPD, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("ROLL_INER", &ROLL_INER, 0.1f, 0.0f, 0.0f, "%.2f");

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
	ImGui::DragFloat("Limit MoveY", &limitMoveY, 1.0f, 0.0f, 0.0f, "%.2f");

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
	// ��Q���̃��X�g�擾
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

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
				MyLib::Vector3 move = GetMove();
				move.y *= -1.0f;
				SetMove(move);

				/*CMyEffekseer::GetInstance()->SetEffect(
					CMyEffekseer::EFKLABEL::EFKLABEL_IMPACT,
					GetWorldMtx().GetWorldPosition(), MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 30.0f, true);*/
				m_bHit = true;
				return true;
			}
		}
	}

	m_bHit = false;
	return false;
}