//=============================================================================
// 
//  �Ί_���� [stonewall.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "stonewall.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"
#include "course.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\map_object\\ishigaki000.png";
	const int WIDTH_BLOCK = 2;
	const float WIDTH = 200.0f;		// �΂߂̕�
	const float HEIGHT = 500.0f;	// ����
	const float INTERVAL_TEXU = 500.0f;	// U���W�̊Ԋu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CStoneWall::CStoneWall(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_vecSegmentPosition.clear();	// ��_�̈ʒu
	m_vecVtxPosition.clear();		// �e���_�̈ʒu
	m_courceLength = 0.0f;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CStoneWall::~CStoneWall()
{

}

//==========================================================================
// ��������
//==========================================================================
CStoneWall* CStoneWall::Create()
{
	// �������̊m��
	CStoneWall* pObjMeshField = DEBUG_NEW CStoneWall;

	if (pObjMeshField != nullptr)
	{
		// ����������
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CStoneWall::Init()
{
	HRESULT hr;

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// ���_���W���蓖��
	BindVtxPosition();
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CStoneWall::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �e���_�v�Z
//==========================================================================
void CStoneWall::CalVtxPosition()
{

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
	m_courceLength = 0.0f;
	for (int i = 0; i < segmentSize; i++)
	{
		float distance = 0.0f;

		while (1)
		{
			distance += CCourse::GetCreateDistance();

			if (distance >= vecLength[i])
			{
				m_courceLength += CCourse::GetCreateDistance() - (distance - vecLength[i]);

				distance = vecLength[i];

				m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, m_courceLength, 20.0f));
				break;
			}

			m_courceLength += CCourse::GetCreateDistance();
			m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, m_courceLength));
		}
	}
}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CStoneWall::Reset()
{
	// �I������
	CObject3DMesh::Uninit();

	// ���S�t���O�������Z�b�g
	SetEnableDeath(false);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �e���_�v�Z
	CalVtxPosition();

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 10.0f, 0.0f));				// �ʒu
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_vecVtxPosition.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();

	// ���_���W���蓖��
	BindVtxPosition();
}

//==========================================================================
// �X�V����
//==========================================================================
void CStoneWall::Update()
{

}

//==========================================================================
// ���_���W
//==========================================================================
void CStoneWall::BindVtxPosition()
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

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		rot.y = m_vecVtxPosition[next].AngleXZ(m_vecVtxPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{
			rot.y *= -1;
		}

		// ��]���f
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecVtxPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);


		/*CEffect3D::Create(
			m_vecVtxPosition[y] + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);*/

		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(0.0f, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, HEIGHT, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);


		// ���_���W���
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CStoneWall::Draw()
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
void CStoneWall::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3* pVtxNor = GetVtxNor();
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	int heightBlock = static_cast<int>(m_vecVtxPosition.size());

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// �c�̕������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH_BLOCK; nCntWidth++)
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

	if (pTex != nullptr)
	{
		float posU = 0.0f, posV = 0.0f;
		int idx = 0;

		int texID = CTexture::GetInstance()->Regist(TEXTURE);
		float intervalV = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU).y;

		for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
		{// �c�̕������J��Ԃ�

			// ���Z�b�g
			posV = 0.0f;

			for (int nCntWidth = 0; nCntWidth < WIDTH_BLOCK; nCntWidth++)
			{// ���̕������J��Ԃ�

				idx = nCntWidth + (nCntHeight * WIDTH_BLOCK);
				pTex[idx] = D3DXVECTOR2(posU, posV);

				// �c�̊������i�߂�
				posV += sqrtf((pVtxPos[idx].y - pVtxPos[idx + 1].y) * (pVtxPos[idx].y - pVtxPos[idx + 1].y)) / intervalV;
				
			}

			// ���̊������i�߂�
			posU += pVtxPos[idx + 1].DistanceXZ(pVtxPos[idx - 1]) / INTERVAL_TEXU;
			
		}
	}

	// ���_���X�V
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ��_�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CStoneWall::GetVecPosition(int idx)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecSegmentPosition[idx];
}

//==========================================================================
// ��_�̈ʒu�ݒ�
//==========================================================================
void CStoneWall::SetVecPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return;

	m_vecSegmentPosition[idx] = pos;
}

//==========================================================================
// �e���_�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CStoneWall::GetVecVtxPosition(int idx)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecVtxPosition[idx];
}

//==========================================================================
// �e���_�̈ʒu�ݒ�
//==========================================================================
void CStoneWall::SetVecVtxPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return;

	m_vecVtxPosition[idx] = pos;
}