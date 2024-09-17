//======================================================
//
//�I�u�W�F�N�g�i�r���{�[�h�j�����̃w�b�_[objectBillboard.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECT_BILLBOARD_H_	//���d�C���N���[�h�΍�
#define _OBJECT_BILLBOARD_H_

#include "..\object_3d\object3D.h"

//�r���{�[�h�N���X
class CObjectBillboard : public CObject3D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CObjectBillboard(int nPriority = PRIORITY_DEFAULT);		//�f�t�H���g
	virtual ~CObjectBillboard();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//����
	static CObjectBillboard* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nIdxTexture);
};

#endif	//���d