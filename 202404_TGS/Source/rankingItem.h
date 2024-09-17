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
		ITEM_TIME,
		ITEM_ALLRANK_DATA,
		ITEM_YOU,
		ITEM_MAX
	};

	CRankingItem(int nPriority = 11);
	~CRankingItem();

	// �I�[�o�[���C�h���ꂽ�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	virtual MyLib::Vector3 GetPos();
	virtual void SetMove(MyLib::Vector3 move);	//�����L���O�A�C�e���̈ړ��l�Z�b�g
	void Kill();	// �폜
	static CRankingItem* Create(int nNumRank,int nTime,int nAllRank, MyLib::Vector3 posAll,float fSize,bool bNewRecord);	// ��������

private:

	
	/*void SetScoreData()*/
	//=============================
	// �����o�ϐ�
	//=============================
	float m_fStateTime;
	CObject2D* m_pItem[ITEM_MAX];	//���ڐ�
	CObject2D* m_pScoreItem[6];		//�X�R�A�̌���obj
	int m_nNumRank;					//����
	int m_nTime;					//�X�R�A�l
	int m_nAllRank;					//���]�̒l
	static int m_nObjNum;			//���I�u�W�F�N�g�̐�����

	MyLib::Vector3 m_posAll;		//�S���ڂ̊�ʒu
	MyLib::Vector3 m_posItem[ITEM_MAX];//�e���ڂ̈ʒu
	MyLib::Vector3 m_moveAll;		//�ړ��l
	float m_fSize;					//�g�嗦

	bool m_bUpdate;					//�X�V���ꂽ�X�R�A���H
};


#endif