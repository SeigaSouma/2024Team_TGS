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

//==========================================================================
// �N���X��`
//==========================================================================
// �}�b�v�G�f�B�^�\�N���X
class CEdit_Map
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

	static CEdit_Map* Create();	// ��������
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

	//=============================
	// �����o�ϐ�
	//=============================
	std::vector<int> m_nModelIdx;				// ���f���C���f�b�N�X
	std::vector<std::string> m_ModelFile;		// ���f���C���f�b�N�X
	std::vector<LPDIRECT3DTEXTURE9> m_pTexture;	// �e�N�X�`���̃|�C���^
	std::vector<CObjectX*> m_pObjX;			// �I�u�W�F�N�gX�̃|�C���^

	LPDIRECT3DTEXTURE9 m_HandleTex[CHandle::HandleType::TYPE_MAX];	// �e�N�X�`���̃|�C���^


	SDragDropData m_DragData;	// �h���b�O���f�[�^
	bool m_bGrab;	// �͂ݔ���
	bool m_bReGrab;	// �Ē͂ݔ���

	CObjectX* m_pCopyObj;				// �R�s�[�I�u�W�F�N�g
	CObjectX* m_pGrabObj;				// �͂݃I�u�W�F�N�g
	CHandle* m_pHandle;					// �ړ��n���h��
	CHandle::HandleType m_HandleType;	// �n���h���̎��
	CHandle::HandleAngle m_moveAngle;	// �ړ��̌���

	static CListManager<CObjectX> m_List;	// ���X�g

};

#endif