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

	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(880.0f, 237.0f, 0.0f);	// �����ʒu
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(50.0f, 50.0f);
	const float DSTANCE_TIMER = SIZE_NUMBER.x * 2;
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
	&CTimer::StateWait,			// �ҋ@
	&CTimer::StatAppearance,	// �o��
	&CTimer::StatAddLittle,		// �������Z
	&CTimer::StateAdjustment,	// ����
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
CTimer *CTimer::Create()
{
	// �����p�̃I�u�W�F�N�g
	CTimer *pScore = nullptr;

	if (pScore == nullptr)
	{// nullptr��������

		// �������̊m��
		pScore = DEBUG_NEW CTimer;

		if (pScore != nullptr)
		{// �������̊m�ۂ��o���Ă�����

			// ����������
			pScore->Init();
		}

		return pScore;
	}

	return nullptr;
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
	m_bAddTime = true;			// �^�C�}�[���Z�̃t���O


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
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTimer::Uninit()
{
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
// �o�����
//==========================================================================
void CTimer::StatAppearance()
{
	// ���ԉ��Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//if (m_fStateTime >= TIME_APPEARANCE)
	//{
	//	// ��ԑJ��
	//	m_state = STATE_ADD_LITTLE;
	//	m_fStateTime = 0.0f;

	//	// �^�C�}�[��i�߂�
	//	m_bAddTime = true;
	//}
}

//==========================================================================
// �������Z���
//==========================================================================
void CTimer::StatAddLittle()
{
	// ���ԉ��Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//if (m_fStateTime >= TIME_ADDLITTLE)
	//{
	//	// ��ԑJ��
	//	m_state = STATE_ADJ;
	//	m_fStateTime = 0.0f;
	//	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SCENE_MAIN);
	//}
}

//==========================================================================
// �������
//==========================================================================
void CTimer::StateAdjustment()
{
	// ���ԉ��Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	/*m_pos.x = UtilFunc::Correction::EasingLinear(m_posOrigin.x, DEST_POSITION.x, m_fStateTime / TIME_ADJUSTMENT);
	m_pos.y = UtilFunc::Correction::EasingLinear(m_posOrigin.y, DEST_POSITION.y, m_fStateTime / TIME_ADJUSTMENT);*/

	MyLib::Vector3 setpos = m_pos;

	//if (m_fStateTime >= TIME_ADJUSTMENT)
	//{
	//	// ��ԑJ��
	//	m_state = STATE_WAIT;
	//	m_fStateTime = 0.0f;
	//}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CTimer::Draw()
{

}
