//======================================================
//
//���U���g�V�[���̃w�b�_[result.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _RESULT_H_
#define _RESULT_H_
#include "..\..\manager\manager.h"

//�O���錾
class CPause;
class CFade;

//��
class CGoal;

//�^�C�g���N���X
class CResult : public CScene
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CResult();
	~CResult();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CPause* m_pPause;	//�|�[�Y
	CFade* m_pFade;		//�t�F�[�h
};

#endif // !_GAME_H_
