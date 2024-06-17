//=============================================================================
// 
//  ����]�[���}�l�[�W�� [judgezoneManager.cpp]
//  Author : �Ό��D�n
// 
//============================================================================
#include "judgezoneManager.h"
#include "judgezone.h"
#include "judge.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	
}
CJudgeZoneManager* CJudgeZoneManager::m_ThisPtr = nullptr;

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CJudgeZoneManager::CJudgeZoneManager()
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CJudgeZoneManager::~CJudgeZoneManager()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CJudgeZoneManager *CJudgeZoneManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// �������̊m��
		m_ThisPtr = DEBUG_NEW CJudgeZoneManager;

		if (m_ThisPtr != nullptr)
		{
			// ����������
			m_ThisPtr->Init();
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CJudgeZoneManager::Init()
{
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CJudgeZoneManager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// ����m�F����
//==========================================================================
void CJudgeZoneManager::Check(float progress)
{
	CListManager<CJudgeZone>::Iterator itr = m_zoneList.GetEnd();
	while (m_zoneList.ListLoop(itr))
	{
		if ((*itr)->IsEnable())
		{
			CJudgeZone::SZone zone = (*itr)->GetZone();
			if (progress >= zone.start && progress <= zone.end)
			{//�͈͓�
				(*itr)->Check();
			}
			else if (progress > zone.end)
			{//�I���i����j
				CJudge::JUDGE judge = (*itr)->Judge();	//�����ɔ��肪�����Ă�
				(*itr)->Uninit();
			}
		}
	}

	// �����������]�[���j��
	Release();
}

//==========================================================================
// �����]�[���j������
//==========================================================================
void CJudgeZoneManager::Release()
{
	CJudgeZone* pZone = nullptr;
	while (m_zoneList.ListLoop(&pZone))
	{
		if (!pZone->IsEnable())
		{// �����̃]�[���̂ݔj��
			m_zoneList.Delete(pZone);	// ���X�g���O
			pZone->Uninit();			// �I��
			delete pZone;				// �j��
		}
	}
}

//==========================================================================
// �S�]�[���j������
//==========================================================================
void CJudgeZoneManager::ReleaseAll()
{
	CJudgeZone* pZone = nullptr;
	while (m_zoneList.ListLoop(&pZone))
	{
		m_zoneList.Delete(pZone);	// ���X�g���O
		pZone->Uninit();			// �I��
		delete pZone;				// �j��
	}
}
