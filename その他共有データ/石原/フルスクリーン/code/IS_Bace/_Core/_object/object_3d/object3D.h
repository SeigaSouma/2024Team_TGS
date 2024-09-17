//======================================================
//
//�I�u�W�F�N�g�i3D�j�����̃w�b�_[object3D.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECT3D_H_	//���d�C���N���[�h�΍�
#define _OBJECT3D_H_
#include "..\object_empty\object.h"

//�I�u�W�F�N�g�N���X
class CObject3D : public CObject
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObject3D(int nPriority = PRIORITY_DEFAULT);	//�f�t�H���g
	virtual ~CObject3D();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority = PRIORITY_DEFAULT);

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetDepth(void) { return m_fDepth; }
	int GetIdxTexture(void) { return m_nIdxTexture; }

	//�e�N�X�`���ݒ�
	void BindTexture(int nIdx) { m_nIdxTexture = nIdx; }

	//�ݒ�
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }	//�ʒu
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }	//����

	//���O
	virtual void Exclusion(void){}
protected:
	HRESULT SetCol(const D3DXCOLOR col);	//�F
	void SetSize(const float fWidth, const float fDepth);
	HRESULT SetTex(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex3);	//�e�N�X�`�����W
	HRESULT SetTex(const int nPatternU, const int nPatternV);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() { return m_pVtxbuff; }
	D3DXMATRIX* GetMtxWorld() { return &m_mtxWorld; }

private:
	void CulcNor(VERTEX_3D* pVtx);

	int m_nIdxTexture;					//�e�N�X�`���ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;	//���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;				//���[���h�}�g��

	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����
	float m_fWidth;		//��(X)
	float m_fDepth;		//���s(Z)
};

#endif	//���d