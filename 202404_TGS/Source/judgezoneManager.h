//=============================================================================
// 
//  ����]�[���}�l�[�W���w�b�_�[ [judgezoneManager.h]
//  Author : �Ό��D�n
// 
//=============================================================================
#ifndef _JUDGE_ZONE_MANAGER_H_
#define _JUDGE_ZONE_MANAGER_H_		// ��d�C���N���[�h�h�~
#include "listmanager.h"
#include "judgezone.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ����]�[���}�l�[�W���N���X
class CJudgeZoneManager
{
public:
	CJudgeZoneManager();
	~CJudgeZoneManager();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// �����o�֐�
	//=============================
	void Check(float progress);
	void Add(CJudgeZone* zone) { m_zoneList.Regist(zone); }
	void Release();		//��������������]�[����j��
	void ReleaseAll();	//����]�[�������ׂĔj��

	//=============================
	// �ÓI�֐�
	//=============================
	static CJudgeZoneManager* Create();
	static CJudgeZoneManager* GetInstance() { return m_ThisPtr; }	// �C���X�^���X�擾

private:
	CListManager<CJudgeZone> m_zoneList;
	static CJudgeZoneManager* m_ThisPtr;		// ���g�̃|�C���^
};

#endif