//=============================================================================
// 
//  �N�H�[�^�j�I������ [objectQuaternion.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "objectQuaternion.h"
#include "calculation.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CObjectQuaternion::CObjectQuaternion(int nPriority) : CObjectX(nPriority)
{
	// �l�̃N���A
	D3DXQuaternionIdentity(&m_quaternion);
	m_vecAxis = 0.0f;			// ��]��
	m_fValueRot = 0.0f;			// ��]�p
	D3DXMatrixIdentity(&m_RotationMtx);	// �v�Z�p�}�g���b�N�X�錾
	m_fRotDest = 0.0f;
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CObjectQuaternion::~CObjectQuaternion()
{

}

//==========================================================================
// ��������
//==========================================================================
CObjectQuaternion* CObjectQuaternion::Create()
{
	// �������̊m��
	CObjectQuaternion* pObjChara = DEBUG_NEW CObjectQuaternion;

	if (pObjChara != nullptr)
	{
		// ����������
		HRESULT hr = pObjChara->Init();
		if (FAILED(hr)){
			return nullptr;
		}
	}

	return pObjChara;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectQuaternion::Init()
{
	CObjectX::Init("data\\MODEL\\box.x");
	CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CObjectQuaternion::Init(const std::string& file)
{
	CObjectX::Init(file);
	CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CObjectQuaternion::Uninit()
{
	// �I������
	CObjectX::Uninit();

	// �I�u�W�F�N�g�̔j��
	Release();
}

//==========================================================================
// �X�V����
//==========================================================================
void CObjectQuaternion::Update()
{
	// �I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
#if 0
	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();


	MyLib::Vector3 vec = MyLib::Vector3(sinf(D3DX_PI + rot.y), 0.0f, cosf(D3DX_PI + rot.y));

	m_vecAxis.x = vec.z;
	m_vecAxis.z = -vec.x;



	// �ʒu�X�V
	pos += move;

	// ��]�p�ݒ�
	m_fValueRot = move.x * 0.005f;

	move.x += (0.0f - move.x) * 0.05f;


	// �ݒ�
	SetPosition(pos);
	SetMove(move);
	SetRotation(rot);
#endif

}

//==========================================================================
// ���[���h�}�g���b�N�X�̌v�Z����
//==========================================================================
void CObjectQuaternion::CalWorldMtx()
{
#if 0
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ���擾
	MyLib::Vector3 pos = GetPosition();

	// ������
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXQuaternionIdentity(&m_quaternion);


	// ����
	MyLib::Vector3 rotDiff = GetRotation() - GetOldRotation();

	// �C�ӂ̉�]���ɂ������]�p����N�H�[�^�j�I���v�Z
	D3DXQuaternionRotationAxis(&m_quaternion, &m_vecAxis, m_fValueRot);

	// ��]�}�g���b�N�X���쐬
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);

	// �N�H�[�^�j�I���𐳋K��
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);


	// �����𔽉f����
	D3DXMatrixMultiply(&m_RotationMtx, &m_RotationMtx, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_RotationMtx);

	// �ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
#else


	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	MyLib::Matrix mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X�錾

	// ���擾
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 rotOld = GetOldRotation();
	MyLib::Matrix mtxWorld = GetWorldMtx();


	// ���[���h�}�g���b�N�X�̏�����
	mtxWorld.Identity();
	D3DXQuaternionIdentity(&m_quaternion);

	MyLib::Vector3 vec;

	D3DXQUATERNION quatX;
	vec = { 1.0f, 0.0f, 0.0f };
	D3DXQuaternionRotationAxis(&quatX, &vec, rotOld.x);

	D3DXQUATERNION quatY;
	vec = { 0.0f, 1.0f, 0.0f };
	D3DXQuaternionRotationAxis(&quatY, &vec, rotOld.y);

	D3DXQUATERNION quatZ;
	vec = { 0.0f, 0.0f, 1.0f };
	D3DXQuaternionRotationAxis(&quatZ, &vec, rotOld.z);

	// x���Ay���Az���̏��ŉ�]��K�p
	m_quaternion = quatY * m_quaternion;
	m_quaternion = quatX * m_quaternion;
	m_quaternion = quatZ * m_quaternion;

	// �N�H�[�^�j�I���𐳋K��
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// �N�H�[�^�j�I����g�ݍ��킹�ĉ�]�}�g���b�N�X���쐬
	mtxRot.Identity();
	D3DXMATRIX D3DmtxRot = mtxRot.ConvertD3DXMATRIX();
	D3DXMatrixRotationQuaternion(&D3DmtxRot, &m_quaternion);


	// �����𔽉f����
	mtxWorld.Multiply(mtxWorld, mtxRot);


	// �ʒu�𔽉f����
	mtxTrans.Translation(pos);
	mtxWorld.Multiply(mtxWorld, mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMATRIX mtx = mtxWorld.ConvertD3DXMATRIX();
	pDevice->SetTransform(D3DTS_WORLD, &mtx);
	SetWorldMtx(mtxWorld);
#endif
}

//==========================================================================
// �`�揈��
//==========================================================================
void CObjectQuaternion::Draw()
{
	// ���[���h�}�g���b�N�X�̌v�Z����
	CalWorldMtx();

	// �`��̂�
	CObjectX::DrawOnly();
}
