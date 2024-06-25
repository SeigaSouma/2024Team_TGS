//=============================================================================
//
// �}�b�vUI���� [map_ui.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "map_ui.h"
#include "objectX.h"
#include "game.h"
#include "course.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

//=============================================================================
// �萔��`
//=============================================================================
namespace
{
	const float DISTANCE = 600.0f;	// ����
	const MyLib::Vector3 POS = MyLib::Vector3(-1000.0f, 100.0f, 100.0f);
	const MyLib::Vector3 CAMPOS = MyLib::Vector3(-50.0f, 150.0f, 350.0f);
	const float MODEL_ROTY = D3DX_PI * 0.1f;
	const float ROTY = D3DX_PI * 0.5f;	// Y���p�x
}

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
// �t�@�C����
std::string CMapUI::m_aModelList[TYPE::TYPE_MAX] =
{
	"data\\MODEL\\map_ui\\map.x",
	"data\\MODEL\\map_ui\\player.x",
};

// �C���X�^���X
CMapUI* CMapUI::m_pInstance = nullptr;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CMapUI::CMapUI() : CObject(7)
{
	// �l�̃N���A
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_apObj[i] = nullptr;
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CMapUI::~CMapUI()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CMapUI::Init()
{
	SetType(CObject::TYPE::TYPE_OBJECTX);

	// �Q�[����ʂ����݂��Ă��邩�A�R�[�X�����邩�m�F
	if (CGame::GetInstance() == nullptr) { return E_FAIL; }
	if (CGame::GetInstance()->GetCourse() == nullptr) { return E_FAIL; }

	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_apObj[i] = CObjectX::Create(m_aModelList[i]);
		m_apObj[i]->SetEnableDisp(false);
		m_apObj[i]->SetPosition(POS);
		m_apObj[i]->SetRotation(MyLib::Vector3(0.0f, MODEL_ROTY, 0.0f));
	}

	m_pMyCamera = new CCamera;
	m_pMyCamera->Init();
	m_pMyCamera->SetRotation(MyLib::Vector3(0.0f, ROTY, 0.0f));
	m_pMyCamera->SetDistance(DISTANCE);
	m_pMyCamera->WarpCamera(POS + CAMPOS);

	SetEnableDisp(true);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMapUI::Uninit()
{
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		if (m_apObj[i] != nullptr)
		{
			m_apObj[i] = nullptr;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CMapUI::Update()
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CMapUI::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�ւ̃|�C���^���擾
	D3DXMATRIX mtxView, mtxProjection;
	D3DVIEWPORT9 viewportDef;

	// ���݂̃r���[�|�[�g�̎擾
	pDevice->GetViewport(&viewportDef);

	// ���݂̃r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ���݂̃v���W�F�N�V�����}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);

	// �ʂ̃J������ݒ肵�ĕ`�悷��
	if (m_pMyCamera != nullptr)
	{
		m_pMyCamera->SetCamera();

		// �`�揈��
		for (int i = 0; i < TYPE::TYPE_MAX; i++)
		{
			if (m_apObj[i] != nullptr)
			{
				m_apObj[i]->SetEnableDisp(true);
				m_apObj[i]->Draw();
				m_apObj[i]->SetEnableDisp(false);
			}
		}
	}

	// ���݂̃r���[�|�[�g�̎擾
	pDevice->SetViewport(&viewportDef);

	// ���݂̃r���[�}�g���b�N�X�̎擾
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);

	// ���݂̃v���W�F�N�V�����}�g���b�N�X�̎擾
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

//=============================================================================
// ����
//=============================================================================
CMapUI* CMapUI::Create()
{
	if (m_pInstance != nullptr) { return m_pInstance; }
	m_pInstance = new CMapUI;

	if (m_pInstance->Init() == E_FAIL)
	{
		// �J��
		Release();
	}

	return m_pInstance;
}

//=============================================================================
// �J��
//=============================================================================
void CMapUI::Release()
{
	if (m_pInstance == nullptr) { return; }

	// �I������
	m_pInstance = nullptr;
}
