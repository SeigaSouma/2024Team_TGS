//=============================================================================
// 
//  �����΃w�b�_�[ [waterstone.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _WATERSTONE_H_
#define _WATERSTONE_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����΃N���X
class CWaterStone : public CObjectX
{
public:
	
	CWaterStone(int nPriority = 6);
	~CWaterStone();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// �����o�֐�
	//=============================
	void Kill();		// �폜

	//=============================
	// �ÓI�֐�
	//=============================
	/**
	@brief		��������
	@details	�K�v������Έ����ǉ�
	*/
	static CWaterStone *Create(const MyLib::Vector3& pos);

private:

	//=============================
	// �����o�֐�
	//=============================


	//=============================
	// �����o�ϐ�
	//=============================
	float m_fSplashTimer;		// ���Ԃ��^�C�}�[
	float m_fIntervalSplash;	// ���Ԃ��܂ł̃C���^�[�o��
};


#endif