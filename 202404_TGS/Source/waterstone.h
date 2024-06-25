//=============================================================================
// 
//  �����΃w�b�_�[ [waterstone.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _WATERSTONE_H_
#define _WATERSTONE_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "waterstoneManager.h"

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
	void SetStoneInfo(const CWaterStone_Manager::SStoneInfo& info) { m_StoneInfo = info; }	// �Ώ��ݒ�
	CWaterStone_Manager::SStoneInfo GetStoneInfo() { return m_StoneInfo; }	// �Ώ��擾

	//=============================
	// �ÓI�֐�
	//=============================
	/**
	@brief		��������
	@details	�K�v������Έ����ǉ�
	*/
	static CWaterStone* Create(const CWaterStone_Manager::SStoneInfo& info);
	static CListManager<CWaterStone> GetListObj() { return m_List; }	// ���X�g�擾

private:

	//=============================
	// �����o�֐�
	//=============================


	//=============================
	// �����o�ϐ�
	//=============================
	float m_fSplashTimer;		// ���Ԃ��^�C�}�[
	float m_fIntervalSplash;	// ���Ԃ��܂ł̃C���^�[�o��
	CWaterStone_Manager::SStoneInfo m_StoneInfo;	// �Ώ��
	static CListManager<CWaterStone> m_List;		// ���X�g
};


#endif