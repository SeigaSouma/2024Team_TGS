//=============================================================================
// 
//  ����]�[���}�l�[�W�� [judgezoneManager.cpp]
//  Author : �Ό��D�n
// 
//============================================================================
#include "judgezoneManager.h"
#include "judgezone.h"
#include "judge.h"
#include "judgeobj.h"
#include "game.h"
#include "course.h"
#include "texture.h"
#include "object2D.h"
#include "spline.h"
#include "objectBillboard.h"
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
				CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), judge);

				(*itr)->Uninit();
			}

#ifdef _DEBUG
			CCourse* pCource = CGame::GetInstance()->GetCourse();
			MyLib::Vector3 pos;
			float length = pCource->GetCourceLength();

			//�X�^�[�g
			pos = MySpline::GetSplinePosition_NonLoop(pCource->GetVecPosition(), length * (*itr)->GetZone().start);
			pos.y = (*itr)->GetBorder();
			CEffect3D::Create(
				pos,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

			//�I��
			pos = MySpline::GetSplinePosition_NonLoop(pCource->GetVecPosition(), length * (*itr)->GetZone().end);
			pos.y = (*itr)->GetBorder();
			CEffect3D::Create(
				pos,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif // _DEBUG
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
	std::list<CJudgeZone*> removeList;
	std::list<CJudgeZone*>::iterator itr;
	itr = m_zoneList.GetEnd();
	CJudgeZone* pObj = nullptr;
	while (m_zoneList.ListLoop(itr))
	{
		if (!(*itr)->IsEnable())
		{
			removeList.push_back((*itr));
		}
	}
	
	for (itr = removeList.begin(); itr != removeList.end(); itr++)
	{
		m_zoneList.Delete((*itr));
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

//==========================================================================
// ����]�[���ޓǂݍ��ݏ���
//==========================================================================
void CJudgeZoneManager::Load(std::string path)
{
	CJudgeZone* pJudgeZone = nullptr;
	std::string aPath[2];

	// �t�@�C�����J��
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("USEZONE") != std::string::npos)
			{// TYPE�Ŕz�u���̎��

				// �X�g���[���쐬
				std::istringstream lineStream(line);

				// ���n��
				std::string path;
				lineStream >>
					hoge >>
					hoge >>		// ��
					path;		// ���l
				path = UtilFunc::Transformation::ReplaceBackslash(path);

				//����]�[���ǂݍ���
				LoadZone(path);
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// �t�@�C�������
		File.close();
	}
}

//==========================================================================
// ����]�[���ǂݍ��ݏ���
//==========================================================================
void CJudgeZoneManager::LoadZone(std::string path)
{
	CJudgeZone* pJudgeZone = nullptr;
	std::string aPath[2];

	// �t�@�C�����J��
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("ZONESET") != std::string::npos)
			{// MODELSET�Ŕz�u���ǂݍ���

				// �ǂݍ��ݏ��
				CJudgeZone::SZone zone = { 0.0f,1.0f };
				float border = 0.0f;

				while (line.find("END_ZONESET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("START") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							zone.start;	// ���l
					}
					if (line.find("END") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							zone.end;	// ���l
					}
					if (line.find("BORDER") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							border;		// ���l
					}
					else if (line.find("CONDITION_TOP") != std::string::npos)
					{// POS�ňʒu

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							aPath[0];	// �p�X
						aPath[0] = UtilFunc::Transformation::ReplaceBackslash(aPath[0]);
					}
					else if (line.find("CONDITION_UNDER") != std::string::npos)
					{// ROT�Ō���

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							aPath[1];	// �p�X
						aPath[1] = UtilFunc::Transformation::ReplaceBackslash(aPath[1]);
					}
				}

				// ��荞��
				if (pJudgeZone == nullptr)
				{
					pJudgeZone = CJudgeZone::Create(zone.start, zone.end, border);
				}
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// �t�@�C�������
		File.close();
	}

	// �����ǂݍ���
	if (pJudgeZone != nullptr)
	{
		pJudgeZone->SetInfo(CJudge::BORDER::UP, LoadCondition(aPath[CJudge::BORDER::UP]));
		pJudgeZone->SetInfo(CJudge::BORDER::DOWN, LoadCondition(aPath[CJudge::BORDER::DOWN]));
		m_zoneList.Regist(pJudgeZone);
	}
}

//==========================================================================
// �����ǂݍ��ݏ���
//==========================================================================
CJudge::SJudgeInfo CJudgeZoneManager::LoadCondition(std::string path)
{
	CJudge::SJudgeInfo info;
	info.type = CJudge::JUDGETYPE::TYPE_NONE;
	info.judgeParam = 
	{
		{CJudge::JUDGE::JUDGE_AAA,-1},
		{CJudge::JUDGE::JUDGE_BBB,-1},
		{CJudge::JUDGE::JUDGE_CCC,-1},
		{CJudge::JUDGE::JUDGE_DDD,-1},
	};

	// �t�@�C�����J��
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("CONDITIONSET") != std::string::npos)
			{// MODELSET�Ŕz�u���ǂݍ���
				while (line.find("END_CONDITIONSET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("TYPE") != std::string::npos)
					{// TYPE�Ŕz�u���̎��
						int num = 0;
						
						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							num;		// ���l

						info.type = static_cast<CJudge::JUDGETYPE>(num);
					}
					if (line.find("JUDGE_AAA") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.judgeParam[CJudge::JUDGE::JUDGE_AAA];	// ���l
					}
					if (line.find("JUDGE_BBB") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.judgeParam[CJudge::JUDGE::JUDGE_BBB];	// ���l
					}
					if (line.find("JUDGE_CCC") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.judgeParam[CJudge::JUDGE::JUDGE_CCC];	// ���l
					}
					if (line.find("JUDGE_DDD") != std::string::npos)
					{// TYPE�Ŕz�u���̎��

						// �X�g���[���쐬
						std::istringstream lineStream(line);

						// ���n��
						lineStream >>
							hoge >>
							hoge >>		// ��
							info.judgeParam[CJudge::JUDGE::JUDGE_DDD];	// ���l
					}
				}
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// �t�@�C�������
		File.close();
	}
	return info;
}
