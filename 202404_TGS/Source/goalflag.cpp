//=============================================================================
// 
//  �S�[���t���O���� [goalflag.cpp]
//  Author : �����V����̃v���O����(���؂肵�Ă܂���)
// 
//=============================================================================
#include "goalflag.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "stagecleartext.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\box.x";
}

//==========================================================================
// �֐��|�C���^
//==========================================================================
CGoalflagX::GOALFLAG_FUNC CGoalflagX::m_GoalflagFuncList[] =
{
	& CGoalflagX::SampleWho,	// �t�[
	& CGoalflagX::SampleWao,	// ���I
};

//==========================================================================
// �ÓI�����o�ϐ�
//==========================================================================
CListManager<CGoalflagX> CGoalflagX::m_List = {};	// ���X�g

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CGoalflagX::CGoalflagX(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	m_fStateTime = 0.0f;	// ��ԃJ�E���^�[
	m_state = SAMPLE_WAO;	// ���
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CGoalflagX::~CGoalflagX()
{

}

//==========================================================================
// ��������
//==========================================================================
CGoalflagX* CGoalflagX::Create(MyLib::Vector3 pos)
{
	// �������̊m��
	CGoalflagX* pObj = DEBUG_NEW CGoalflagX;

	if (pObj != nullptr)
	{
		// ����������
		pObj->Init();

		// �ʒu��񂹂��Ă���
		pObj->SetPosition(pos);
	}

	return pObj;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CGoalflagX::Init()
{
	// ���X�g�ɒǉ�
	m_List.Regist(this);

	// ��ނ̐ݒ�
	CObject::SetType(TYPE_OBJECTX);

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
void CGoalflagX::Uninit()
{

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �폜
//==========================================================================
void CGoalflagX::Kill()
{

	// ���X�g����폜
	m_List.Delete(this);

	// �I������
	CObjectX::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CGoalflagX::Update()
{
	// ��ԃJ�E���^�[���Z
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// ��ԕʏ���
	(this->*(m_GoalflagFuncList[m_state]))();

	// �ʒu���擾
	MyLib::Vector3 pos = CObjectX::GetPosition();
	MyLib::Vector3 Playerpos;

	// ���X�g���[�v
	CListManager<CPlayer> PlayerList = CPlayer::GetListObj();
	CPlayer* pObj = nullptr;
	while (PlayerList.ListLoop(&pObj))
	{
		// �v���C���[�̈ʒu���擾
		Playerpos = pObj->GetPosition();
	}

	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_MAINCLEAR &&
		Playerpos.x >= pos.x)
	{// �S�[����������

		// �K�v�ȃQ�[���̏�Ԑݒ肵�Ă�
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAINCLEAR);
		CStageClearText::Create(MyLib::Vector3(640.0f, 400.0f, 0.0f));
	}
}

//==========================================================================
// �t�[
//==========================================================================
void CGoalflagX::SampleWho()
{

}

//==========================================================================
// ���I
//==========================================================================
void CGoalflagX::SampleWao()
{

}

//==========================================================================
// �`�揈��
//==========================================================================
void CGoalflagX::Draw()
{
	// �`��
	CObjectX::Draw();
}

