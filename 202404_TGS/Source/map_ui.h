//=============================================================================
//
// �}�b�vUI�w�b�_�[ [map_ui.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _MAP_UI_H_
#define _MAP_UI_H_	// ��d�C���N���[�h�h�~

#include "object.h"

// �O���錾
class CObjectX;
class CCamera;
class CPlayer;

//==========================================================================
// �N���X��`
//==========================================================================
class CMapUI : public CObject
{
private:

	// �g�p�I�u�W�F�N�g�񋓌^
	enum TYPE
	{
		TYPE_MAP = 0,	// �}�b�v
		TYPE_PLAYER,	// �v���C���[
		TYPE_MAX
	};

private:

	CMapUI();	// �R���X�g���N�^
	~CMapUI();	// �f�X�g���N�^

public:

	// �ÓI�����o�֐�
	static CMapUI* Create();	// ����
	static CMapUI* GetInstance() { return m_pInstance; }	// �C���X�^���X�擾
	static void Release();	// �J��
	
	// �����o�֐�
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// �ݒ�
	void BindPlayer(CPlayer* pPlayer) { m_pMyPlayer = pPlayer; }

private:

	// �ÓI�����o�ϐ�
	static CMapUI* m_pInstance;	// �V���O���g���p�C���X�^���X
	static std::string m_aModelList[TYPE::TYPE_MAX];	// �I�u�W�F�N�g�t�@�C����

	// �����o�֐�
	void SetMapRotation();	// �}�b�v�̉�]

	// �����o�ϐ�
	float m_fRatio;			// �S�[���܂ł̊���
	CObjectX* m_apObj[TYPE::TYPE_MAX];	// �I�u�W�F�N�g
	CCamera* m_pMyCamera;	// �J����
	CPlayer* m_pMyPlayer;	// �v���C���[
};

#endif