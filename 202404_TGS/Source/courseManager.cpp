//=============================================================================
// 
//  �R�[�X�}�l�[�W������ [courseManager.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "courseManager.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"

#include "course.h"
#include "waterfield.h"
#include "stonewall.h"
#include "stonewall_front.h"
#include "map_block.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\map\\chunkdata.bin";
	const int NUM_CHUNK = 5;	// �`�����N�̐�

	const std::vector<MyLib::Vector3> DEFAULT_SEGMENTPOS =
	{
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 2500.0f, 0.0f, 0.0f },
		{ 5000.0f, 0.0f, 0.0f },
		{ 7500.0f, 0.0f, 0.0f },
		{ 9000.0f, 0.0f, 0.0f },
		{ 9000.0f, 0.0f, 0.0f },
	};
	const float DISTANCE_TO_CHUNCK = 3000.0f;	// �`�����N���m�̊Ԋu
}
CCourseManager* CCourseManager::m_ThisPtr = nullptr;	// ���g�̃|�C���^
const float CCourseManager::m_fBlockLength = 9000.0f;	// �u���b�N�̒���

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCourseManager::CCourseManager()
{

}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCourseManager::~CCourseManager()
{
	
}

//==========================================================================
// ��������
//==========================================================================
CCourseManager *CCourseManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// �������̊m��
		m_ThisPtr = DEBUG_NEW CCourseManager;

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
HRESULT CCourseManager::Init()
{

	// �ǂݍ���
	Load();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCourseManager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �폜
//==========================================================================
void CCourseManager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// �X�V����
//==========================================================================
void CCourseManager::Update()
{
	

}

//==========================================================================
// �Z�[�u
//==========================================================================
void CCourseManager::Save()
{
	// �t�@�C�����J��
	std::ofstream File(FILENAME, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	std::vector<std::vector<MyLib::Vector3>> savedata = m_vecAllSegmentPos;

	// ���ꂼ��̃u���b�N�̍ŏ��ƍŌ����
	for (auto& data : savedata)
	{
		data.erase(data.begin());
		data.pop_back();

		// �f�[�^���o�C�i���t�@�C���ɏ����o��
		File.write(reinterpret_cast<char*>(data.data()), data.size() * sizeof(MyLib::Vector3));
	}

	// �t�@�C�������
	File.close();
}

//==========================================================================
// ���[�h
//==========================================================================
void CCourseManager::Load()
{
	
	//=============================
	// �S�u���b�N���(�R�[�X)��ǂݍ��ށB��_�̈ʒu
	//=============================
	// �t�@�C�����J��
	std::ifstream File(FILENAME, std::ios::binary);
	if (!File.is_open()) {
		// ��O����

		m_vecAllSegmentPos.emplace_back();

		m_vecAllSegmentPos[0] = DEFAULT_SEGMENTPOS;
		m_vecAllSegmentPos[0].insert(m_vecAllSegmentPos[0].begin(), MyLib::Vector3(0.0f, 0.0f, 0.0f));
		m_vecAllSegmentPos[0].push_back(DEFAULT_SEGMENTPOS.back());

		Save();
		return;
	}

	// �\���̂̃T�C�Y���擾
	std::streamsize structSize = sizeof(MyLib::Vector3);

	// �t�@�C���̖����܂Ńf�[�^��ǂݍ���
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);

	// �f�[�^�̌����v�Z
	size_t numVectors = fileSize / structSize;

	// �x�N�g���̔z���p��
	m_vecAllSegmentPos.clear();
	m_vecAllSegmentPos.emplace_back(std::vector<MyLib::Vector3>(numVectors));


	// �t�@�C������f�[�^��ǂݍ���
	File.read(reinterpret_cast<char*>(m_vecAllSegmentPos[0].data()), fileSize);

	// �t�@�C�������
	File.close();



	//=============================
	// �����_���I�o
	//=============================
	int segmentSize = static_cast<int>(m_vecAllSegmentPos.size()) - 1;
	
	std::vector<int> randIdx;
	for (int i = 0; i < NUM_CHUNK; i++)
	{
		randIdx.push_back(UtilFunc::Transformation::Random(0, segmentSize));
	}

	// ��{�̃R�[�X�ɂ���
	std::vector<MyLib::Vector3> segmentpos;	// ��_�̈ʒu
	MyLib::Vector3 start;
	std::vector<MyLib::Vector3> vecstart;	// ��_�̈ʒu

	for (const auto& idx : randIdx)
	{
		for (const auto& pos : m_vecAllSegmentPos[idx])
		{
			segmentpos.push_back(start + pos);
		}
		start = segmentpos.back();
		vecstart.push_back(start);
	}

	//=============================
	// �R�[�X�쐬
	//=============================
	CCourse* pCourse = CCourse::Create("data\\TEXT\\map\\course.bin");
	pCourse->SetVecPosition(segmentpos);
	pCourse->Reset();
	CGame::GetInstance()->SetCource(pCourse);

	// �����_���I�o���ꂽ�u���b�N�ɕt������A�`�F�b�N�|�C���g�A��Q���̐���
	// Block�̓ǂݍ���(��Q���A�`�F�b�N�|�C���g)
	CMapBlock::Load();

	// �����ɂ��킹���z�u���s��
	for (int i = 0; i < NUM_CHUNK; i++)
	{
		CMapBlock* pBlock = new CMapBlock;

		if (pBlock != nullptr)
		{
			pBlock->Set(randIdx[i], vecstart[i], CCourseManager::GetBlockLength() * i);
		}
	}


	//���̒��ŏ�Q���A�`�F�b�N�|�C���g


	//=============================
	// �Ί_(��)
	//=============================
	CStoneWall* pStoneWall = CStoneWall::Create();

	// ��_�n�_�ݒ�
	pStoneWall->SetVecPosition(pCourse->GetVecPosition());
	pStoneWall->Reset();

	std::vector<CCourse::VtxInfo> vtxInfo = pCourse->GetVecVtxinfo();
	std::vector<MyLib::Vector3> vecpos;

	MyLib::Vector3 setpos;
	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * -600.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * -600.0f;
		vecpos.push_back(setpos);
	}

	// �e���_���W
	pStoneWall->SetVecVtxPosition(vecpos);
	pStoneWall->BindVtxPosition();


	//=============================
	// �Ί_(��O)
	//=============================
	CStoneWall* pStoneWall_Front = CStoneWall_Front::Create();

	// ��_�n�_�ݒ�
	pStoneWall_Front->SetVecPosition(pCourse->GetVecPosition());
	pStoneWall_Front->Reset();

	vtxInfo = pCourse->GetVecVtxinfo();
	vecpos.clear();

	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * 800.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * 800.0f;
		vecpos.push_back(setpos);
	}

	// �e���_���W
	pStoneWall_Front->SetVecVtxPosition(vecpos);
	pStoneWall_Front->BindVtxPosition();


	//=============================
	// ���˂�̊X�t�B�[���h
	//=============================
	CMapMesh* pTownField = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS);
	pTownField->SetVecPosition(pCourse->GetVecPosition());
	pTownField->Reset();

	// �Ί_�̒���ɒ��_�����낦��
	pTownField->SetVecVtxPosition(pStoneWall->GetVecTopPosition());
	pTownField->BindVtxPosition();


	//=============================
	// ���˂�̊X�t�B�[���h(��O)
	//=============================
	CMapMesh* pTownFieldFront = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS_FRONT);
	pTownFieldFront->SetVecPosition(pCourse->GetVecPosition());
	pTownFieldFront->Reset();

	// �Ί_�̒���ɒ��_�����낦��
	pTownFieldFront->SetVecVtxPosition(pStoneWall_Front->GetVecTopPosition());
	pTownFieldFront->BindVtxPosition();

}

//==========================================================================
// ��_�ǉ�
//==========================================================================
void CCourseManager::AddSegmentPos()
{
	m_vecAllSegmentPos.emplace_back();
	m_vecAllSegmentPos.back() = DEFAULT_SEGMENTPOS;
}

//==========================================================================
// ��_�폜
//==========================================================================
void CCourseManager::SubSegmentPos()
{
	if (static_cast<int>(m_vecAllSegmentPos.size()) < 1) return;
	m_vecAllSegmentPos.pop_back();
}

//==========================================================================
// ��_�폜
//==========================================================================
void CCourseManager::SubSegmentPos(int idx)
{
	if (static_cast<int>(m_vecAllSegmentPos.size()) < 1) return;

	if (static_cast<int>(m_vecAllSegmentPos.size()) <= idx) return;

	std::vector<std::vector<MyLib::Vector3>>::iterator itr = m_vecAllSegmentPos.begin();
	std::advance(itr, idx);
	m_vecAllSegmentPos.erase(itr);

}
