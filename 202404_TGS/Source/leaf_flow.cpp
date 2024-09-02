//=============================================================================
// 
//  �t���Ϗ��� [leaf_flow.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "leaf_flow.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "game.h"
#include "course.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\effect\\leaf.png";	// �e�N�X�`���̃t�@�C��
}

namespace StateTime	// ��ԕʎ���
{
	const float WAIT = 0.2f;	// �ҋ@
	const float FADEOUT = 0.3f;	// �t�F�[�h�A�E�g
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CLeafFlow::STATE_FUNC CLeafFlow::m_StateFunc[] =
{
	&CLeafFlow::StateFlow,		// ����
	&CLeafFlow::StateFadeOut,	// �t�F�[�h�A�E�g
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CLeafFlow::CLeafFlow(int nPriority) : CLeaf(nPriority)
{
	// �l�̃N���A
	m_fStateTimer = 0.0f;			// ��ԃJ�E���^�[
	m_state = CLeafFlow::State::STATE_FLOW;	// ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CLeafFlow::~CLeafFlow()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CLeafFlow::Init()
{

	// �t���ς̏�����
	CLeaf::Init();

	// ����
	MyLib::Vector3 rot;
	rot.y = UtilFunc::Transformation::Random(-31, 31) * 0.1f;
	rot.x = D3DX_PI * 0.5f;
	SetRotation(rot);

	return S_OK;
}

//==========================================================================
// �X�V����
//==========================================================================
void CLeafFlow::Update()
{
	UpdateState();

	// �X�V����
	CObject3D::Update();
}

//==========================================================================
// ��ԍX�V
//==========================================================================
void CLeafFlow::UpdateState()
{
	// ��ԃ^�C�}�[
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// ��ԍX�V
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// ����
//==========================================================================
void CLeafFlow::StateFlow()
{
	MyLib::Vector3 pos = GetPosition();
	pos.x += 5.0f;
	SetPosition(pos);

	if (pos.x >= 20000.0f)
	{// ���Ԍo��

		m_state = State::STATE_FADEOUT;
		m_fStateTimer = 0.0f;
	}
}

//==========================================================================
// �t�F�[�h�A�E�g
//==========================================================================
void CLeafFlow::StateFadeOut()
{
	float alpha = 1.0f - (m_fStateTimer / StateTime::FADEOUT);
	SetAlpha(alpha);

	if (m_fStateTimer >= StateTime::FADEOUT)
	{// ���Ԍo��

		Uninit();
	}
}
