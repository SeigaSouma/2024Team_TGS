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
#include "timer.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\checkpoint\\flag.x";
	const float ROTATE_TIMER = (1.0f / 30.0f);
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
	m_fPassedTime = 0.0f;
	m_bIsPassed = false;
	m_fRotateTime = 0.0f;
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
	// �ʉߍς݂Ȃ珈�����Ȃ�
	if (m_bIsPassed)
	{
		// ��]���ԉ��Z
		m_fRotateTime += CManager::GetInstance()->GetDeltaTime();

		MyLib::Vector3 rot = GetRotation();
		rot.z = UtilFunc::Correction::EasingEaseIn(0.0f, -D3DX_PI, 0.0f, 0.75f, m_fRotateTime);
		SetRotation(rot);
		return;
	}

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
			// ID�ۑ�
			m_nSaveID = m_MyIndex;

			// �ʉ߂������Ԃ�ۑ�
			m_fRotateTime = 0.0f;
			m_fPassedTime = CTimer::GetInstance()->GetTime();
			m_bIsPassed = true;

			// SE�Đ�
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_KARAKURI);
		}
	}
}

//==========================================================================
// �`�揈��
//==========================================================================
void CCheckpoint::Draw()
{
	// �{�b�N�X�R���C�_�[�̕`��
	if (!CManager::IsDisp_CheckPoint()) return;

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

//==========================================================================
// �`�F�b�N�|�C���g�t�@�C���ǂݍ���
//==========================================================================
void CCheckpoint::Load(const std::string filename)
{
	// �t�@�C�����J��
	std::ifstream File(filename);
	if (!File.is_open()) {
		return;
	}

	float length = 0.0f;

	// �R�����g�p
	std::string hoge;

	// �f�[�^�ǂݍ���
	std::string line;
	while (std::getline(File, line))
	{
		// �R�����g�̓X�L�b�v
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		if (line.find("SET_LENGTH") != std::string::npos)
		{// TYPE�Ŕz�u���̎��

			// �X�g���[���쐬
			std::istringstream lineStream(line);

			// ���n��
			lineStream >>
				hoge >>
				hoge >>	// ��
				length;	// �z�u���̎��

			CCheckpoint::Create(length);

			continue;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// �t�@�C�������
	File.close();
}