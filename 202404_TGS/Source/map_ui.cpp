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
#include "player.h"

//=============================================================================
// �萔��`
//=============================================================================
namespace
{
	const float DISTANCE = 600.0f;	// ����
	const MyLib::Vector3 POS = MyLib::Vector3(-1000.0f, 100.0f, 100.0f);
	const MyLib::Vector3 PLAYERPOS = MyLib::Vector3(-78.0f, 33.0f, 1.7f);
	const MyLib::Vector3 CAMPOS = MyLib::Vector3(-87.0f, 108.0f, 320.0f);
	const float MODEL_ROTY = D3DX_PI * 1.0f;
	const float MODEL_ROTZ = -D3DX_PI * 0.2f;
	const float MODEL_ROTMAXMOVEZ = -D3DX_PI * 1.4f;
	const float ROTY = D3DX_PI * 0.35f;	// Y���p�x
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

	m_pMyCamera = nullptr;
	m_pMyPlayer = nullptr;
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

	// �I�u�W�F�N�g�̐����Ɛݒ�
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		m_apObj[i] = CObjectX::Create(m_aModelList[i]);
		m_apObj[i]->SetEnableDisp(false);
		m_apObj[i]->SetPosition(POS + i * PLAYERPOS);
		m_apObj[i]->SetRotation(MyLib::Vector3(0.0f, MODEL_ROTY, MODEL_ROTZ));
	}

	// �`��p�J�����̐����Ɛݒ�
	m_pMyCamera = new CCamera;
	m_pMyCamera->Init();
	m_pMyCamera->SetRotation(MyLib::Vector3(0.0f, ROTY, 0.0f));
	m_pMyCamera->SetDistance(DISTANCE);
	m_pMyCamera->WarpCamera(POS + CAMPOS);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CMapUI::Uninit()
{
	// �I�u�W�F�N�g�̔p��
	for (int i = 0; i < TYPE::TYPE_MAX; i++)
	{
		if (m_apObj[i] != nullptr)
		{
			m_apObj[i] = nullptr;
		}
	}

	// �J�����̔p��
	if (m_pMyCamera != nullptr)
	{
		m_pMyCamera->Uninit();
		delete m_pMyCamera;
		m_pMyCamera = nullptr;
	}

	m_pMyPlayer = nullptr;
	m_pInstance = nullptr;
}

//=============================================================================
// �X�V����
//=============================================================================
void CMapUI::Update()
{
	// �}�b�v�̉�]����
	SetMapRotation();

	//float windowWidth = 100.0f;
	//const float  POS_MOVE = 0.1f;
	////=============================
	//// �ʒu�ݒ�
	////=============================
	//ImGui::Text("pos");
	//ImGui::SameLine();
	//// X
	//ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
	//{
	//	ImGui::SetNextItemWidth(windowWidth);
	//	ImGui::DragFloat("x", &PLAYERPOS.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
	//	ImGui::SameLine();
	//}
	//ImGui::PopID();
	//// Y
	//ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
	//{
	//	ImGui::SetNextItemWidth(windowWidth);
	//	ImGui::DragFloat("y", &PLAYERPOS.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
	//	ImGui::SameLine();
	//}
	//ImGui::PopID();
	//// Z
	//ImGui::PushID(0); // �E�B�W�F�b�g���ƂɈقȂ�ID�����蓖�Ă�
	//{
	//	ImGui::SetNextItemWidth(windowWidth);
	//	ImGui::DragFloat("z", &PLAYERPOS.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
	//}
	//ImGui::PopID();
	//m_pMyCamera->WarpCamera(POS + CAMPOS);

	//m_apObj[TYPE::TYPE_PLAYER]->SetPosition(POS + PLAYERPOS);
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

//=============================================================================
// ��]����
//=============================================================================
void CMapUI::SetMapRotation()
{
	CGame* pgame = CGame::GetInstance();

	// �Q�[����ʂ��m�F
	if (pgame == nullptr) { return; }

	// �v���C���[�����邩�m�F
	if (m_pMyPlayer == nullptr) { return; }

	// �}�b�v�I�u�W�F�N�g�̉�]�擾
	MyLib::Vector3 rot = m_apObj[TYPE::TYPE_MAP]->GetRotation();

	// �v���C���[�̐i�s�̊��������߂ĉ�]
	float courselen = pgame->GetCourse()->GetCourceLength();
	float playerlen = m_pMyPlayer->GetMoveLength();
	m_fRatio = playerlen / courselen;
	rot.z = MODEL_ROTZ + (MODEL_ROTMAXMOVEZ * m_fRatio);

	// �ݒ�
	m_apObj[TYPE::TYPE_MAP]->SetRotation(rot);
}
