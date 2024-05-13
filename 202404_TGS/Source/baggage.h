//=============================================================================
// 
//  �ו��w�b�_�[ [baggage.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SAMPLE_OBJX_H_
#define _SAMPLE_OBJX_H_		// ��d�C���N���[�h�h�~

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ו��N���X
class CBaggage : public CObjectX
{
public:
	
	//=============================
	// �񋓌^��`
	//=============================
	enum TYPE
	{
		TYPE_CLOTH = 0,	// �z
		TYPE_MAX
	};

	CBaggage(int nPriority = 6);
	~CBaggage();

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
	static CBaggage *Create(TYPE type);
	static CListManager<CBaggage> GetListObj() { return m_List; }	// ���X�g�擾

private:

	
	//=============================
	// �����o�֐�
	//=============================
	// ���̑�

	//=============================
	// �����o�ϐ�
	//=============================
	TYPE m_type;		// ���
	float m_fWeight;	// �d��
	static CListManager<CBaggage> m_List;	// ���X�g

};


#endif