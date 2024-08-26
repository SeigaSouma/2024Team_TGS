//=============================================================================
// 
//  ���w�b�_�[ [kite.h]
//  Author : IbukiOkusada
// 
//=============================================================================

#ifndef _KITE_H_
#define _KITE_H_	// ��d�C���N���[�h�h�~

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"
#include "people.h"

// �O���錾
class CShadow;

//==========================================================================
// �N���X��`
//==========================================================================
// �l�N���X
class CKite : public CPeople
{
public:


	CKite(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CKite();

	// �I�[�o�[���C�h���ꂽ�֐�
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// �폜

	static CKite* Create(const std::string& pFileName, MyLib::Vector3 pos);

protected:

private:

	MyLib::Vector3 m_StartRot;	// �J�n����
};



#endif