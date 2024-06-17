//=============================================================================
// 
//  ����]�[���}�l�[�W�� [judgezoneManager.cpp]
//  Author : �Ό��D�n
// 
//============================================================================
#include "judgezoneManager.h"
#include "judgezone.h"
#include "judge.h"
#include "texture.h"
#include "object2D.h"
#include <map>

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	std::map<CJudge::JUDGE, std::string> TEXTURE =
	{
		{CJudge::JUDGE::JUDGE_AAA,"data\\TEXTURE\\judge_test_01.png"},
		{CJudge::JUDGE::JUDGE_BBB,"data\\TEXTURE\\judge_test_02.png"},
		{CJudge::JUDGE::JUDGE_CCC,"data\\TEXTURE\\judge_test_03.png"},
		{CJudge::JUDGE::JUDGE_DDD,"data\\TEXTURE\\judge_test_04.png"},
	};
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
				CObject2D* pObj = CObject2D::Create();
				pObj->SetSize(D3DXVECTOR2(128.0f, 36.0f));
				pObj->SetPosition(MyLib::Vector3(400.0f, 100.0f, 0.0f));
				int nTexIdx = CTexture::GetInstance()->Regist(TEXTURE[judge]);
				pObj->BindTexture(nTexIdx);
				pObj->SetVtx();

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
