//======================================================
//
//�L�����̃p�[�c����[charaparts.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "charaparts.h"
#include "..\..\..\_Core\manager\manager.h"
#include "..\..\..\_Core\renderer\renderer.h"
#include "..\..\..\_Core\texture\texture.h"
#include "..\..\utility\Utility.h"
#include "..\..\xmodel\xmodel.h"
#include <assert.h>

//=================================
//�R���X�g���N�^
//=================================
CCharaParts::CCharaParts()
{
	m_pModel = nullptr;
	m_pos = IS_Utility::VEC3_ZERO;
	m_rot = IS_Utility::VEC3_ZERO;
	m_posMotioned = IS_Utility::VEC3_ZERO;
	m_rotMotioned = IS_Utility::VEC3_ZERO;
	m_pParent = nullptr;
}

//=================================
//�f�X�g���N�^
//=================================
CCharaParts::~CCharaParts()
{
}

//========================
//����������
//========================
HRESULT CCharaParts::Init(void)
{
	return S_OK;
}

//========================
//�I������
//========================
void CCharaParts::Uninit(void)
{
	
}

//========================
//�X�V����
//========================
void CCharaParts::Update(void)
{
	//�������񉼂̒l������
	m_posMotioned = m_pos;
	m_rotMotioned = m_rot;
}

//========================
//�`�揈��
//========================
void CCharaParts::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p
	D3DXMATRIX mtxParent;			//�e�̃}�g��
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//���f���擾
	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotMotioned.y, m_rotMotioned.x, m_rotMotioned.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_posMotioned.x, m_posMotioned.y, m_posMotioned.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//�p�[�c�̐e�}�g���ݒ�
	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMtx();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//�p�[�c�̃}�g���Ɛe�}�g�����������킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pModel->GetBufMat()->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModel->GetNumMat(); nCntMat++)
	{
		//�}�e���A���ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

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
CCharaParts* CCharaParts::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CCharaParts* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�I�u�W�F�N�g2D�̐���
		pObj = new CCharaParts;

		//������
		pObj->Init();
		pObj->SetPos(pos);
		pObj->SetRot(rot);
		pObj->m_pModel = pModel;

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

