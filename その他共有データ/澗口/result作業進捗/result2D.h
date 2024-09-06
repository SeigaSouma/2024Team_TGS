//=============================================================================
// 
//  ���U���g2D�w�b�_�[[result2D.h]
//  Author : ���������Y
// 
//=============================================================================

#ifndef _RESULT2D_H_
#define _RESULT2D_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ��ʃN���X
class CResult2D : public CObject
{
public:

	//�񋓌^
	typedef enum Items
	{
		ITEM_RANK = 0,
		ITEM_TIME,
		ITEM_SCORE,
		ITEM_ALLRANK,
		ITEM_RANKING_UPD,
		ITEM_MAX

	};

	CResult2D(int nPriority = 11);
	~CResult2D();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// �폜
	static CResult2D* Create();	// ��������

private:
	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;
	CObject2D* m_pLogo;
	CObject2D* m_pBg;
	CObject2D* m_pItem[5];
};


#endif