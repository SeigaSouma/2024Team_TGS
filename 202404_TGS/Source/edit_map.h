//=============================================================================
// 
//  �}�b�v�G�f�B�^�\�w�b�_�[ [edit_map.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _EDIT_MAP_H_
#define _EDIT_MAP_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�G�f�B�^�\�N���X
class CEdit_Map : public CEdit
{
public:

	//=============================
	// �\���̒�`
	//=============================
	struct SDragDropData
	{
		int nType;		// ���
		CObjectX* objX;	// �I�u�W�F�N�gX

		SDragDropData() : nType(0), objX(nullptr) {}
	};

	CEdit_Map();
	~CEdit_Map();
	
	HRESULT Init();
	void Uninit();
	void Update();


	void Save();	// �Z�[�u
	void Load();	// ���[�h
	void Delete(CObjectX* obj);	// �폜
	void Regist(int idx, MyLib::Vector3 pos, MyLib::Vector3 rot, bool bShadow);	// ���蓖��

	static CListManager<CObjectX> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �\���̒�`
	//=============================
	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	//=============================
	// �����o�֐�
	//=============================
	HRESULT ReadXFile();	// �t�@�C���ǂݍ���
	void ChangeHandle();	// �n���h���؂�ւ�

	// �G�f�B�b�g���֐�
	void MenuBar();			// ���j���[�o�[����
	void SetObjectButton();	// �I�u�W�F�N�g�{�^���ݒ�
	void EditImGui();		// ImGui�̑���
	void Grab();			// �͂ݒ�����
	void Remove();			// �Ĉړ�
	void SelectObject();	// �I�u�W�F�N�g�I��

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<int> m_nModelIdx;				// ���f���C���f�b�N�X
	std::vector<LPDIRECT3DTEXTURE9> m_pTexture;	// �e�N�X�`���̃|�C���^
	std::vector<CObjectX*> m_pObjX;			// �I�u�W�F�N�gX�̃|�C���^

	LPDIRECT3DTEXTURE9 m_HandleTex[CHandle::HandleType::TYPE_MAX];	// �e�N�X�`���̃|�C���^


	SDragDropData m_DragData;	// �h���b�O���f�[�^
	bool m_bGrab;			// �͂ݔ���
	bool m_bReGrab;			// �Ē͂ݔ���
	bool m_bHoverWindow;	// �E�B���h�E�̃z�o�[����

	CObjectX* m_pCopyObj;				// �R�s�[�I�u�W�F�N�g
	CObjectX* m_pGrabObj;				// �͂݃I�u�W�F�N�g
	CHandle* m_pHandle;					// �ړ��n���h��
	CHandle::HandleType m_HandleType;	// �n���h���̎��
	CHandle::HandleAngle m_moveAngle;	// �ړ��̌���

	static std::vector<std::string> m_ModelFile;		// ���f���t�@�C��
	static bool m_bLoad;					// �ǂݍ��ݔ���
	static CListManager<CObjectX> m_List;	// ���X�g

};

#endif