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
#include <map>

//==========================================================================
// �N���X��`
//==========================================================================
//**************************************************************************
// ����]�[���S��
//**************************************************************************
class CJudgeZone
{
public:
	// �n�_�I�_�\����
	struct SZone
	{
		float start;	// �n�_
		float end;	// �I�_
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
	void SetBorder(const float borderHeight) { m_borderHeight = borderHeight; }
	void SetInfo(CJudge::BORDER border, CJudge::SJudgeInfo info) { m_aJudgeInfo[border] = info; }
	bool IsEnable() { return m_isEnable; }

	//=============================
	// �ÓI�֐�
	//=============================
	static CJudgeZone* Create(const float start, const float end, const float borderHeight);	// ����

private:
	SZone m_zone;
	CJudge* m_pJudge;
	bool m_isEnable;
	CJudge::SJudgeInfo m_aJudgeInfo[2];
	float m_borderHeight;
};

#endif