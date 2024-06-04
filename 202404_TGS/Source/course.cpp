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

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const int WIDTH_BLOCK = 2;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCourse::CCourse(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_pCollisionLineBox.clear();	// �����蔻��{�b�N�X

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

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 100.0f, 0.0f));				// �ʒu
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_LineInfo.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	if (FAILED(hr))
	{// ���s���Ă�����
		return E_FAIL;
	}

	// ���_���ݒ�
	SetVtx();

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCourse::Uninit()
{

	// �I������
	CObject3DMesh::Uninit();
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

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 100.0f, 0.0f));				// �ʒu
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_LineInfo.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();
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

	for (int y = 0; y < static_cast<int>(m_LineInfo.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		// ����̏��
		LineInfo info = m_LineInfo[y];

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �ʒu���f
		mtxTrans.Translation(info.pos);
		mtxParent.Multiply(mtxParent, mtxTrans);

		// �������f
		mtxRotate.RotationYawPitchRoll(info.rot.y, info.rot.x, info.rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(info.width, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-info.width, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);


		// ���_���W���
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

		MyLib::Vector3 setpos = (pVtxPos[idx] + pVtxPos[nextidx]) * 0.5f;
		setpos.y = GetPosition().y;

		m_pCollisionLineBox[y]->SetPosition(setpos);


		/*CEffect3D::Create(
			pVtxPos[idx] + MyLib::Vector3(0.0f, setpos.y, 0.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			pVtxPos[nextidx] + MyLib::Vector3(0.0f, setpos.y, 0.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);*/
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

	int heightBlock = static_cast<int>(m_LineInfo.size());

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

		LineInfo info;
		info.pos = MyLib::Vector3(0.0f, 0.0f, 0.0f);
		info.rot = 0.0f;
		info.width = 200.0f;

		m_LineInfo.push_back(info);

		info.pos = MyLib::Vector3(0.0f, 0.0f, 200.0f);
		m_LineInfo.push_back(info);

		MyLib::AABB aabb(-25.0f, 25.0f);
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)));
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)));

		Save();
		return E_FAIL;
	}

	// �\���̂̃T�C�Y���擾
	std::streamsize structSize = sizeof(LineInfo);
	
	// �t�@�C���̖����܂Ńf�[�^��ǂݍ���
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);
	
	// �f�[�^�̌����v�Z
	size_t numVectors = fileSize / structSize;
	
	// �x�N�g���̔z���p��
	m_LineInfo.resize(numVectors);
	
	// �t�@�C������f�[�^��ǂݍ���
	File.read(reinterpret_cast<char*>(m_LineInfo.data()), fileSize);

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
	File.write(reinterpret_cast<char*>(m_LineInfo.data()), m_LineInfo.size() * sizeof(LineInfo));
		
	// �t�@�C�������
	File.close();
}

//==========================================================================
// �ӏ��擾
//==========================================================================
CCourse::LineInfo CCourse::GetLineInfo(int idx)
{
	if (static_cast<int>(m_LineInfo.size()) <= idx) return LineInfo();

	return m_LineInfo[idx];
}

//==========================================================================
// �ӏ��ݒ�
//==========================================================================
void CCourse::SetLineInfo(int idx, const LineInfo& info)
{
	if (static_cast<int>(m_LineInfo.size()) <= idx) return;

	m_LineInfo[idx] = info;
}

//==========================================================================
// �ӏ��ǉ�
//==========================================================================
void CCourse::PushLineInfo()
{
	m_LineInfo.push_back(LineInfo());
}

//==========================================================================
// �ӏ��폜
//==========================================================================
void CCourse::PopLineInfo()
{
	if (static_cast<int>(m_LineInfo.size()) >= 1)
	{
		m_LineInfo.pop_back();
	}
}