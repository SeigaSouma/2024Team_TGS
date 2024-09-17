//======================================================
//
//�I�u�W�F�N�g�iX�j�����̃w�b�_[objectX.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "..\..\manager\manager.h"
#include "..\object_empty\object.h"
#include "..\..\collision\collision.h"
#include "..\..\utility\Utility.h"

//�O���錾
class CXModel;

//�I�u�W�F�N�g�N���X
class CObjectX : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObjectX(int nPriority = PRIORITY_DEFAULT);		//�f�t�H���g
	virtual ~CObjectX();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObjectX* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	float GetDepth(void) { return m_fDepth; }
	static CObjectX* GetTop(void) { return m_pTop; }
	CObjectX* GetNext(void) { return m_pNext; }
	CXModel* GetModel(void) { return m_pModel; }
	CObject* GetObj(void) { return this; }
	D3DXVECTOR3 GetMove(void) { return IS_Utility::VEC3_ZERO; }

	//�ݒ�
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetModel(CXModel* pModel);

	//�g�p���f���P�ʂŏ���
	static void Delete(CXModel* pTarget);

	//���X�g���O
	virtual void Exclusion(void);

protected:
	void SetColor(const bool bEnable, const D3DXCOLOR col = D3DXCOLOR(0xffffffff));
	D3DXMATRIX* GetMtx() { return &mtxWorld; }


private:
	//���f��
	D3DXMATRIX mtxWorld;		//���[���h�}�g��
	CXModel* m_pModel;			//���f��
	D3DXCOLOR m_changeColor;	//�ύX�����F
	bool m_bEnable;				//�F�ύX�L������

	//�ʒu��
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��
	float m_fHeight;	//����
	float m_fDepth;		//���s

	//���X�g
	static CObjectX* m_pTop;	//�擪�I�u�W�F�N�g
	static CObjectX* m_pCur;	//�Ō���I�u�W�F�N�g
	CObjectX* m_pNext;			//���̃I�u�W�F�N�g
	CObjectX* m_pPrev;			//�O�̃I�u�W�F�N�g
	bool m_bExclusion;			//���O�t���O
	static int m_nNumAll;		//����
};

#endif // !_OBJECT_H_
