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
#include "spline.h"
#include "convert.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\FIELD\\water-bg-pattern-04.jpg";
	const int WIDTH_BLOCK = 2;
	const float WIDTH = 2000.0f;
	const float INTERVAL_TEXU = 500.0f;	// U���W�̊Ԋu
}
const float CCourse::m_fCreateDistance = 200.0f;	// �����Ԋu

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CCourse::CCourse(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_pCollisionLineBox.clear();	// �����蔻��{�b�N�X
	m_vecSegmentPosition.clear();	// ��_�̈ʒu
	m_vecVtxInfo.clear();			// �e���_�̏��
	m_courceLength = 0.0f;
	m_fTexU = 0.0f;	// U�X�N���[���p
	m_fTexV = 0.0f;	// V�X�N���[���p
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
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// �I�u�W�F�N�g3D���b�V���̏���������
	Reset();

	// ���_���ݒ�
	SetVtx();


	D3DXCOLOR* pVtxCol = GetVtxCol();

	// �S�Ă̗v�f����������
	std::fill(pVtxCol, pVtxCol + GetNumVertex(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f));


	// ���_���W�v�Z
	SetVtxPosition();


	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CCourse::Uninit()
{
	m_pCollisionLineBox.clear();

	// �I������
	CObject3DMesh::Uninit();
}

//==========================================================================
// �e���_�v�Z
//==========================================================================
void CCourse::CalVtxPosition()
{

	// �ŏ��ƍŌ�A�t�����ɏ����o��
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// �ŏ�
	angle = m_vecSegmentPosition[1].AngleXZ(m_vecSegmentPosition[0]);
	begin = MyLib::Vector3(
		m_vecSegmentPosition[0].x + sinf(angle) * -10.0f,
		m_vecSegmentPosition[0].y,
		m_vecSegmentPosition[0].z + cosf(angle) * -10.0f);

	// �Ō�
	int endIdx = m_vecSegmentPosition.size() - 1;
	angle = m_vecSegmentPosition[endIdx].AngleXZ(m_vecSegmentPosition[endIdx - 1]);
	end = MyLib::Vector3(
		m_vecSegmentPosition[endIdx].x + sinf(angle) * 10.0f,
		m_vecSegmentPosition[endIdx].y,
		m_vecSegmentPosition[endIdx].z + cosf(angle) * 10.0f);

	m_vecSegmentPosition.insert(m_vecSegmentPosition.begin(), begin);
	m_vecSegmentPosition.push_back(end);

	//// �Z�O�����g�̒������v�Z
	//int segmentSize = m_vecSegmentPosition.size();
	//std::vector<float> vecLength(segmentSize);

	//for (int i = 0; i < segmentSize; ++i)
	//{
	//	// ����̃C���f�b�N�X(���[�v)
	//	int next = (i + 1) % segmentSize;

	//	if (next == 0)
	//	{
	//		vecLength[i] = 10.0f;
	//		break;
	//	}

	//	// �_���m�̋���
	//	vecLength[i] = m_vecSegmentPosition[i].Distance(m_vecSegmentPosition[next]);
	//}


	// ���_���N���A
	m_vecVtxInfo.clear();



	// �Z�O�����g�����v�Z
	std::vector<MyLib::Vector3> vecpos = CalSegmentDivision(m_vecSegmentPosition);

	// pos�̗v�f�n��
	std::transform(vecpos.begin(), vecpos.end(), std::back_inserter(m_vecVtxInfo), MyConvert::Vector3ToVtxInfo);


	//// �e���_�i�[
	//m_courceLength = 0.0f;
	//for (int i = 0; i < segmentSize; i++)
	//{
	//	float distance = 0.0f;

	//	while (1)
	//	{
	//		distance += m_fCreateDistance;

	//		if (distance >= vecLength[i])
	//		{
	//			m_courceLength += m_fCreateDistance - (distance - vecLength[i]);

	//			distance = vecLength[i];

	//			m_vecVtxInfo.push_back(VtxInfo(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, m_courceLength, 20.0f), 0.0f));
	//			break;
	//		}

	//		m_courceLength += m_fCreateDistance;
	//		m_vecVtxInfo.push_back(VtxInfo(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, m_courceLength), 0.0f));
	//	}
	//}
}

//==========================================================================
// �Z�O�����g�����v�Z
//==========================================================================
std::vector<MyLib::Vector3> CCourse::CalSegmentDivision(const std::vector<MyLib::Vector3>& segment)
{
	// �Z�O�����g�̒������v�Z
	int segmentSize = segment.size();
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
		vecLength[i] = segment[i].Distance(segment[next]);
	}

	// �e���_�i�[
	float courceLength = 0.0f;
	std::vector<MyLib::Vector3> vecpos;

	for (int i = 0; i < segmentSize; i++)
	{
		float distance = 0.0f;

		while (1)
		{
			distance += m_fCreateDistance;

			if (distance >= vecLength[i])
			{
				courceLength += m_fCreateDistance - (distance - vecLength[i]);

				distance = vecLength[i];

				vecpos.push_back(MySpline::GetSplinePosition_NonLoop(segment, courceLength, 20.0f));
				break;
			}

			courceLength += m_fCreateDistance;
			vecpos.push_back(MySpline::GetSplinePosition_NonLoop(segment, courceLength));
		}
	}

	return vecpos;
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
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// ��ސݒ�
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// �e���_�v�Z
	CalVtxPosition();

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 0.5f, 0.0f));				// �ʒu
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_vecVtxInfo.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();


	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	MyLib::AABB aabb(-25.0f, 25.0f);
	for (const auto& vtx : m_vecSegmentPosition)
	{
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
	}

}

//==========================================================================
// ���Z�b�g
//==========================================================================
void CCourse::ReCreateVtx()
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

	// �ŏ��ƍŌ������
	m_vecSegmentPosition.erase(m_vecSegmentPosition.begin());
	m_vecSegmentPosition.pop_back();

	// �e���_�v�Z
	CalVtxPosition();

	// �e��ϐ�������
	SetPosition(MyLib::Vector3(0.0f, 10.0f, 0.0f));				// �ʒu
	SetWidthBlock(1);		// ������
	SetHeightBlock(static_cast<int>(m_vecVtxInfo.size()) - 1);	// �c����
	SetWidthLen(0.0f);		// �c����
	SetHeightLen(0.0f);		// ������

	// �I�u�W�F�N�g3D���b�V���̏���������
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// ���_���ݒ�
	SetVtx();


	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	MyLib::AABB aabb(-25.0f, 25.0f);
	for (const auto& vtx : m_vecSegmentPosition)
	{
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
	}
}

//==========================================================================
// �����̒��_���W�v�Z
//==========================================================================
void CCourse::CalBothVtxPosition()
{
#if 0
	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;


	int segmentSize = static_cast<int>(m_vecSegmentPosition.size());

	std::vector<MyLib::Vector3> vecLeft, vecRight;
	MyLib::Vector3 rot;

	// ���E�̃Z�O�����g�v�Z
	for (int y = 0; y < segmentSize; y++)
	{
		// �C���f�b�N�X�v�Z
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		// �e��}�g���b�N�X������
		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �������f
		int next = (y + 1) % segmentSize;

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		// ���̃Z�O�����g�Ƃ̌����擾
		rot.y = m_vecSegmentPosition[next].AngleXZ(m_vecSegmentPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{// �I�[�̂�
			rot.y *= -1;
		}

		// ��]���f
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecSegmentPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);




		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		// ���_���W���
		vecLeft.push_back(mtxLeft.GetWorldPosition());
		vecRight.push_back(mtxRight.GetWorldPosition());

		CEffect3D::Create(
			vecLeft.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			vecRight.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	}


	// ���_���W
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int idx = 0;

	// �Z�O�����g�����v�Z
	std::vector<MyLib::Vector3> vecpos;

	// ��
	vecpos.clear();
	vecpos = CalSegmentDivision(vecLeft);
	idx = 0;
	for (int i = 0; i < static_cast<int>(vecpos.size()) - 1; i++)
	{
		// ���_���W���
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

	// �E
	vecpos.clear();
	vecpos = CalSegmentDivision(vecRight);
	idx = 1;
	for (int i = 0; i < static_cast<int>(vecpos.size()); i++)
	{
		// ���_���W���
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}
#else

	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;


	int segmentSize = static_cast<int>(m_vecSegmentPosition.size());

	std::vector<MyLib::Vector3> vecLeft, vecRight;
	MyLib::Vector3 rot;

	// ���E�̃Z�O�����g�v�Z
	for (int y = 0; y < segmentSize; y++)
	{
		// �C���f�b�N�X�v�Z
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		// �e��}�g���b�N�X������
		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �������f
		int next = (y + 1) % segmentSize;

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		// ���̃Z�O�����g�Ƃ̌����擾
		rot.y = m_vecSegmentPosition[next].AngleXZ(m_vecSegmentPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{// �I�[�̂�
			rot.y *= -1;
		}

		// ��]���f
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecSegmentPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);




		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		// ���_���W���
		vecLeft.push_back(mtxLeft.GetWorldPosition());
		vecRight.push_back(mtxRight.GetWorldPosition());

		CEffect3D::Create(
			vecLeft.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			vecRight.back() + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	}


	// ���_���W
	MyLib::Vector3* pVtxPos = GetVtxPos();
	int idx = 0;

	// �Z�O�����g�����v�Z
	std::vector<MyLib::Vector3> vecpos;

	// ��
	vecpos.clear();
	vecpos = CalSegmentDivision(vecLeft);
	idx = 0;
	for (int i = 0; i < GetNumVertex() / 2; i++)
	{
		// ���_���W���
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

	// �E
	vecpos.clear();
	vecpos = CalSegmentDivision(vecRight);
	idx = 1;
	for (int i = 0; i < GetNumVertex() / 2; i++)
	{
		// ���_���W���
		pVtxPos[idx] = vecpos[i];

		idx += WIDTH_BLOCK;
	}

#endif

}

//==========================================================================
// �X�V����
//==========================================================================
void CCourse::Update()
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


	// ���_���W
	SetVtx();
}

//==========================================================================
// ���_���W
//==========================================================================
void CCourse::SetVtxPosition()
{
#if 0
	// �v�Z�p�ϐ�
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();

	MyLib::Vector3 beforeVec = MyLib::Vector3(0.0f, 0.0f, 1.0f);

	for (int y = 0; y < static_cast<int>(m_vecVtxInfo.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// �������f
		int next = (y + 1) % static_cast<int>(m_vecVtxInfo.size());

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		m_vecVtxInfo[y].rot.y = m_vecVtxInfo[next].pos.AngleXZ(m_vecVtxInfo[y].pos);
		UtilFunc::Transformation::RotNormalize(m_vecVtxInfo[y].rot.y);

		if (bEnd)
		{
			m_vecVtxInfo[y].rot.y *= -1;
		}

		// ��]���f
		mtxRotate.RotationYawPitchRoll(m_vecVtxInfo[y].rot.y, m_vecVtxInfo[y].rot.x, m_vecVtxInfo[y].rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// �ʒu���f
		mtxTrans.Translation(m_vecVtxInfo[y].pos);
		mtxParent.Multiply(mtxParent, mtxTrans);

		if (y == 1)
		{// ����X�L�b�v
			CEffect3D::Create(
				m_vecVtxInfo[y].pos + GetPosition(),
				0.0f,
				D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
				20.0f, 4, CEffect3D::MOVEEFFECT::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
		}

		// �I�t�Z�b�g���f
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		//mtxLeft.Multiply(mtxLeft, mtxRotate);
		ImGui::Text("x:%f y:%f z:%f, rot.y:%f", m_vecVtxInfo[y].pos.x, m_vecVtxInfo[y].pos.y, m_vecVtxInfo[y].pos.z, m_vecVtxInfo[y].rot.y);

		// ���_���W���
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();


		if (y > 0)
		{// ����X�L�b�v

			int up = idx - WIDTH_BLOCK, down = nextidx - WIDTH_BLOCK;
			int before = y - 1;



			// �x�N�g��AB��90�x�����v���ɉ�]
			MyLib::Vector3 rotatedB = (m_vecVtxInfo[before].pos - m_vecVtxInfo[y].pos).rotateAroundY();

			// �V�����x�N�g���̏I�_���v�Z
			MyLib::Vector3 newB = m_vecVtxInfo[y].pos + rotatedB;


			MyLib::Vector3 AB = newB - m_vecVtxInfo[y].pos;
			MyLib::Vector3 AP = pVtxPos[idx] - m_vecVtxInfo[y].pos;

			// ����̓_���O��̂������ɂ������瓯�n�_�ɒu��
			// ��
			float crossProductY = AB.x * AP.z - AB.z * AP.x;
			if (crossProductY > 0)
			{
				pVtxPos[idx] = pVtxPos[up];

				CEffect3D::Create(
					pVtxPos[idx] + GetPosition(),
					0.0f,
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
					80.0f, 2, CEffect3D::MOVEEFFECT::MOVEEFFECT_GENSUI, CEffect3D::TYPE_NORMAL);
			}

			AP = pVtxPos[nextidx] - m_vecVtxInfo[y].pos;

			// ��
			crossProductY = AB.x * AP.z - AB.z * AP.x;
			if (crossProductY > 0)
			{
				pVtxPos[nextidx] = pVtxPos[down];
			}

		}



	}

#else
	// �����̒��_���W�v�Z
	CalBothVtxPosition();
#endif

	int i = 0;
	MyLib::Vector3 fieldpos = GetPosition();
	for (const auto& vtx : m_vecSegmentPosition)
	{
		MyLib::Vector3 setpos = vtx + fieldpos;
		m_pCollisionLineBox[i]->SetPosition(setpos);
		i++;
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
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3 vec1, vec2, nor = MyLib::Vector3(0.0f, 1.0f, 0.0f);
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	int heightBlock = static_cast<int>(m_vecVtxInfo.size());

	// �e�N�X�`�����
	float posU_UP = 0.0f, posU_Down = 0.0f, posV = 0.0f;
	int texID = CTexture::GetInstance()->Regist(TEXTURE);
	float intervalV = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU).y;
	int idx = 0, nextIdx = 0;

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// �c�̕������J��Ԃ�

		// ���Z�b�g
		posV = 0.0f;

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
		{// ���̕������J��Ԃ�

			// ����̒��_�C���f�b�N�X
			idx = nCntWidth + (nCntHeight * (nWidth + 1));

			// �ׂ̃C���f�b�N�X
			nextIdx = idx + 2;

			// UV���W�ݒ�
			if (nCntWidth == 0)
			{
				pTex[idx] = D3DXVECTOR2(posU_UP + m_fTexU, posV + m_fTexV);


				// ���̊������i�߂�
				if (nextIdx < vtxNum)
				{
					posU_UP += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 2]) / INTERVAL_TEXU;
				}
				
			}
			else
			{
				pTex[idx] = D3DXVECTOR2(posU_Down + m_fTexU, posV + m_fTexV);

				// ���̊������i�߂�
				if (nextIdx < vtxNum)
				{
					posU_Down += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 2]) / INTERVAL_TEXU;
				}
			}

			// �c�̊������i�߂�
			posV += pVtxPos[idx].DistanceXZ(pVtxPos[idx + 1]) / intervalV;
			
#if 0
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
#endif
			pVtxNor[idx] = nor;
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

		// ��O����
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 0.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 0.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 500.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1000.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1800.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1800.0f });

		MyLib::AABB aabb(-25.0f, 25.0f);

		for (int i = 0; i < static_cast<int>(m_vecSegmentPosition.size()); i++)
		{
			m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		}

		// �I�u�W�F�N�g3D���b�V���̏���������
		Reset();

		Save();
		return E_FAIL;
	}

	// �\���̂̃T�C�Y���擾
	std::streamsize structSize = sizeof(MyLib::Vector3);
	
	// �t�@�C���̖����܂Ńf�[�^��ǂݍ���
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);
	
	// �f�[�^�̌����v�Z
	size_t numVectors = fileSize / structSize;
	
	// �x�N�g���̔z���p��
	m_vecSegmentPosition.resize(numVectors);
	
	// �t�@�C������f�[�^��ǂݍ���
	File.read(reinterpret_cast<char*>(m_vecSegmentPosition.data()), fileSize);

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

	std::vector<MyLib::Vector3> savedata = m_vecSegmentPosition;
	savedata.erase(savedata.begin());
	savedata.pop_back();


	// �f�[�^���o�C�i���t�@�C���ɏ����o��
	File.write(reinterpret_cast<char*>(savedata.data()), savedata.size() * sizeof(MyLib::Vector3));
		
	// �t�@�C�������
	File.close();
}

//==========================================================================
// �����蔻��{�b�N�X�擾
//==========================================================================
CCollisionLine_Box* CCourse::GetCollisionLineBox(int idx)
{
	if (static_cast<int>(m_pCollisionLineBox.size()) <= idx) return nullptr;

	return m_pCollisionLineBox[idx];
}

//==========================================================================
// �e���_���̈ʒu�擾
//==========================================================================
CCourse::VtxInfo CCourse::GetVecVtxinfo(int idx)
{
	if (static_cast<int>(m_vecVtxInfo.size()) <= idx) return VtxInfo();

	return m_vecVtxInfo[idx];
}

//==========================================================================
// �e���_���̈ʒu�ݒ�
//==========================================================================
void CCourse::SetVecVtxinfo(int idx, const VtxInfo& vecinfo)
{
	if (static_cast<int>(m_vecVtxInfo.size()) <= idx) return;

	m_vecVtxInfo[idx] = vecinfo;
}

//==========================================================================
// ��_�̈ʒu�擾
//==========================================================================
MyLib::Vector3 CCourse::GetVecPosition(int idx)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecSegmentPosition[idx];
}

//==========================================================================
// ��_�̈ʒu�ݒ�
//==========================================================================
void CCourse::SetVecPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return;

	m_vecSegmentPosition[idx] = pos;
}
