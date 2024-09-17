//======================================================
//
//�Q�[���V�[��[game.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "game.h"
#include "..\..\..\..\main.h"
#include "..\pause\pause.h"
#include "..\result\result.h"
#include "..\ranking\ranking.h"
#include "..\..\manager\manager.h"
#include "..\..\texture\texture.h"
#include "..\..\camera\camera.h"
#include "..\..\sound\sound.h"
#include "..\..\_object\object_empty\object.h"
#include "..\..\input\input.h"
#include "..\..\..\_Expansion\fade\fade.h"
#include "..\..\utility\Utility.h"
#include "../../chara/character.h"
#include "..\..\_object\object_3d_instancing\object3D_instancing.h"
#include "..\..\_object\object_3d\object3D.h"
#include "..\..\debugproc\debugproc.h"

//=================================
//�R���X�g���N�^
//=================================
CGame::CGame()
{
	m_pPause = nullptr;
	m_pFade = nullptr;
}

//=================================
//�f�X�g���N�^
//=================================
CGame::~CGame()
{
}

//=================================
//������
//=================================
HRESULT CGame::Init(void)
{
	//�J�������Z�b�g
	CManager::GetInstance()->CManager::GetInstance()->GetCamera()->ResetPos();

	int texNum = CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\Block_R_01.png");
	CObject2D::Create(D3DXVECTOR3(60.0f, 60.0f, 0.0f), IS_Utility::VEC3_ZERO, 100.0f, 100.0f)->BindTexture(texNum);
	CObject2D::Create(D3DXVECTOR3(1280.0f, 60.0f, 0.0f), IS_Utility::VEC3_ZERO, 100.0f, 100.0f)->BindTexture(texNum);
	CObject2D::Create(D3DXVECTOR3(60.0f, 720.0f, 0.0f), IS_Utility::VEC3_ZERO, 100.0f, 100.0f)->BindTexture(texNum);
	CObject2D::Create(D3DXVECTOR3(1280.0f, 720.0f, 0.0f), IS_Utility::VEC3_ZERO, 100.0f, 100.0f)->BindTexture(texNum);
	CObject2D::Create(D3DXVECTOR3(300.0f, 400.0f, 0.0f), IS_Utility::VEC3_ZERO, 100.0f, 100.0f)->BindTexture(texNum);

	//CObject3D_Instancing* pobj = CObject3D_Instancing::Create(100.0f, 100.0f, 2);
	//pobj->SetInstanceNum(3);
 	//pobj->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\Block_R_01.png"));
	//pobj->AddInstance(D3DXVECTOR3(0.0f, 0.0f, 0.0f), IS_Utility::VEC3_ZERO, 0, 0, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//pobj->AddInstance(D3DXVECTOR3(50.0f, 1.0f, 50.0f), IS_Utility::VEC3_ZERO, 0, 0, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	//pobj->AddInstance(D3DXVECTOR3(-50.0f, -1.0f, -50.0f), IS_Utility::VEC3_ZERO, 0, 0, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	return S_OK;
}

//=================================
//�I��
//=================================
void CGame::Uninit(void)
{
	//�I�u�W�F�S�j��
	for (int cnt = 0; cnt < CObject::PRIORITY_FADE; cnt++)
	{
		CObject::ReleaseAll(cnt);
	}

	//���~�߂�
	CManager::GetInstance()->GetSound()->Stop();
}

//=================================
//�X�V
//=================================
void CGame::Update(void)
{
	CManager* pIns = CManager::GetInstance();
	CInputKeyboard* pKeyboard = pIns->GetInputKeyboard();
	CInputGamePad* pGamepad = pIns->GetInputGamePad();
	bool bPush = false;

	if (pGamepad != nullptr && pGamepad->IsConnect() == true)
	{//�Q�[���p�b�h�ڑ�
		if (pGamepad->GetTrigger(XINPUT_GAMEPAD_A))
		{
			bPush = true;
		}
	}
	else
	{//���ڑ�
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			bPush = true;
		}
	}

	if (m_pFade == nullptr && bPush == true)
	{//�X�y�[�X�����ꂽ
		m_pFade = CFade::Create(CScene::MODE_TUTORIAL);
	}
}

//=================================
//�`��
//=================================
void CGame::Draw(void)
{
	//���i�͂��ׂ�CObject�N���X�p�����Ă���̂������Ă���̂Ŏ����`��
	//�|�[�Y�̒��g�����ׂ�CObject�N���X�p�����Ă�̂Ŏ����`��
	//����Ă����ł��邱�Ƃ͂Ȃ�
}