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
		TYPE_TMP_YMDHOUSE,
		TYPE_TMP_FLOWER,
		TYPE_MAX
	};

	//=============================
	// �ו����
	//=============================
	struct SBaggageInfo
	{
		std::string path;	// ���f���t�@�C���p�X
		float weight;		// �d��
		float width;		// �Ԃꕝ
		float cycle;		// ����

		SBaggageInfo() : path(""), weight(0.0f), width(0.0f), cycle(0.0f) {}
	};

	CBaggage(int nPriority = 4);
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
	bool IsLand() { return m_bLand; }	// ���n����
	void SetModelPath(const std::string path) { m_baggageInfo.path = path; }
	void SetCycle(const float& deviation) { m_baggageInfo.cycle = deviation; }
	void SetWidth(const float& deviWidth) { m_baggageInfo.width = deviWidth; }
	void SetWeight(const float& weight) { m_baggageInfo.weight = weight; }

	//=============================
	// �ÓI�֐�
	//=============================
	static CBaggage* Create(TYPE type, SBaggageInfo info);
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
	bool m_bLand;			// ���n����
	static CListManager<CBaggage> m_List;	// ���X�g
	SBaggageInfo m_baggageInfo;
	float m_fDeviation;
};


#endif