//=============================================================================
// 
//  �S�[���t���O���� [goalflag.cpp]
//  Author : �����V����̃v���O����(���؂肵�Ă܂���)
// 
//=============================================================================
#include "checkpoint.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "spline.h"
#include "course.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\koko.x";
}

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CCheckpoint> CCheckpoint::m_List = {};	// ���X�g
int CCheckpoint::m_nAll = 0;
int CCheckpoint::m_nSaveID = 0;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCheckpoint::CCheckpoint(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_fLength = 0.0f;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCheckpoint::~CCheckpoint()
{

}

//==========================================================================
// ��������
//==========================================================================
CCheckpoint* CCheckpoint::Create(const float length)
{
	// �������̊m��
	CCheckpoint* pObj = DEBUG_NEW CCheckpoint;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();

		// �����ݒ�
		pObj->SetLength(length);

		// �������Z
		m_nAll++;
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCheckpoint::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

	// ID�ݒ�
	m_MyIndex = m_nAll;

	// ����������
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCheckpoint::Uninit()
{
	// �������炷
	if (m_nAll > 0)
	{
		m_nAll--;
	}

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CCheckpoint::Kill()
{
	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CCheckpoint::Update()
{
	// �ʒu���擾
	float playerlen = 0.0f;

	// ���X�g���[�v
	CListManager<CPlayer> PlayerList = CPlayer::GetListObj();
	CPlayer* pObj = nullptr;
	while (PlayerList.ListLoop(&pObj))
	{
		// �v���C���[�̈ʒu���擾
		playerlen = pObj->GetMoveLength();
	}

	if (playerlen >= m_fLength)
	{// �`�F�b�N�|�C���g�ʉ߂�����

		if (m_nSaveID < m_MyIndex)
		{
			m_nSaveID = m_MyIndex;
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCheckpoint::Draw()
{
	// �`��
	CObjectX::Draw();
}

//==========================================================================
// �����ݒ�
//==========================================================================
void CCheckpoint::SetLength(const float length)
{
	// ������ݒ�
	m_fLength = length;

	// ���W��ݒ�
	MyLib::Vector3 pos = MySpline::GetSplinePosition_NonLoop(CGame::GetInstance()->GetCourse()->GetVecPosition(), m_fLength);
	SetPosition(pos);
}
