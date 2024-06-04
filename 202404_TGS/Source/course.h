//=============================================================================
// 
//  �R�[�X�w�b�_�[ [course.h]
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
// �R�[�X�N���X
class CCourse : public CObject3DMesh
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
	};

	// �ӏ��
	struct Line
	{
		LineInfo lineInfo;	// �ӂ̓������
	};

	CCourse(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CCourse();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Reset();	// ���Z�b�g

	// ���擾�E�ݒ�
	std::vector<LineInfo> GetLineInfo() { return m_LineInfo; };						// �ӏ��擾
	LineInfo GetLineInfo(int idx);													// �ӏ��擾
	void SetLineInfo(const std::vector<LineInfo>& vecinfo) { m_LineInfo = vecinfo; }// �ӏ��ݒ�
	void SetLineInfo(int idx, const LineInfo& info);								// �ӏ��ݒ�
	void PushLineInfo();															// �ӏ��ǉ�
	void PopLineInfo();																// �ӏ��폜

	// �t�@�C������
	void Save();	// �Z�[�u
	HRESULT Load(const std::string& file);	// ���[�h

	static CCourse *Create(const std::string& file);	// ����

private:

	// �����o�֐�
	void SetVtxPosition();	// ���_���W

	// �����o�ϐ�
	std::string m_FileName;	// �t�@�C����
	std::vector<LineInfo> m_LineInfo;	// �ӏ��
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// �����蔻��{�b�N�X

};



#endif