//======================================================
//
//�I�u�W�F�N�g�i�r���{�[�h�j����[objectBillboard.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "objectBillboard.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\input\input.h"
#include "..\..\utility\Utility.h"

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject3D(nPriority)
{

}

//=================================
//�f�X�g���N�^
//=================================
CObjectBillboard::~CObjectBillboard()
{
}

//========================
//����������
//========================
HRESULT CObjectBillboard::Init(void)
{
	return CObject3D::Init();
}

//========================
//�I������
//========================
void CObjectBillboard::Uninit(void)
{
	CObject3D::Uninit();
}

//========================
//�X�V����
//========================
void CObjectBillboard::Update(void)
{
	CObject3D::Update();
}

//========================
//�`�揈��
//========================
void CObjectBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p
	D3DXMATRIX mtxView;				//�r���[�}�g���擾�p
	D3DXVECTOR3 pos = CObject3D::GetPos();
	D3DXMATRIX* mtxWorld = CObject3D::GetMtxWorld();
	LPDIRECT3DVERTEXBUFFER9 pVtxbuff = CObject3D::GetVtxBuff();

	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(mtxWorld);

	//�r���[�}�g���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(mtxWorld, nullptr, &mtxView);
	//�C��
	mtxWorld->_41 = 0.0f;
	mtxWorld->_42 = 0.0f;
	mtxWorld->_43 = 0.0f;

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, pVtxbuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(CObject3D::GetIdxTexture()));

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//��������
//========================
CObjectBillboard* CObjectBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, const int nIdxTexture)
{
	CObjectBillboard* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pObj = new CObjectBillboard;

		//������
		pObj->Init();

		//�ݒ�
		pObj->SetPos(pos);
		pObj->SetRot(rot);
		pObj->SetSize(fWidth, fDepth);
		pObj->BindTexture(nIdxTexture);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}
