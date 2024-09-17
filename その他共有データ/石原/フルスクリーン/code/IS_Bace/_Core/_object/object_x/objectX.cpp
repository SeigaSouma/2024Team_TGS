//======================================================
//
//�I�u�W�F�N�g�iX�j����[object3D.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "objectX.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\input\input.h"
#include "..\..\xmodel\xmodel.h"
#include "..\object_empty\object.h"
#include "..\..\utility\Utility.h"
#include <assert.h>

//�ÓI�����o�ϐ�
CObjectX* CObjectX::m_pTop = nullptr;
CObjectX* CObjectX::m_pCur = nullptr;
int CObjectX::m_nNumAll = 0;

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	//�N���A
	m_pos = IS_Utility::VEC3_ZERO;
	m_rot = IS_Utility::VEC3_ZERO;

	if (m_pCur == nullptr)
	{//�Ō�������Ȃ��i���Ȃ킿�擪�����Ȃ��j
		m_pTop = this;		//�����擪
		m_pPrev = nullptr;		//�O��N�����Ȃ�
		m_pNext = nullptr;
	}
	else
	{//�Ō��������
		m_pPrev = m_pCur;		//�Ō���������̑O�̃I�u�W�F
		m_pCur->m_pNext = this;	//�Ō���̎��̃I�u�W�F������
		m_pNext = nullptr;			//�����̎��̃I�u�W�F�͂��Ȃ�
	}
	m_pCur = this;				//�����Ō��
	m_bExclusion = false;		//�����Ă�
	m_pModel = nullptr;
	m_bEnable = false;
	m_nNumAll++;
}

//=================================
//�f�X�g���N�^
//=================================
CObjectX::~CObjectX()
{
}

//========================
//����������
//========================
HRESULT CObjectX::Init(void)
{
	return S_OK;
}

//========================
//�I������
//========================
void CObjectX::Uninit(void)
{
	m_bExclusion = true;		//���O�\��

	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CObjectX::Update(void)
{
	
}

//========================
//�`�揈��
//========================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetInstance()->GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//�v�Z�p
	D3DMATERIAL9 matDef;												//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;													//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//���f���擾
	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&mtxWorld);

	//�g�k�𔽉f
	//D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
	//D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pModel->GetBufMat()->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModel->GetNumMat(); nCntMat++)
	{
		//�}�e���A���ύX
		D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;

		if (m_bEnable == true)
		{//�F��ύX����ꍇ
		 //���C���F�ύX
			changeMat.Diffuse = m_changeColor;
		}

		//�}�e���A���ݒ�
		pDevice->SetMaterial(&changeMat);

		//�e�N�X�`���ݒ�
		pDevice->SetTexture(0, pTexture->GetAddress(m_pModel->GetIdxTexture()[nCntMat]));

		//���f���`��
		m_pModel->GetMesh()->DrawSubset(nCntMat);
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//========================
//��������
//========================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CObjectX* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pObj = new CObjectX;

		//������
		pObj->Init();

		//�ݒ�
		pObj->SetPos(pos);
		pObj->SetRot(rot);
		pObj->SetModel(pModel);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//========================
//���f���ݒ�ƃT�C�Y�v��
//========================
void CObjectX::SetModel(CXModel * pModel)
{
	m_pModel = pModel;

	D3DXVECTOR3 vtxMin, vtxMax;
	m_fWidth = vtxMax.x - vtxMin.x;
	m_fHeight = vtxMax.y - vtxMin.y;
	m_fDepth = vtxMax.z - vtxMin.z;
}

//========================
//�F�ύX�ݒ�
//========================
void CObjectX::SetColor(const bool bEnable, const D3DXCOLOR col)
{
	m_bEnable = bEnable;

	if (bEnable == true)
	{
		m_changeColor = col;
	}
}

//========================
//�I�u�W�F�N�g�P�ʏ��O����
//========================
void CObjectX::Delete(CXModel* pTarget)
{
	CObjectX* pObject = m_pTop;	//�擪������

	while (pObject != nullptr)
	{//�Ō���܂ŉ񂵑�����
		CObjectX* pObjectNext = pObject->m_pNext;	//���̃I�u�W�F�ۑ�

		if (pObject->GetModel() == pTarget)
		{
			pObject->Uninit();	//���O
		}

		pObject = pObjectNext;	//��������
	}
}

//========================
//���O����
//========================
void CObjectX::Exclusion(void)
{
	if (m_pPrev != nullptr)
	{//�O�ɃI�u�W�F������
		m_pPrev->m_pNext = m_pNext;	//�O�̃I�u�W�F�̎��̃I�u�W�F�͎����̎��̃I�u�W�F
	}
	if (m_pNext != nullptr)
	{
		m_pNext->m_pPrev = m_pPrev;	//���̃I�u�W�F�̑O�̃I�u�W�F�͎����̑O�̃I�u�W�F
	}

	if (m_pCur == this)
	{//�Ō���ł���
		m_pCur = m_pPrev;	//�Ō���������̑O�̃I�u�W�F�ɂ���
	}
	if (m_pTop == this)
	{
		m_pTop = m_pNext;	//�擪�������̎��̃I�u�W�F�ɂ���
	}

	//����
	m_nNumAll--;	//�������炷
}