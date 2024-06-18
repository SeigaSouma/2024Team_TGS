//=============================================================================
// 
//  �Ί_�w�b�_�[ [stonewall.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _STONEWALL_H_
#define _STONEWALL_H_	// ��d�C���N���[�h�h�~

#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �Ί_�N���X
class CStoneWall : public CObject3DMesh
{

public:

	CStoneWall(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CStoneWall();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Reset();	// ���Z�b�g
	void CalVtxPosition();	// �e���_�v�Z
	void BindVtxPosition();	// ���_���W���蓖��

	// ���擾�E�ݒ�
	std::vector<MyLib::Vector3> GetVecPosition() { return m_vecSegmentPosition; }	// ��_�̈ʒu�擾
	MyLib::Vector3 GetVecPosition(int idx);											// ��_�̈ʒu�擾
	void SetVecPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecSegmentPosition = vecpos; }
	void SetVecPosition(int idx, const MyLib::Vector3& pos);						// ��_�̈ʒu�ݒ�
	std::vector<MyLib::Vector3> GetVecVtxPosition() { return m_vecVtxPosition; }	// �e���_�̈ʒu�擾
	MyLib::Vector3 GetVecVtxPosition(int idx);										// �e���_�̈ʒu�擾
	void SetVecVtxPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecVtxPosition = vecpos; }
	void SetVecVtxPosition(int idx, const MyLib::Vector3& pos);						// �e���_�̈ʒu�ݒ�

	static CStoneWall *Create();	// ����

private:

	// �����o�ϐ�
	std::vector<MyLib::Vector3> m_vecSegmentPosition;	// ��_�̈ʒu
	std::vector<MyLib::Vector3> m_vecVtxPosition;		// �e���_�̈ʒu
	float m_courceLength;
};



#endif