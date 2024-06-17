//=============================================================================
// 
//  ����]�[���w�b�_�[ [judgezone.h]
//  Author : �Ό��D�n
// 
//=============================================================================
#ifndef _JUDGE_ZONE_H_
#define _JUDGE_ZONE_H_		// ��d�C���N���[�h�h�~
#include "listmanager.h"
#include "judge.h"

//==========================================================================
// �N���X��`
//==========================================================================
//**************************************************************************
// ����]�[���S��
//**************************************************************************
class CJudgeZone
{
public:
	// �����ޗ�
	enum JUDGETYPE
	{
		TYPE_NONE = 0,
		TYPE_HITNUM,
	};

	// �n�_�I�_�\����
	struct SZone
	{
		float start;	// �n�_
		float end;	// �I�_
	};

	// ������\����
	struct SJudgeInfo
	{
		JUDGETYPE type;
		CJudge::JUDGE judge;
		int hitnum;
	};

	CJudgeZone();
	virtual ~CJudgeZone();

	//=============================
	// �I�[�o�[���C�h�֐�
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();

	//=============================
	// �����o�֐�
	//=============================
	virtual void Check();
	virtual CJudge::JUDGE Judge();

	// �擾�E�ݒ�
	SZone GetZone() { return m_zone; }
	void SetZone(const SZone zone) { m_zone = zone; }
	void SetZone(const float start, const float end);
	bool IsEnable() { return m_isEnable; }

	//=============================
	// �ÓI�֐�
	//=============================
	static CJudgeZone* Create(const float start, const float end);	// ����

private:
	SZone m_zone;
	CJudge* m_pJudge;
	bool m_isEnable;
	SJudgeInfo m_aJudgeInfo[2];
};

#endif