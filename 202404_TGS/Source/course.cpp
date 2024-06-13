//=============================================================================
// 
//  �N������ [course.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "course.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int WIDTH_BLOCK = 2;
	const float WIDTH = 200.0f;
	const float CREATEDISTANCE = 50.0f;	// �����Ԋu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCourse::CCourse(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_pCollisionLineBox.clear();	// �����蔻��{�b�N�X
	m_vecSegmentPosition.clear();	// ��_�̈ʒu
	m_vecVtxPosition.clear();		// �e���_�̈ʒu
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CCourse::~CCourse()
{

}

//==========================================================================
// ��������
//==========================================================================
CCourse* CCourse::Create(const std::string& file)
{
	// �������̊m��
	CCourse* pObjMeshField = DEBUG_NEW CCourse;

	if (pObjMeshField != nullptr)
	{
		pObjMeshField->m_FileName = file;

		// ����������
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CCourse::Init()
{
	HRESULT hr;

	// ���[�h����
	Load(m_FileName);

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist("");
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �I�u�W�F�N�g3D���b�V���̏���������
	Reset();

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCourse::Uninit()
{
	m_pCollisionLineBox.clear();

	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �e���_�v�Z
//==========================================================================
void CCourse::CalVtxPosition()
{
	
	// �ŏ��ƍŌ�A�t�����ɏ����o��
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// �ŏ�
	angle = m_vecSegmentPosition[1].AngleXZ(m_vecSegmentPosition[0]);
	begin = MyLib::Vector3(
		m_vecSegmentPosition[0].x + sinf(angle) * -10.0f,
		m_vecSegmentPosition[0].y,
		m_vecSegmentPosition[0].z + cosf(angle) * -10.0f);

	// �Ō�
	int endIdx = m_vecSegmentPosition.size() - 1;
	angle = m_vecSegmentPosition[endIdx].AngleXZ(m_vecSegmentPosition[endIdx - 1]);
	end = MyLib::Vector3(
		m_vecSegmentPosition[endIdx].x + sinf(angle) * 10.0f,
		m_vecSegmentPosition[endIdx].y,
		m_vecSegmentPosition[endIdx].z + cosf(angle) * 10.0f);

	m_vecSegmentPosition.insert(m_vecSegmentPosition.begin(), begin);
	m_vecSegmentPosition.push_back(end);

	// �Z�O�����g�̒������v�Z
	int segmentSize = m_vecSegmentPosition.size();
	std::vector<float> vecLength(segmentSize);

	for (int i = 0; i < segmentSize; ++i)
	{
		// ����̃C���f�b�N�X(���[�v)
		int next = (i + 1) % segmentSize;

		if (next == 0)
		{
			vecLength[i] = 10.0f;
			break;
		}

		// �_���m�̋���
		vecLength[i] = m_vecSegmentPosition[i].Distance(m_vecSegmentPosition[next]);
	}


	// ���_���N���A
	m_vecVtxPosition.clear();

	// �e���_�i�[
	float toataldistance = 0.0f;
	for (int i = 0; i < segmentSize; i++)
	{
		float distance = 0.0f;

		while (1)
		{
			distance += CREATEDISTANCE;

			if (distance >= vecLength[i])
			{
				toataldistance += CREATEDISTANCE - (distance - vecLength[i]);

				distance = vecLength[i];

				m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, toataldistance, 20.0f));
				break;
			}

			toataldistance += CREATEDISTANCE;
			m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, toataldistance));
		}
	}
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CCourse::Reset()
{
	// �I������
	CObject3DMesh::Uninit();

	// ���S�t���O�������Z�b�g
	SetEnableDeath(false);




	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist("");
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// �e���_�v�Z
	CalVtxPosition();

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 100.0f, 0.0f));				// �ʒu
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_vecVtxPosition.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();


	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	MyLib::AABB aabb(-25.0f, 25.0f);
	for (const auto& vtx : m_vecSegmentPosition)
	{
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CCourse::Update()
{
	// ���_���W�v�Z
	SetVtxPosition();

	SetVtx();
}

//==========================================================================
// ���_���W
//==========================================================================
void CCourse::SetVtxPosition()
{
	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rot;

	for (int y = 0; y < static_cast<int>(m_vecVtxPosition.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �������f
		int next = (y + 1) % static_cast<int>(m_vecVtxPosition.size());

		rot.y = m_vecVtxPosition[next].AngleXZ(m_vecVtxPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		ImGui::Text("x:%f y:%f z:%f, rot.y:%f", m_vecVtxPosition[y].x, m_vecVtxPosition[y].y, m_vecVtxPosition[y].z, rot.y);

		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecVtxPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);


		

		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		//mtxLeft.Multiply(mtxLeft, mtxRotate);


		// ���_���W���
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

		CEffect3D::Create(
			m_vecVtxPosition[y] + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			pVtxPos[idx] + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			pVtxPos[nextidx] + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	}

	int i = 0;
	MyLib::Vector3 fieldpos = GetPosition();
	for (const auto& vtx : m_vecSegmentPosition)
	{
		MyLib::Vector3 setpos = vtx + fieldpos;
		m_pCollisionLineBox[i]->SetPosition(setpos);
		i++;
	}

}

//==========================================================================
// �`�揈��
//==========================================================================
void CCourse::Draw()
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (CManager::GetInstance()->IsWireframe())
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ���C���[�t���[�����[�h
	}

	// �`�揈��
	CObject3DMesh::Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// ���߂郂�[�h
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CCourse::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3 *pVtxNor = GetVtxNor();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	int heightBlock = static_cast<int>(m_vecVtxPosition.size());

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// �c�̕������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
		{// ���̕������J��Ԃ�

			// ����̒��_
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (WIDTH_BLOCK));
			int nVerTexW = (WIDTH_BLOCK) + 1;

			int nLeft = nCntWidth + (nCntHeight * (WIDTH_BLOCK));
			int nRight = nCntWidth + (nCntHeight * (WIDTH_BLOCK)) + nVerTexW;

			if (nNowPoint >= vtxNum)
			{
				continue;
			}

			if (nRight >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxRight = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
			}
			else
			{
				VtxRight = pVtxPos[nRight];
			}

			if (nLeft >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxLeft = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
			}
			else
			{
				VtxLeft = pVtxPos[nLeft];
			}

			if (nNowPoint >= vtxNum)
			{// ���_����������

				// ���_���W�̐ݒ�
				VtxNow = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
			}
			else
			{
				VtxNow = pVtxPos[nNowPoint];
			}

			// �x�N�g�����v�Z
			vec1 = VtxRight - VtxNow;
			vec2 = VtxLeft - VtxNow;

			// �O�ς����߂�
			D3DXVec3Cross(&nor, &vec1, &vec2);

			// �O�ς̐��K�������Ė@���ɂ���
			D3DXVec3Normalize(&nor, &nor);

			// �@��
			pVtxNor[nNowPoint] = nor;
		}
	}

	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ���[�h����
//==========================================================================
HRESULT CCourse::Load(const std::string& file)
{
	// �t�@�C�����J��
	std::ifstream File(file, std::ios::binary);
	if (!File.is_open()) {

		// ��O����
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 0.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 500.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1000.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1800.0f });

		MyLib::AABB aabb(-25.0f, 25.0f);

		for (int i = 0; i < static_cast<int>(m_vecSegmentPosition.size()); i++)
		{
			m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		}

		Save();
		return E_FAIL;
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
	m_vecSegmentPosition.resize(numVectors);
	
	// �t�@�C������f�[�^��ǂݍ���
	File.read(reinterpret_cast<char*>(m_vecSegmentPosition.data()), fileSize);

	// �t�@�C�������
	File.close();

	return S_OK;
}

//==========================================================================
// �Z�[�u����
//==========================================================================
void CCourse::Save()
{
	// �t�@�C�����J��
	std::ofstream File(m_FileName, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// �f�[�^���o�C�i���t�@�C���ɏ����o��
	File.write(reinterpret_cast<char*>(m_vecSegmentPosition.data()), m_vecSegmentPosition.size() * sizeof(MyLib::Vector3));
		
	// �t�@�C�������
	File.close();
}

//==========================================================================
// �����蔻��{�b�N�X�擾
//==========================================================================
CCollisionLine_Box* CCourse::GetCollisionLineBox(int idx)
{
	if (static_cast<int>(m_pCollisionLineBox.size()) <= idx) return nullptr;

	return m_pCollisionLineBox[idx];
}

//==========================================================================
// ��_�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CCourse::GetVecPosition(int idx)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecSegmentPosition[idx];
}

//==========================================================================
// ��_�̈ʒu�ݒ�
//==========================================================================
void CCourse::SetVecPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return;

	m_vecSegmentPosition[idx] = pos;
}

//==========================================================================
// �e���_�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CCourse::GetVecVtxPosition(int idx)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecVtxPosition[idx];
}

//==========================================================================
// �e���_�̈ʒu�ݒ�
//==========================================================================
void CCourse::SetVecVtxPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return;

	m_vecVtxPosition[idx] = pos;
}
