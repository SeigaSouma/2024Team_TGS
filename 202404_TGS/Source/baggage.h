//=============================================================================
// 
//  �ו��w�b�_�[ [baggage.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _BAGGAGE_H_
#define _BAGGAGE_H_		// ��d�C���N���[�h�h�~

#include "objectQuaternion.h"
#include "listmanager.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �ו��N���X
class CBaggage : public CObjectQuaternion
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
	void SetForce(const MyLib::Vector3& power) { m_force = power; }				// �͐ݒ�
	void AddForce(const MyLib::Vector3& power, const MyLib::Vector3& ActPos);	// �͒ǉ�
	bool IsDrop() { return m_bDrop; }	// ��������擾

	//=============================
	// �ÓI�֐�
	//=============================
	static CBaggage *Create(TYPE type);
	static CListManager<CBaggage> GetListObj() { return m_List; }	// ���X�g�擾

private:

	
	//=============================
	// �����o�֐�
	//=============================
	bool Hit();
	// ���̑�

	//=============================
	// �����o�ϐ�
	//=============================
	TYPE m_type;			// ���
	float m_fWeight;		// �d��
	MyLib::Vector3 m_force;	// ��
	MyLib::Vector3 m_velorot;	// ��]���x
	bool m_bDrop;		// ��������
	static CListManager<CBaggage> m_List;	// ���X�g

};


#endif