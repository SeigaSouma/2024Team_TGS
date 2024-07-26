//=============================================================================
// 
//  �X�t�B�[���h(�Œ蕽��) [townfield_fixedplane.cpp]
//  Author : �Ό��D�n
// 
//=============================================================================
#include "townfield_fixedplane.h"
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
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\soil.jpg";
	const int WIDTH_BLOCK = 2;
	const float FIELD_WORLDLINE_Z = 3000.0f;		// ��΍��W�̃��C��
	const float INTERVAL_TEXU = 900.0f;	// U���W�̊Ԋu
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CTownField_FixedPlane::CTownField_FixedPlane(int nPriority, const LAYER layer) : CMapMesh(nPriority, layer)
{
	
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CTownField_FixedPlane::~CTownField_FixedPlane()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CTownField_FixedPlane::Init()
{
	HRESULT hr;

	// ����������
	CMapMesh::Init();

	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);
	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CTownField_FixedPlane::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// ���_���W
//==========================================================================
void CTownField_FixedPlane::BindVtxPosition()
{
	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rot;

	for (int cnt = 0; cnt < static_cast<int>(m_vecVtxPosition.size()); cnt++)
	{
		pVtxPos[cnt] = m_vecVtxPosition[cnt];
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �e���_UV���W�ݒ�
//==========================================================================
void CTownField_FixedPlane::SetVtxTexUV()
{
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int nHeight = GetHeightBlock();

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


			int front = (nCntHeight * WIDTH_BLOCK);
			int back = (nCntHeight * WIDTH_BLOCK) + 1;

			// �c�̊������i�߂�
			posV += sqrtf((pVtxPos[front].z - pVtxPos[front + 1].z) * (pVtxPos[front].z - pVtxPos[front + 1].z)) / intervalV;

			pTex[back] = D3DXVECTOR2(posU, 0.0f);
			pTex[front] = D3DXVECTOR2(posU, posV);

			// ���̊������i�߂�
			posU += pVtxPos[back + 1].DistanceXZ(pVtxPos[back - 1]) / INTERVAL_TEXU;

		}
	}
}
