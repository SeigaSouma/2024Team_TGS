//=============================================================================
// 
//  �R�[�X�G�f�B�^�\�w�b�_�[ [edit_course.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_COURSE_H_
#define _EDIT_COURSE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �R�[�X�G�f�B�^�\�N���X
class CEdit_Course : public CEdit
{
public:

	CEdit_Course();
	~CEdit_Course();
	
	// �I�[�o�[���C�h�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;

private:

	//=============================
	// �\���̒�`
	//=============================
	struct LineData
	{
		MyLib::Vector3 pos;	// �ʒu
		MyLib::Vector3 rot;	// ����
		float width;		// ��
	};

	// �����o�֐�
	void FileControl();		// �t�@�C������
	void ChangeEditCourse();	// �ҏW����R�[�X�ύX
	void SelectLine();		// ���C���I��
	void DragLine();		// ���C���͂�
	void Transform();		// �g�����X�t�H�[��
	void ChangeLineNum();	// �ӂ̐��ύX
	void ChangeMode();		// ���[�h�ύX
	void TransformBeginEnd();	// �ŏ��ƍŌ�ό`
	void AddPoint();		// �n�_�ǉ�

	LineData m_lineData;	// �ӏ��
	int m_nCourseEditIdx;		// ���삷��C���f�b�N�X�ԍ�
	int m_nVtxEditIdx;			// ���삷��C���f�b�N�X�ԍ�
	bool m_bEdit;			// ���쒆����
	bool m_bDrag;			// �͂ݔ���
	bool m_bHoverWindow;	// �}�E�X�̃E�B���h�E�z�o�[����
	bool m_bSetMode;		// �ݒ胂�[�h����
	bool m_bAutoCreateMode;		// ������������
};


#endif