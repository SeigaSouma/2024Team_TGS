//=============================================================================
// 
//  �Ί_�w�b�_�[ [stonewall.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _COURSE_H_
#define _COURSE_H_	// ��d�C���N���[�h�h�~

#include "object3DMesh.h"
#include "collisionLine_Box.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �Ί_�N���X
class CStoneWall : public CObject3DMesh
{

public:

	//=============================
	// �\���̒�`
	//=============================
	// �ӂ̓������
	struct LineInfo
	{
		MyLib::Vector3 pos;	// �ʒu
		MyLib::Vector3 rot;	// ����
		float width;		// ��

		LineInfo() : pos(0.0f), rot(0.0f), width(0.0f) {}
		LineInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot, float _width) : pos(_pos), rot(_rot), width(_width) {}
	};

	// �ӏ��
	struct Line
	{
		LineInfo lineInfo;	// �ӂ̓������
	};

	CStoneWall(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CStoneWall();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Reset();	// ���Z�b�g
	void ReCreateVtx();	// ���Z�b�g
	void CalVtxPosition();	// �e���_�v�Z

	// ���擾�E�ݒ�
	CCollisionLine_Box* GetCollisionLineBox(int idx);								// �����蔻��{�b�N�X�擾
	std::vector<MyLib::Vector3> GetVecPosition() { return m_vecSegmentPosition; }	// ��_�̈ʒu�擾
	MyLib::Vector3 GetVecPosition(int idx);											// ��_�̈ʒu�擾
	void SetVecPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecSegmentPosition = vecpos; }
	void SetVecPosition(int idx, const MyLib::Vector3& pos);						// ��_�̈ʒu�ݒ�
	float GetCourceLength() { return m_courceLength; }								// �R�[�X�̒���

	std::vector<MyLib::Vector3> GetVecVtxPosition() { return m_vecVtxPosition; }	// �e���_�̈ʒu�擾
	MyLib::Vector3 GetVecVtxPosition(int idx);										// �e���_�̈ʒu�擾
	void SetVecVtxPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecVtxPosition = vecpos; }
	void SetVecVtxPosition(int idx, const MyLib::Vector3& pos);						// �e���_�̈ʒu�ݒ�

	// �t�@�C������
	void Save();	// �Z�[�u
	HRESULT Load(const std::string& file);	// ���[�h

	static CStoneWall *Create(const std::string& file);	// ����

private:

	// �����o�֐�
	void SetVtxPosition();	// ���_���W

	// �����o�ϐ�
	std::string m_FileName;	// �t�@�C����
	std::vector<MyLib::Vector3> m_vecSegmentPosition;	// ��_�̈ʒu
	std::vector<MyLib::Vector3> m_vecVtxPosition;		// �e���_�̈ʒu
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// �����蔻��{�b�N�X
	float m_courceLength;
};



#endif