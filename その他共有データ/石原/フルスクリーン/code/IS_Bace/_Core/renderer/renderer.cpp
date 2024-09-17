//======================================================
//
//�����_�����O����[renderer.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "renderer.h"
#include "..\..\..\main.h"
#include "..\..\_Core\manager\manager.h"
#include "..\..\_Core/debugproc/debugproc.h"
#include "..\..\_Core/_object/object_empty/object.h"
#include "..\lostrssmanager\lostrssmanager.h"

//=================================
//�R���X�g���N�^
//=================================
CRenderer::CRenderer()
{
	//�N���A
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_bResetWait = false;
}

//=================================
//�f�X�g���N�^
//=================================
CRenderer::~CRenderer()
{
}

//========================
//����������
//========================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;

	m_hWnd = hWnd;

	//Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	//���݂̃X�N���[�����[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�B�X�v���C���[�h�ݒ�
	m_dispMode = (bWindow == TRUE) ? CRenderer::DISPLAYMODE::MODE_WINDOW : CRenderer::DISPLAYMODE::MODE_FULLSCREEN;

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&m_d3dppWindow, sizeof(m_d3dppWindow));
	m_d3dppWindow.BackBufferWidth = SCREEN_WIDTH;			//��ʃT�C�Y�i���j
	m_d3dppWindow.BackBufferHeight = SCREEN_HEIGHT;		//��ʃT�C�Y�i�����j
	m_d3dppWindow.BackBufferFormat = d3ddm.Format;		//�o�b�N�o�b�t�@�̌`��
	m_d3dppWindow.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̐�
	m_d3dppWindow.SwapEffect = D3DSWAPEFFECT_DISCARD;		//�_�u���o�b�t�@�̐؂�ւ��i�����j
	m_d3dppWindow.EnableAutoDepthStencil = TRUE;			//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	m_d3dppWindow.AutoDepthStencilFormat = D3DFMT_D24S8;	//�f�v�X�o�b�t�@�Ƃ���24�r�b�g�A�X�e���V���o�b�t�@�Ƃ���8�r�b�g�g�p
	m_d3dppWindow.Windowed = TRUE;						//�E�B���h�E���[�h
	m_d3dppWindow.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	m_d3dppWindow.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��
	m_d3dppWindow.hDeviceWindow = hWnd;

	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	int displayWidth = desktop.right;
	int displayHeight = desktop.bottom;
	ZeroMemory(&m_d3dppFull, sizeof(m_d3dppFull));
	//<��ʃT�C�Y�͌�Őݒ�>
	m_d3dppFull.BackBufferFormat = d3ddm.Format;		//�o�b�N�o�b�t�@�̌`��
	m_d3dppFull.BackBufferCount = 1;					//�o�b�N�o�b�t�@�̐�
	m_d3dppFull.SwapEffect = D3DSWAPEFFECT_DISCARD;		//�_�u���o�b�t�@�̐؂�ւ��i�����j
	m_d3dppFull.EnableAutoDepthStencil = TRUE;			//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	m_d3dppFull.AutoDepthStencilFormat = D3DFMT_D24S8;	//�f�v�X�o�b�t�@�Ƃ���24�r�b�g�A�X�e���V���o�b�t�@�Ƃ���8�r�b�g�g�p
	m_d3dppFull.Windowed = TRUE;						//�E�B���h�E���[�h
	m_d3dppFull.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	m_d3dppFull.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��
	m_d3dppFull.hDeviceWindow = hWnd;

	float displayAspect = (float)displayWidth / (float)displayHeight;
	float gameAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	if (displayAspect > gameAspect)
	{//����
		m_d3dppFull.BackBufferWidth = displayWidth * ((float)SCREEN_HEIGHT / (float)displayHeight);			//��ʃT�C�Y�i���j
		m_d3dppFull.BackBufferHeight = SCREEN_HEIGHT;		//��ʃT�C�Y�i�����j
	}
	else
	{//�c���E�ς��Ȃ�
		m_d3dppFull.BackBufferWidth = SCREEN_WIDTH;			//��ʃT�C�Y�i���j
		m_d3dppFull.BackBufferHeight = displayHeight * ((float)SCREEN_WIDTH / (float)displayWidth);		//��ʃT�C�Y�i�����j
	}

	SwitchDisplayMode(m_dispMode);

	//Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
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

	//�I�u�W�F�N�g�̏���������
	//�����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	return S_OK;
}

//========================
//�I������
//========================
void CRenderer::Uninit(void)
{
	//�I�������i��������������̂��̂Ă�j
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//========================
//�X�V����
//========================
void CRenderer::Update(void)
{
	CObject::UpdateAll();
}

//========================
//�`�揈��
//========================
void CRenderer::Draw(void)
{
	//��ʃN���A�i�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);//�}�N���ɂ�����H

	//�`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//���������ꍇ
	 //�����������`�揈��
		CObject::DrawAll();
		CManager::GetInstance()->GetDebProc()->Draw();

		//�`��I������
		m_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ��i�����Ƀf�o�C�X���X�g���m�j
	HRESULT hr = m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
	if (hr == D3DERR_DEVICELOST || m_bResetWait)
	{
		SwitchDisplayMode(m_dispMode);
		ResetDevice();
	}
}

//=================================
//�r���[�|�[�g�ݒ�
//=================================
void CRenderer::SetViewport(float x, float y, float width, float height)
{
	D3DVIEWPORT9 vp;
	vp.X = x;
	vp.Y = y;
	vp.Width = width;
	vp.Height = height;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	m_pD3DDevice->SetViewport(&vp);
}

//=================================
//�u�����f�B���O��ސݒ�
//=================================
void CRenderer::SetBlendType(BLENDTYPE type)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾

	switch (type)
	{
	case BLENDTYPE_NORMAL:	//�ʏ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case BLENDTYPE_ADD:
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case BLENDTYPE_SUB:
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	}
}

//=================================
//Z�o�b�t�@�Q�Ɛݒ�
//=================================
void CRenderer::SetZEnable(const bool bEnable)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾

	pDevice->SetRenderState(D3DRS_ZENABLE, (bEnable == true ? TRUE : FALSE));
}

//=================================
//Z�e�X�g�ݒ�
//=================================
void CRenderer::SetEnableZTest(bool bEnable)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾

	//Z�o�b�t�@�̗L���E�����ݒ�
	pDevice->SetRenderState(D3DRS_ZFUNC, (bEnable == true ? D3DCMP_LESSEQUAL : D3DCMP_ALWAYS));
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, (bEnable == true ? TRUE : FALSE));
}

//=================================
//�A���t�@�e�X�g�ݒ�
//=================================
void CRenderer::SetEnableAlplaTest(bool bEnable)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾

	//�A���t�@�e�X�g�L���E�����ݒ�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, (bEnable == true ? TRUE : FALSE));
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, (bEnable == true ? D3DCMP_GREATER : D3DCMP_ALWAYS));
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=================================
//�f�B�X�v���C���[�h�ؑ�
//=================================
void CRenderer::SwitchDisplayMode(DISPLAYMODE mode)
{
	D3DDISPLAYMODE d3ddm;
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));

	//���݂̃X�N���[�����[�h���擾
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	//���[�h�ɉ����ăv���[���e�[�V�����p�����[�^�̐ݒ�
	if (mode == CRenderer::DISPLAYMODE::MODE_WINDOW)
	{//�E�B���h�E���[�h
		m_d3dpp = m_d3dppWindow;

		//�E�B���h�E�X�^�C���E�ʒu�T�C�Y�ύX
		SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hWnd, HWND_TOP, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOZORDER | SWP_FRAMECHANGED);

		//�ĕ`��̋���
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
	}
	else if (mode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
	{//�t���X�N���[�����[�h
		m_d3dpp = m_d3dppFull;

		//��ʃT�C�Y�擾
		RECT desktop;
		GetWindowRect(GetDesktopWindow(), &desktop);

		//�E�B���h�E�X�^�C���E�ʒu�T�C�Y�ύX
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, desktop.right, desktop.bottom, SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	m_dispMode = mode;

	//���\�[�X����������
	CLostResourceManager::GetInstance()->Backup();
	m_bResetWait = true;
}

//=================================
//�f�o�C�X���Z�b�g
//=================================
void CRenderer::ResetDevice()
{
	//�f�o�C�X���Z�b�g
	HRESULT hr = m_pD3DDevice->TestCooperativeLevel();
	if (hr == S_OK || hr == D3DERR_DEVICENOTRESET)
	{
		hr = m_pD3DDevice->Reset(&m_d3dpp);
		if (FAILED(hr))
		{
			if (hr == D3DERR_INVALIDCALL)
			{
				assert(false);
			}

			return;
		}

		//���\�[�X�Đ���
		CLostResourceManager::GetInstance()->Restore();
		m_bResetWait = false;
	}
}