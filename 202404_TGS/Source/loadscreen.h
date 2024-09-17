//=============================================================================
// 
//  �t�F�[�h�w�b�_�[ [loadscreen.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _LOADSCREEN_H_
#define _LOADSCREEN_H_	// ��d�C���N���[�h�h�~

#include "main.h"
#include "object2D_Anim.h"
#include "scene.h"

//==========================================================================
// �O���錾
//==========================================================================

//==========================================================================
// �N���X��`
//==========================================================================
// �t�F�[�h�N���X��`
class CLoadScreen
{
public:

	CLoadScreen();
	~CLoadScreen();

	// �����o�֐�
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void Kill();

	CObject2D_Anim*GetMyObject();
	static CLoadScreen *Create();

private:

	CObject2D* m_apObj2D[10];	// ������
};

#endif