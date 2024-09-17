//======================================================
//
//�L�����̃p�[�c�����̃w�b�_[charaparts.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _CHARA_PARTS_H_
#define _CHARA_PARTS_H_
#include "../../_object/object_x/objectX.h"

//�O���錾
class CXModel;

//�L�����̃p�[�c�N���X
class CCharaParts
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CCharaParts();
	~CCharaParts();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CCharaParts* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//�擾
	D3DXMATRIX GetMtx() { return m_mtxWorld; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }

	//�ݒ�
	void SetParent(CCharaParts* pModel) { m_pParent = pModel; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosMotioned(D3DXVECTOR3 pos) { m_posMotioned = pos; }
	void SetRotMotioned(D3DXVECTOR3 rot) { m_rotMotioned = rot; }
private:
	CXModel* m_pModel;			//�g�p����X�t�@�C�����f��
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_posMotioned;	//���[�V������̈ʒu
	D3DXVECTOR3 m_rotMotioned;	//���[�V������̌���
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g��
	CCharaParts* m_pParent;		//�e�̃p�[�c
};

#endif // !_MODEL_H_
