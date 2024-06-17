//=============================================================================
// 
//  �N������ [waterfield.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "waterfield.h"
#include "manager.h"
#include "input.h"
#include "calculation.h"
#include "debugproc.h"

// �p����
#include "waterfield_right.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water_03.jpg";
	const int BLOCK = 240;
	const float BLOCK_SIZE = 400.0f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CWaterField::CWaterField(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_fTexU = 0.0f;		// U�X�N���[���p
	m_fTexV = 0.0f;		// V�X�N���[���p
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CWaterField::~CWaterField()
{

}

//==========================================================================
// ��������
//==========================================================================
CWaterField* CWaterField::Create(TYPE type)
{
	// �������̊m��
	CWaterField* pObjMeshField = nullptr;

	switch (type)
	{
	case CWaterField::TYPE_NORMAL:
		pObjMeshField = DEBUG_NEW CWaterField;
		break;

	case CWaterField::TYPE_RIGHT:
		pObjMeshField = DEBUG_NEW CWaterField_Right;
		break;

	case CWaterField::TYPE_LEFT:
		pObjMeshField = DEBUG_NEW CWaterField_Right;
		break;

	default:
		return nullptr;
		break;
	}

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
HRESULT CWaterField::Init()
{
	// �e�N�X�`���̊��蓖��
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 0.1f, 0.0f));			// �ʒu
	SetWidthBlock(BLOCK);		// ������
	SetHeightBlock(BLOCK);		// �c����
	SetWidthLen(BLOCK_SIZE);	// �c����
	SetHeightLen(BLOCK_SIZE);	// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();


	// �S�Ă̗v�f����������
	D3DXCOLOR* pVtxCol = GetVtxCol();
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(0.8f, 0.8f, 1.0f, 0.8f));

	return E_FAIL;
}

//==========================================================================
// �I������
//==========================================================================
void CWaterField::Uninit()
{
	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �X�V����
//==========================================================================
void CWaterField::Update()
{
	m_fTexU -= 0.003f;		// U�X�N���[���p

	if (m_fTexU >= 1.0f)
	{
		m_fTexU = 0.0f;
	}
	else if (m_fTexU <= 0.0f)
	{
		m_fTexU = 1.0f;
	}

	if (m_fTexV >= 1.0f)
	{
		m_fTexV = 0.0f;
	}
	else if (m_fTexV <= 0.0f)
	{
		m_fTexV = 1.0f;
	}

	// ���_���ݒ�
	SetVtx();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CWaterField::Draw()
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// �`�揈��
	CObject3DMesh::Draw();
}

//==========================================================================
// ���_���ݒ菈��
//==========================================================================
void CWaterField::SetVtx()
{
	// ���_���X�V
	CObject3DMesh::SetVtx();

	VERTEX_3D* pVtx;	// ���_���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	int nHBlock = GetHeightBlock();
	int nWBlock = GetWidthBlock();

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < nHBlock + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < nWBlock + 1; nCntWidth++)
		{// ���̒��_�����J��Ԃ�

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			(
				((float)nCntWidth / (1.0f / (float)(nWBlock + 1))) * (1.0f / (float)(nWBlock + 1)) + m_fTexU,
				((float)nCntHeight / (1.0f / (float)(nHBlock + 1))) * (1.0f / (float)(nHBlock + 1)) + m_fTexV
			);
			pVtx += 1;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();
}
