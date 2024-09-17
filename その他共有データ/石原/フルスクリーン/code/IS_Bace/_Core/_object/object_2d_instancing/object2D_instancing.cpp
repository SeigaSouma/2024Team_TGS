//======================================================
//
//�|���S���i2D,�C���X�^���V���O����j����[object2D_instancing.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "object2D_instancing.h"
#include "..\..\..\..\main.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\utility\Utility.h"
#include <string>

//���_���ݒ�
D3DVERTEXELEMENT9 CObject2D_Instancing::m_aVtxElem[] =
{
	{0,0,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
	{0,8,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
	{1,0,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},
	{2,0,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2},
	{3,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR,0},
	D3DDECL_END()
};

//=================================
//�R���X�g���N�^�i�f�t�H���g�j
//=================================
CObject2D_Instancing::CObject2D_Instancing(int nPriority) : CObject(nPriority)
{
	//�N���A
	m_pDecl = nullptr;
	m_pSinglePolyVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_pInstancePosVtxBuff = nullptr;
	m_pInstanceUVVtxBuff = nullptr;
	m_pInstanceColVtxBuff = nullptr;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_nIdxTexture = -1;
	m_InstanceList.clear();
	m_nInstanceNum = 0;
	m_pEffect = nullptr;
}

//=================================
//�f�X�g���N�^
//=================================
CObject2D_Instancing::~CObject2D_Instancing()
{
}

//========================
//����������
//========================
HRESULT CObject2D_Instancing::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(SinglePolyVtx) * 4,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pSinglePolyVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	//���_�錾����
	if (FAILED(pDevice->CreateVertexDeclaration(m_aVtxElem, &m_pDecl)))
	{
		return E_FAIL;
	}

	//����
	D3DXCreateEffectFromFile(
		pDevice,
		"code\\IS_Bace\\_Core\\_object\\object_2d_instancing\\instancing.fx",
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	//�C���f�b�N�X�o�b�t�@�ݒ�
	WORD* pIdx;	//�C���f�b�N�X���ւ̃|�C���^
	int nCntWidth;	//�C���f�b�N�X�J�E���^

	//�o�b�t�@���b�N
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 2;
	pIdx[4] = 1;
	pIdx[5] = 3;

	//�o�b�t�@�A�����b�N
	m_pIdxBuff->Unlock();

	//�P�|���ݒ�
	SetVtx();

	return S_OK;
}

//========================
//�I������
//========================
void CObject2D_Instancing::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pSinglePolyVtxBuff != nullptr)
	{
		m_pSinglePolyVtxBuff->Release();
		m_pSinglePolyVtxBuff = nullptr;
	}
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
	if (m_pEffect != nullptr)
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
	ReleaseInstanceBuff();

	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CObject2D_Instancing::Update(void)
{

}

//========================
//�`�揈��
//========================
void CObject2D_Instancing::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();			//�e�N�X�`���I�u�W�F�N�g�擾

	//�C���X�^���X�p�o�b�t�@�ސݒ�
	SetInstanceBuff();

	//�`��J�n
	//���g���ݒ�
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_nInstanceNum);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetVertexDeclaration(m_pDecl);
	pDevice->SetStreamSource(0, m_pSinglePolyVtxBuff, 0, sizeof(SinglePolyVtx));
	pDevice->SetStreamSource(1, m_pInstancePosVtxBuff, 0, sizeof(InstancePosVtx));
	pDevice->SetStreamSource(2, m_pInstanceUVVtxBuff, 0, sizeof(InstanceUVVtx));
	pDevice->SetStreamSource(3, m_pInstanceColVtxBuff, 0, sizeof(D3DXCOLOR));
	pDevice->SetIndices(m_pIdxBuff);
	
	m_pEffect->SetTechnique("InstancingTech");
	unsigned int passNum = 0;
	m_pEffect->Begin(&passNum, 0);
	m_pEffect->BeginPass(0);

	m_pEffect->SetTexture("tex", pTexture->GetAddress(m_nIdxTexture));
	m_pEffect->SetFloat("screenWidth", SCREEN_WIDTH * 0.5f);
	m_pEffect->SetFloat("screenHeight", SCREEN_HEIGHT * 0.5f);

	//�`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	m_pEffect->EndPass();
	m_pEffect->End();

	//���g���߂�
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSourceFreq(3, 1);
}

//========================
//���������i�e�N�X�`���Ȃ��j
//========================
CObject2D_Instancing* CObject2D_Instancing::Create(const float fWidth, const float fHeight, const int nInstanceNum, const int nPriority)
{
	CObject2D_Instancing* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pObj = new CObject2D_Instancing(nPriority);

		//������
		pObj->Init();

		//�l�ݒ�
		pObj->SetSize(fWidth, fHeight);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//========================
//�T�C�Y�ύX
//========================
HRESULT CObject2D_Instancing::SetSize(const float fWidth, const float fHeight)
{
	//�l�ύX
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	//���_�z�u
	return SetVtx();
}

//========================
//�e�N�X�`���ݒ�
//========================
void CObject2D_Instancing::BindTexture(int nIdx)
{
	m_nIdxTexture = nIdx;

	SetVtx();
}

//========================
//�C���X�^���X���ݒ�
//========================
void CObject2D_Instancing::SetInstanceNum(const int num)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾

	m_nInstanceNum = num;

	//�o�b�t�@������Ȃ�j��
	ReleaseInstanceBuff();

	//�o�b�t�@����
	//�ʒu�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(InstancePosVtx) * m_nInstanceNum,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pInstancePosVtxBuff,
		nullptr);
	//UV�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(InstanceUVVtx) * m_nInstanceNum,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pInstanceUVVtxBuff,
		nullptr);
	//�J���[�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(D3DXCOLOR) * m_nInstanceNum,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pInstanceColVtxBuff,
		nullptr);
}

//========================
//�C���X�^���X�ǉ��i���ł��j
//========================
void CObject2D_Instancing::AddInstance(float x, float y, float u, float v, D3DXCOLOR col)
{
	Instance ins;
	ins.pos.x = x;
	ins.pos.y = y;
	ins.uv.u = u;
	ins.uv.v = v;
	ins.col = col;

	AddList(ins);
}

//========================
//�C���X�^���X�ǉ��i�ȗ����j
//========================
void CObject2D_Instancing::AddInstance(D3DXVECTOR3 pos, const int nPatternU, const int nPatternV, D3DXCOLOR col)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	int patWidth = IS_Utility::Clamp(pTexture->GetPatWidth(m_nIdxTexture), INT_MAX, 1);
	int patHeight = IS_Utility::Clamp(pTexture->GetPatHeight(m_nIdxTexture), INT_MAX, 1);

	Instance ins;
	ins.pos.x = pos.x;
	ins.pos.y = pos.y;
	ins.uv.u = (float)nPatternU / patWidth;
	ins.uv.v = (float)nPatternV / patHeight;

	ins.col.r = col.r;
	ins.col.g = col.g;
	ins.col.b = col.b;
	ins.col.a = col.a;

	AddList(ins);
}

//========================
//���X�g�ǉ�
//========================
void CObject2D_Instancing::AddList(Instance ins)
{
	if (m_InstanceList.size() + 1 <= m_nInstanceNum)
	{
		m_InstanceList.emplace_back(ins);
	}
}

//========================
//�C���X�^���X�p�o�b�t�@�ɓ����
//========================
void CObject2D_Instancing::SetInstanceBuff()
{
	//���X�g�̃C�e���[�^���炤
	auto itr = m_InstanceList.begin();

	//�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	InstancePosVtx* pPos;	//�ݒ�p�|�C���^
	InstanceUVVtx* pTex;
	D3DXCOLOR* pCol;

	m_pInstancePosVtxBuff->Lock(0, 0, (void**)&pPos, 0);
	m_pInstanceUVVtxBuff->Lock(0, 0, (void**)&pTex, 0);
	m_pInstanceColVtxBuff->Lock(0, 0, (void**)&pCol, 0);

	for (int cnt = 0; cnt < m_nInstanceNum; cnt++, itr++)
	{
		pPos[cnt] = (*itr).pos;
		pTex[cnt] = (*itr).uv;
		pCol[cnt] = (*itr).col;
	}

	//���_�o�b�t�@���A�����b�N
	m_pInstanceColVtxBuff->Unlock();
	m_pInstanceUVVtxBuff->Unlock();
	m_pInstancePosVtxBuff->Unlock();
}

//========================
//�C���X�^���X�o�b�t�@�j��
//========================
void CObject2D_Instancing::ReleaseInstanceBuff()
{
	if (m_pInstancePosVtxBuff != nullptr)
	{
		m_pInstancePosVtxBuff->Release();
		m_pInstancePosVtxBuff = nullptr;
	}
	if (m_pInstanceUVVtxBuff != nullptr)
	{
		m_pInstanceUVVtxBuff->Release();
		m_pInstanceUVVtxBuff = nullptr;
	}
	if (m_pInstanceColVtxBuff != nullptr)
	{
		m_pInstanceColVtxBuff->Release();
		m_pInstanceColVtxBuff = nullptr;
	}
}

//========================
//���_���W�ݒ�
//========================
HRESULT CObject2D_Instancing::SetVtx(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	int patWidth = IS_Utility::Clamp(pTexture->GetPatWidth(m_nIdxTexture), INT_MAX, 1);
	int patHeight = IS_Utility::Clamp(pTexture->GetPatHeight(m_nIdxTexture), INT_MAX, 1);

	SinglePolyVtx singleVtx[4];
	singleVtx[0] = { 0.0f,0.0f,0.0f,0.0f };
	singleVtx[1] = { m_fWidth,0.0f,1.0f / patWidth,0.0f };
	singleVtx[2] = { 0.0f,m_fHeight,0.0f,1.0f / patHeight };
	singleVtx[3] = { m_fWidth,m_fHeight,1.0f / patWidth, 1.0f / patHeight };

	//���_�o�b�t�@�̃��b�N�ƒ��_���ւ̃|�C���^���擾
	SinglePolyVtx* pVtx;	//�ݒ�p�|�C���^
	if (FAILED(m_pSinglePolyVtxBuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	for (int cnt = 0; cnt < 4; cnt++)
	{
		pVtx[cnt] = singleVtx[cnt];
	}

	//���_�o�b�t�@���A�����b�N
	if (FAILED(m_pSinglePolyVtxBuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}
