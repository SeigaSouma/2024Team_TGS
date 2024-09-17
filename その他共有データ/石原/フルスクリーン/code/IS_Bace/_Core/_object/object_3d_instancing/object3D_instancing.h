//======================================================
//
//�|���S���i3D,�C���X�^���V���O����j�����̃w�b�_[object3D_instancing.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECT3D_INSTANCING_H_	//���d�C���N���[�h�΍�
#define _OBJECT3D_INSTANCING_H_
#include "..\object_empty\object.h"
#include <list>

//�I�u�W�F�N�g�N���X
class CObject3D_Instancing : public CObject
{
public:
	//�C���X�^���V���O�p�o�b�t�@�\����
	struct SinglePolyVtx
	{
		float px, py, pz;	//���_���W
		float nx, ny, nz;	//�@��
		float u, v;			//�e�N�X�`�����W
	};

	struct InstanceUVVtx
	{
		float u, v;			//�e�N�X�`�����W
	};

	//�ϐ��Ǘ��p
	struct Instance
	{
		D3DXMATRIX mtx;		//�}�g��
		InstanceUVVtx uv;	//�e�N�X�`�����W
		D3DXCOLOR col;		//�J���[
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CObject3D_Instancing(int nPriority = PRIORITY_04);	//�f�t�H���g
	virtual ~CObject3D_Instancing();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObject3D_Instancing* Create(const float fWidth, const float fHeight, const int nInstanceNum, const int nPriority = PRIORITY_04);

	//�P�|���擾�E�ݒ�
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	HRESULT SetSize(const float fWidth, const float fHeight);		//�T�C�Y
	int GetIdxTexture(void) { return m_nIdxTexture; }
	virtual void BindTexture(int nIdx);
	void SetInstanceNum(const int num);

	//�C���X�^���X�ݒ�
	void AddInstance(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nPatternU, const int nPatternV, D3DXCOLOR col);
	void ResetList() { m_InstanceList.clear(); }

	//���O
	virtual void Exclusion(void){}

private:
	void AddList(Instance ins);
	void SetInstanceBuff();
	void ReleaseInstanceBuff();

	HRESULT SetVtx(void);

	static D3DVERTEXELEMENT9 m_aVtxElem[];
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	LPDIRECT3DVERTEXBUFFER9 m_pSinglePolyVtxBuff;
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pInstanceMtxVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pInstanceUVVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pInstanceColVtxBuff;
	LPD3DXEFFECT m_pEffect;
	float m_fWidth;		//��
	float m_fHeight;	//����
	int m_nIdxTexture;	//�e�N�X�`���ԍ�
	
	std::list<Instance> m_InstanceList;
	int m_nInstanceNum;
};

#endif	//���d