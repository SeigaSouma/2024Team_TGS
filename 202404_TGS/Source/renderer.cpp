//=============================================================================
// 
//  �����_���[���� [renderer.cpp]
//  Author : ���n�Ή�
// 
//=============================================================================
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "debugproc.h"
#include "camera.h"
#include "fade.h"
#include "instantfade.h"
#include "blackframe.h"
#include "pause.h"
#include "input.h"
#include "loadmanager.h"
#include "Imguimanager.h"
#include "fog.h"

namespace
{
	const D3DXCOLOR ALPHACOLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.95f);
	const D3DXCOLOR NONE_ALPHACOLOR = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	const D3DXVECTOR2 NORMALSIZE = D3DXVECTOR2(640.0f, 360.0f);
	const D3DXVECTOR2 MINISIZE = D3DXVECTOR2(640.0f, 360.0f) * 1.01f;
}

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;			// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	m_pD3DDevice = nullptr;	// Direct3D�f�o�C�X�ւ̃|�C���^
}

//==========================================================================
// �f�X�g���N�^
//==========================================================================
CRenderer::~CRenderer()
{

}

//==========================================================================
// ����������
//==========================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							// �p�����[�^�̃[���N���A

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	m_d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�o�C�X�o�b�t�@�Ƃ���16bit���g��
	m_d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��


	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_���[�X�e�[�g�ݒ�
	ResetRendererState();
	

	// �����̎��ݒ�
	srand((unsigned int)time(0));


	// �}���`�^�[�Q�b�g�����_���[�̏�����
	InitMTRender();

	return S_OK;
}

//==========================================================================
// �����_���[�X�e�[�g�ݒ�
//==========================================================================
void CRenderer::ResetRendererState()
{
	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}


//==========================================================================
// �}���`�^�[�Q�b�g�����_���[�̏�����
//==========================================================================
void CRenderer::InitMTRender()
{
	// �}���`�^�[�Q�b�g��ʂ̕`�攻��
	m_bDrawMultiScreen = false;

	// �ۑ��p�o�b�t�@
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̐���
	for (int i = 0; i < 2; i++)
	{
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_Multitarget.pTextureMT[i],
			NULL);


		// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X�̐���
		m_Multitarget.pTextureMT[i]->GetSurfaceLevel(0, &m_Multitarget.pRenderMT[i]);
	}

	// �e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
	m_pD3DDevice->CreateDepthStencilSurface(
		SCREEN_WIDTH, SCREEN_HEIGHT,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_Multitarget.pZBuffMT,
		NULL);

	// ���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// ���݂�Z�o�b�t�@���擾(�ۑ�)
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[0]);

	// Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
	m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

	// �����_�����O�^�[�Q�b�g�p�̃e�N�X�`���̃N���A
	m_pD3DDevice->Clear(
		0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0);


	// �����_�����O�^�[�Q�b�g�����ɖ߂�
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);

	// Z�o�b�t�@�����ɖ߂�
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// �e�N�X�`���p�r���[�|�[�g�̐ݒ�
	m_Multitarget.viewportMT.X = 0;						// �`�悷���ʂ̍���X���W
	m_Multitarget.viewportMT.Y = 0;						// �`�悷���ʂ̍���Y���W
	m_Multitarget.viewportMT.Width = SCREEN_WIDTH;		// �`�悷���ʂ̕�
	m_Multitarget.viewportMT.Height = SCREEN_HEIGHT;	// �`�悷���ʂ̍���
	m_Multitarget.viewportMT.MinZ = 0.0f;
	m_Multitarget.viewportMT.MaxZ = 1.0f;


	m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_Multitarget.pVtxBuff,
		nullptr);



	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_Multitarget.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 size = NORMALSIZE;

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = 640.0f - size.x;
	pVtx[0].pos.y = 360.0f - size.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 640.0f + size.x;
	pVtx[1].pos.y = 360.0f - size.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = 640.0f - size.x;
	pVtx[2].pos.y = 360.0f + size.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 640.0f + size.x;
	pVtx[3].pos.y = 360.0f + size.y;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = ALPHACOLOR;
	pVtx[1].col = ALPHACOLOR;
	pVtx[2].col = ALPHACOLOR;
	pVtx[3].col = ALPHACOLOR;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N���b�N
	m_Multitarget.pVtxBuff->Unlock();

}

//==========================================================================
// �I������
//==========================================================================
void CRenderer::Uninit()
{
	// Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//==========================================================================
// �X�V����
//==========================================================================
void CRenderer::Update()
{
	// �S�Ă̍X�V
	CObject::UpdateAll();
}

//==========================================================================
// �`�揈��
//==========================================================================
void CRenderer::Draw()
{
	static bool bDisp = true;

	// �L�[�{�[�h���擾
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

//#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_F1))
	{// �`��؂�ւ�
		bDisp = bDisp ? false : true;
	}
//#endif

	// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	m_pD3DDevice->Clear
	(	0, nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);

	// �J�����̐ݒ�
	CCamera* pCamerea = CManager::GetInstance()->GetCamera();

	// �ؑ֗p
	LPDIRECT3DTEXTURE9 pTextureWk;	// �ؑ֗p�p�e�N�X�`��
	LPDIRECT3DSURFACE9 pRenderWk;	// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X

	// �`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// �`�悪���������Ƃ�


		// �t�H�O������
		MyFog::DisableFog(m_pD3DDevice);

		// �t�H�O�L��
		MyFog::SetFog();

		D3DVIEWPORT9 viewportDef;	//�r���[�|�[�g�ۑ�

		// ���݂̃r���[�|�[�g���擾
		m_pD3DDevice->GetViewport(&viewportDef);

		if (CManager::GetInstance()->IsLoadComplete())
		{

			// �ۑ��p�o�b�t�@
			LPDIRECT3DSURFACE9 pRenderDef = nullptr, pZBuffDef = nullptr;
			D3DXMATRIX mtxView, mtxProjection;

			
			// �f�t�H���g�̃����_���[�^�[�Q�b�g���擾
			GetDefaultRenderTarget(&pRenderDef, &pZBuffDef, &mtxView, &mtxProjection);


			// �^�[�Q�b�g�ؑ�
			if (m_bDrawMultiScreen)
			{
				CManager::GetInstance()->GetRenderer()->ChangeTarget(pCamerea->GetPositionV(), pCamerea->GetPositionR(), MyLib::Vector3(0.0f, 1.0f, 0.0f));

				// �e�N�X�`��[0]�̃N���A
				m_pD3DDevice->Clear(
					0, nullptr,
					(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
					1.0f,
					0);
			}

			if (!m_bDrawMultiScreen)
			{
				CManager::GetInstance()->GetRenderer()->ChangeTarget(pCamerea->GetPositionV(), pCamerea->GetPositionR(), MyLib::Vector3(0.0f, 1.0f, 0.0f));

				// �e�N�X�`��[0]�̃N���A
				m_pD3DDevice->Clear(
					0, nullptr,
					(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
					1.0f,
					0);

				// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
				m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[1]);

				// Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
				m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

				// �e�N�X�`��[0]�̃N���A
				m_pD3DDevice->Clear(
					0, nullptr,
					(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
					1.0f,
					0);

				CManager::GetInstance()->GetRenderer()->ChangeRendertarget(pRenderDef, pZBuffDef, mtxView, mtxProjection);
			}

			// �S�Ă̕`��
			CObject::DrawAll();

			// �t�B�[�h�o�b�N�G�t�F�N�g�Ƀe�N�X�`��[1]��\��t���ĕ`��
			if (m_bDrawMultiScreen)
			{
				DrawMultiTargetScreen(1, ALPHACOLOR, MINISIZE);
			}

			// �J�����̐ݒ�
			CManager::GetInstance()->GetCamera()->SetCamera();

			if (m_bDrawMultiScreen)
			{
				// �����_�[�^�[�Q�b�g�����Ƃɖ߂�
				CManager::GetInstance()->GetRenderer()->ChangeRendertarget(pRenderDef, pZBuffDef, mtxView, mtxProjection);
			}

			// �t�B�[�h�o�b�N�G�t�F�N�g�Ƀe�N�X�`��[0]��\��t���ĕ`��
			if (m_bDrawMultiScreen)
			{
				DrawMultiTargetScreen(0, NONE_ALPHACOLOR, NORMALSIZE);
			}
			

			// �e�N�X�`��0��1�̐ؑ�
			pTextureWk = m_Multitarget.pTextureMT[0];
			m_Multitarget.pTextureMT[0] = m_Multitarget.pTextureMT[1];
			m_Multitarget.pTextureMT[1] = pTextureWk;

			pRenderWk = m_Multitarget.pRenderMT[0];
			m_Multitarget.pRenderMT[0] = m_Multitarget.pRenderMT[1];
			m_Multitarget.pRenderMT[1] = pRenderWk;


			// �f�o�b�O�\���̕`�揈��
			CManager::GetInstance()->GetDebugProc()->Draw();

			// �e�L�X�g�̐ݒ�
			CDebugProc::SetText();

			// �|�[�Y�`�揈��
			if (bDisp)
			{
				CManager::GetInstance()->GetPause()->Draw();
			}

			// ���t���[��
			CManager::GetInstance()->GetBlackFrame()->Draw();
		}
		else
			// ���[�h�}�l�[�W���̍X�V
		{
			GetLoadManager()->Draw();
		}

		// �J�ڂȂ��t�F�[�h�`�揈��
		CInstantFade* pfalsefade = CManager::GetInstance()->GetInstantFade();
		if (pfalsefade != nullptr)
		{
			pfalsefade->Draw();
		}

		// �t�F�[�h�`�揈��
		CFade* pfade = CManager::GetInstance()->GetFade();
		if (pfade != nullptr)
		{
			pfade->Draw();
		}

		// �r���[�|�[�g�����ɖ߂�
		m_pD3DDevice->SetViewport(&viewportDef);

		// �`��I��
		m_pD3DDevice->EndScene();
	}


	// Imgui�̕`��
	ImguiMgr::Draw();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//==========================================================================
// �}���`�^�[�Q�b�g��ʂ̕`��
//==========================================================================
void CRenderer::DrawMultiTargetScreen(int texIdx, const D3DXCOLOR& col, const D3DXVECTOR2& size)
{
	// ���_���ւ̃|�C���^
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_Multitarget.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos.x = 640.0f - size.x;
	pVtx[0].pos.y = 360.0f - size.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = 640.0f + size.x;
	pVtx[1].pos.y = 360.0f - size.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = 640.0f - size.x;
	pVtx[2].pos.y = 360.0f + size.y;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = 640.0f + size.x;
	pVtx[3].pos.y = 360.0f + size.y;
	pVtx[3].pos.z = 0.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�o�b�t�@���A�����b�N���b�N
	m_Multitarget.pVtxBuff->Unlock();


	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	m_pD3DDevice->SetStreamSource(0, m_Multitarget.pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	m_pD3DDevice->SetTexture(0, m_Multitarget.pTextureMT[texIdx]);

	// �|���S���̕`��
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// �f�o�C�X�̎擾
//==========================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice() const
{
	return m_pD3DDevice;
}

HRESULT CRenderer::SetFullScreen()
{
	m_pD3DDevice->Reset(&m_d3dpp);

	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h

	// Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}


	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// ���s�����Ƃ�
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							// �p�����[�^�̃[���N���A

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	m_d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�o�C�X�o�b�t�@�Ƃ���16bit���g��
	m_d3dpp.Windowed = TRUE;										// �E�B���h�E���[�h
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��


	ResetWnd();

	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		GetWnd(),
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			GetWnd(),
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				GetWnd(),
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// ����\��ݒ�
	timeBeginPeriod(1);

	// �E�C���h�E�̕\��
	ShowWindow(GetWnd(), GetCmbShow());		// �E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(GetWnd());				// �N���C�A���g�̈���X�V

	return S_OK;
}

//==========================================================================
// �f�t�H���g�̃����_�[�^�[�Q�b�g�擾
//==========================================================================
void CRenderer::GetDefaultRenderTarget(LPDIRECT3DSURFACE9* pRender, LPDIRECT3DSURFACE9* pZBuff, D3DXMATRIX* viewport, D3DXMATRIX* projection)
{
	// ���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
	m_pD3DDevice->GetRenderTarget(0, &(*pRender));

	// ���݂�Z�o�b�t�@���擾(�ۑ�)
	m_pD3DDevice->GetDepthStencilSurface(&(*pZBuff));

	m_pD3DDevice->GetTransform(D3DTS_VIEW, &(*viewport));
	m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &(*projection));
}

//==========================================================================
// �����_�����O�^�[�Q�b�g�̐ؑ�
//==========================================================================
void CRenderer::ChangeRendertarget(LPDIRECT3DSURFACE9 pRender, LPDIRECT3DSURFACE9 pZBuff, D3DXMATRIX viewport, D3DXMATRIX projection)
{
	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	m_pD3DDevice->SetRenderTarget(0, pRender);

	// Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
	m_pD3DDevice->SetDepthStencilSurface(pZBuff);

	// �e�N�X�`�������_�����O�p�̃r���[�|�[�g��ݒ�
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &viewport);

	// �e�N�X�`�������_�����O�p�̃v���W�F�N�V�����}�g���b�N�X��ݒ�
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projection);
}

//==========================================================================
// �����_�����O�^�[�Q�b�g�̐ؑ�
//==========================================================================
void CRenderer::ChangeTarget(MyLib::Vector3 posV, MyLib::Vector3 posR, MyLib::Vector3 vecU)
{
	D3DXMATRIX mtxview, mtxProjection;


	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[0]);

	// Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
	m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

	// �e�N�X�`�������_�����O�p�̃r���[�|�[�g��ݒ�
	m_pD3DDevice->SetViewport(&m_Multitarget.viewportMT);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(
		&mtxProjection,
		D3DXToRadian(45.0f),	// ����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// �A�X�y�N�g��
		10.0f,		// ��O�̐���
		100000.0f);	// ���s���̐���

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxview);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(
		&mtxview,
		&posV,	// ���_
		&posR,	// �����_
		&vecU);	// ������x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxview);
}