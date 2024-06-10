//=============================================================================
// 
//  ���w�b�_�[ [waterfield.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _WATERFIELD_H_
#define _WATERFIELD_H_	// ��d�C���N���[�h�h�~

#include "object3DMesh.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ���N���X��`
class CWaterField : public CObject3DMesh
{
public:

	CWaterField(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CWaterField();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	static CWaterField *Create();

private:

	float m_fTexU;	// U�X�N���[���p
	float m_fTexV;	// V�X�N���[���p
};



#endif