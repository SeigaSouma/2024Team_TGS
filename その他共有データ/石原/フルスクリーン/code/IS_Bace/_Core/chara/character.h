//======================================================
//
//�L�����N�^�[�����̃w�b�_[character.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//�K�v�Ȃ̃C���N���[�h
#include "..\_object\object_empty\object.h"
#include "characterconfig.h"

//�O���錾
class CCharaParts;
class CMotion;

//�L�����N�^�[�N���X
class CCharacter : public CObject
{
public:
	//���[�V�������
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		MOTIONTYPE_ACTION,	//������
		MOTIONTYPE_JUMP,
		MOTIONTYPE_LAND,
	} MOTIONTYPE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CCharacter();
	~CCharacter();

	//��{����
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//�擾
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }

	//�ݒ�
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetChara(CCharacterConfig::SCharacterConfig &charaConfig);

	//����
	static CCharacter* Create(CCharacterConfig::SCharacterConfig &charaConfig, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	static CCharacter* Create(std::string path, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

	//���X�g�擾
	static CCharacter* GetTop(void) { return m_pTop; }
	CCharacter* GetNext(void) { return m_pNext; }

	//���O�i�̂��ɏ�����j
	void Exclusion() {}

private:
	//���X�g
	static CCharacter* m_pTop;		//�擪�I�u�W�F�N�g
	static CCharacter* m_pCur;		//�Ō���I�u�W�F�N�g
	CCharacter* m_pNext;			//���̃I�u�W�F�N�g
	CCharacter* m_pPrev;			//�O�̃I�u�W�F�N�g
	bool m_bExclusion;				//���O�t���O
	static int m_nNumAll;			//����

	CCharacterConfig::SCharacterConfig m_charaConfig;
	CCharaParts** m_ppCharaParts;	//�L�����p�[�c
	int m_nNumParts;				//�p�[�c��
	CMotion* m_pMotion;				//���[�V�����|�C���^
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g��

	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_rot;	//����

};

#endif // !_CHARACTER_H_
