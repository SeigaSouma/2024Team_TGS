//=============================================================================
// 
//  �^�C�}�[���� [timer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "multinumber.h"
#include "debugproc.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano.png";	// �e�N�X�`���̃t�@�C��

	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(1100.0f, 100.0f, 0.0f);	// �����ʒu
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(30.0f, 30.0f);
	const float DSTANCE_TIMER = SIZE_NUMBER.x * 2.25f;
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================
CTimer *CTimer::m_pTimer = nullptr;	// ���g�̃|�C���^

//==========================================================================
// �֐����X�g
//==========================================================================
CTimer::STATE_FUNC CTimer::m_StateFuncList[] =
{
	&CTimer::StateWait,		// �ҋ@
	&CTimer::StateGoal,		// �S�[��
};

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTimer::CTimer(int nPriority)
{
	// �l�̃N���A
	m_state = STATE_WAIT;		// ���
	m_fStateTime = 0.0f;		// ��Ԏ���
	m_fTime = 0.0f;				// ����
	m_pos = mylib_const::DEFAULT_VECTOR3;	// �ʒu
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;	// ���̈ʒu
	m_bAddTime = false;			// �^�C�}�[���Z�̃t���O
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTimer::~CTimer()
{

}

//==========================================================================
// ��������
//==========================================================================
CTimer* CTimer::Create()
{
	if (m_pTimer != nullptr) return m_pTimer;

	// �������̊m��
	m_pTimer = DEBUG_NEW CTimer;

	if (m_pTimer != nullptr)
	{
		// ����������
		m_pTimer->Init();
	}

	return m_pTimer;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTimer::Init()
{
	// �e��ϐ�������
	m_pos = DEFAULT_POSITION;
	m_posOrigin = m_pos;	// ���̈ʒu
	m_fTime = 0.0f;	// ����
	m_state = STATE_WAIT;			// ���
	m_bAddTime = false;			// �^�C�}�[���Z�̃t���O


	// ���A�b�A�~���b�̌v�Z
	int time[3];
	time[0] = static_cast<int>(m_fTime) / (60);
	time[1] = static_cast<int>(m_fTime) % (60);
	time[2] = static_cast<int>(m_fTime) % 1000;

	for (int i = 0; i < 3; i++)
	{
		// ����
		m_pClearTime[i] = CMultiNumber::Create(
			m_pos,
			SIZE_NUMBER,
			2,
			CNumber::EObjectType::OBJECTTYPE_2D,
			TEXTURE, false, 3);
		if (m_pClearTime[i] == nullptr){
			continue;
		}
		CMultiNumber* pNumber = m_pClearTime[i];

		// �ʒu�ݒ�
		MyLib::Vector3 pos = m_pos;
		pos.x -= DSTANCE_TIMER * i;
		pNumber->SetPosition(pos);

		// �E�񂹂ɐݒ�
		pNumber->SetAlignmentType(CMultiNumber::AlignmentType::ALIGNMENT_RIGHT);

		// �l�̐ݒ�
		pNumber->SetValue(time[i]);
		pNumber->SetType(CObject::TYPE::TYPE_NUMBER);
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTimer::Uninit()
{
	delete m_pTimer;
	m_pTimer = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CTimer::Update()
{
	// ��ԕʏ���
	(this->*(m_StateFuncList[m_state]))();

	if (!m_bAddTime)
	{
		return;
	}

	// �^�C�}�[���Z
	m_fTime += CManager::GetInstance()->GetDeltaTime();

	// �^�C�}�[�𕪁A�b�A�~���b�ɕϊ�
	int time[3];
	time[2] = static_cast<int>(m_fTime / 60);
	time[1] = static_cast<int>(m_fTime) % 60;
	time[0] = static_cast<int>((m_fTime - static_cast<int>(m_fTime)) * 1000);
	time[0] /= 10;

	for (int i = 0; i < 3; i++)
	{
		if (m_pClearTime[i] == nullptr){
			continue;
		}

		// �l�̐ݒ�
		m_pClearTime[i]->SetValue(time[i]);

		// �ʒu�ݒ�
		MyLib::Vector3 pos = m_pos;
		pos.x -= DSTANCE_TIMER * i;
		m_pClearTime[i]->SetPosition(pos);
	}

}

//==========================================================================
// �ҋ@���
//==========================================================================
void CTimer::StateWait()
{
	// ���ԃ��Z�b�g
	m_fStateTime = 0.0f;
}

//==========================================================================
// �S�[�����
//==========================================================================
void CTimer::StateGoal()
{
	m_bAddTime = false;
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTimer::Draw()
{

}
