//=============================================================================
// 
//  �����L���O2D�w�b�_�[[rankingItem.h]
//  Author : ���������Y
// 
//=============================================================================

#ifndef _RANKINGITEM_H_
#define _RANKINGITEM_H_	// ��d�C���N���[�h�h�~

#include "object.h"

//==========================================================================
// �N���X��`
//==========================================================================
// ��ʃN���X
class CRankingItem : public CObject
{
public:

	//�񋓌^
	typedef enum Items
	{
		ITEM_NUM_RANK = 0,
		ITEM_ALLRANK,
		ITEM_MAX
	};

	CRankingItem(int nPriority = 11);
	~CRankingItem();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void Kill();	// �폜
	static CRankingItem* Create();	// ��������

private:

	
	/*void SetScoreData()*/
	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;
	CObject2D* m_pItem[ITEM_MAX];
};


#endif