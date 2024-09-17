//======================================================
//
//�I�u�W�F�N�g�i3D�j����[object3D.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "object3D.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\utility\Utility.h"

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	//�N���A
	m_pVtxbuff = nullptr;
	m_nIdxTexture = -1;
	m_pos = IS_Utility::VEC3_ZERO;
	m_rot = IS_Utility::VEC3_ZERO;
	m_fWidth = 0.0f;
	m_fDepth = 0.0f;
}

//=================================
//�f�X�g���N�^
//=================================
CObject3D::~CObject3D()
{
}

//========================
//����������
//========================
HRESULT CObject3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		nullptr)))
	{
		return E_FAIL;
	}

	//���_���W�i�T�C�Y�j�E�@���ݒ�
	SetSize(m_fWidth, m_fDepth);

	//�F�ݒ�
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//�e�N�X�`�����W�ݒ�
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//========================
//�I������
//========================
void CObject3D::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxbuff != nullptr)
	{
		m_pVtxbuff->Release();
		m_pVtxbuff = nullptr;
	}

	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CObject3D::Update(void)
{
	
}

//========================
//�`�揈��
//========================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();						//�e�N�X�`���I�u�W�F�N�g�擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p

	CManager::GetInstance()->GetRenderer()->SetEnableAlplaTest(true);
	CManager::GetInstance()->GetRenderer()->SetEnableZTest(false);

	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxbuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//�`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	CManager::GetInstance()->GetRenderer()->SetEnableAlplaTest(false);
	CManager::GetInstance()->GetRenderer()->SetEnableZTest(true);
}

//========================
//��������
//========================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, int nPriority)
{
	CObject3D* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pObj = new CObject3D(nPriority);

		//������
		pObj->Init();

		//�f�[�^�ݒ�
		pObj->m_pos = pos;
		pObj->m_rot = rot;
		pObj->SetSize(fWidth, fDepth);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//========================
//�F�ݒ�
//========================
HRESULT CObject3D::SetCol(const D3DXCOLOR col)
{
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	VERTEX_3D *pVtx;	//�ݒ�p�|�C���^
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//���_�J���[�̕ϊ�
	D3DCOLOR exchange = D3DCOLOR_COLORVALUE(col.r, col.g, col.b, col.a);

	//���_�J���[�̐ݒ�
	pVtx[0].col = exchange;
	pVtx[1].col = exchange;
	pVtx[2].col = exchange;
	pVtx[3].col = exchange;

	//���_�o�b�t�@���A�����b�N
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================
//�T�C�Y�ݒ�
//========================
void CObject3D::SetSize(const float fWidth, const float fDepth)
{
	VERTEX_3D *pVtx;	//���_�Đݒ�Ɏg�p

	//�T�C�Y�l�ݒ�
	m_fWidth = fWidth;
	m_fDepth = fDepth;

	//���_�Đݒ�
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	//���_���W�i���΍��W�j
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth * 0.5f, 0.0f, m_fDepth * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth * 0.5f, 0.0f, m_fDepth * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth * 0.5f, 0.0f, -m_fDepth * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth * 0.5f, 0.0f, -m_fDepth * 0.5f);

	//�@���v�Z
	CulcNor(pVtx);

	//���_�o�b�t�@���A�����b�N
	m_pVtxbuff->Unlock();
}

//========================
//�e�N�X�`�����W�ݒ�iVec2�w��j
//========================
HRESULT CObject3D::SetTex(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex3)
{
	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	VERTEX_3D* pVtx;	//�ݒ�p�|�C���^
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void**)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//�e�N�X�`�����W
	pVtx[0].tex = tex0;
	pVtx[1].tex = D3DXVECTOR2(tex3.x, tex0.y);
	pVtx[2].tex = D3DXVECTOR2(tex0.x, tex3.y);
	pVtx[3].tex = tex3;

	//���_�o�b�t�@���A�����b�N
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================
//�e�N�X�`�����W�ݒ�i�ԍ��w��j
//========================
HRESULT CObject3D::SetTex(const int nPatternU, const int nPatternV)
{
	//�e�N�X�`���I�u�W�F�N�g
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	//�e�N�X�`�����W
	D3DXVECTOR2 tex0, tex3;
	tex0 = tex3 = D3DXVECTOR2(0.0f, 0.0f);
	//�p�^�[��������
	int nTexPatU = pTexture->GetPatWidth(m_nIdxTexture);
	int nTexPatV = pTexture->GetPatHeight(m_nIdxTexture);

	//�p�^�[���w�肪�����Ă��邩
	if (nPatternU >= nTexPatU || nPatternV >= nTexPatV)
	{
		return E_FAIL;
	}

	//���W�v�Z
	tex0.x = (float)nPatternU / nTexPatU;
	tex0.y = (float)nPatternV / nTexPatV;
	tex3.x = (float)(nPatternU + 1) / nTexPatU;
	tex3.x = (float)(nPatternV + 1) / nTexPatV;

	return SetTex(tex0, tex3);
}

//========================
//�@���v�Z
//========================
void CObject3D::CulcNor(VERTEX_3D* pVtx)
{
	//�@���x�N�g��
	D3DXVECTOR3 vec1 = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vec2 = pVtx[2].pos - pVtx[0].pos;
	D3DXVec3Cross(&pVtx[0].nor, &vec1, &vec2);
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	vec1 = pVtx[2].pos - pVtx[3].pos;
	vec2 = pVtx[1].pos - pVtx[3].pos;
	D3DXVec3Cross(&pVtx[3].nor, &vec1, &vec2);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

	pVtx[1].nor = (pVtx[0].nor + pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);

	pVtx[2].nor = (pVtx[0].nor + pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);
}
