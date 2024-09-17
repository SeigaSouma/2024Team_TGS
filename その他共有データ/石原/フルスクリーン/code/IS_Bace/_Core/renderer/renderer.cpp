//======================================================
//
//レンダリング処理[renderer.cpp]
//Author:石原颯馬
//
//======================================================
#include "renderer.h"
#include "..\..\..\main.h"
#include "..\..\_Core\manager\manager.h"
#include "..\..\_Core/debugproc/debugproc.h"
#include "..\..\_Core/_object/object_empty/object.h"
#include "..\lostrssmanager\lostrssmanager.h"

//=================================
//コンストラクタ
//=================================
CRenderer::CRenderer()
{
	//クリア
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_bResetWait = false;
}

//=================================
//デストラクタ
//=================================
CRenderer::~CRenderer()
{
}

//========================
//初期化処理
//========================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;

	m_hWnd = hWnd;

	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	//現在のスクリーンモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//ディスプレイモード設定
	m_dispMode = (bWindow == TRUE) ? CRenderer::DISPLAYMODE::MODE_WINDOW : CRenderer::DISPLAYMODE::MODE_FULLSCREEN;

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&m_d3dppWindow, sizeof(m_d3dppWindow));
	m_d3dppWindow.BackBufferWidth = SCREEN_WIDTH;			//画面サイズ（幅）
	m_d3dppWindow.BackBufferHeight = SCREEN_HEIGHT;		//画面サイズ（高さ）
	m_d3dppWindow.BackBufferFormat = d3ddm.Format;		//バックバッファの形式
	m_d3dppWindow.BackBufferCount = 1;					//バックバッファの数
	m_d3dppWindow.SwapEffect = D3DSWAPEFFECT_DISCARD;		//ダブルバッファの切り替え（同期）
	m_d3dppWindow.EnableAutoDepthStencil = TRUE;			//デプスバッファとステンシルバッファを作成
	m_d3dppWindow.AutoDepthStencilFormat = D3DFMT_D24S8;	//デプスバッファとして24ビット、ステンシルバッファとして8ビット使用
	m_d3dppWindow.Windowed = TRUE;						//ウィンドウモード
	m_d3dppWindow.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	m_d3dppWindow.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル
	m_d3dppWindow.hDeviceWindow = hWnd;

	RECT desktop;
	GetWindowRect(GetDesktopWindow(), &desktop);
	int displayWidth = desktop.right;
	int displayHeight = desktop.bottom;
	ZeroMemory(&m_d3dppFull, sizeof(m_d3dppFull));
	//<画面サイズは後で設定>
	m_d3dppFull.BackBufferFormat = d3ddm.Format;		//バックバッファの形式
	m_d3dppFull.BackBufferCount = 1;					//バックバッファの数
	m_d3dppFull.SwapEffect = D3DSWAPEFFECT_DISCARD;		//ダブルバッファの切り替え（同期）
	m_d3dppFull.EnableAutoDepthStencil = TRUE;			//デプスバッファとステンシルバッファを作成
	m_d3dppFull.AutoDepthStencilFormat = D3DFMT_D24S8;	//デプスバッファとして24ビット、ステンシルバッファとして8ビット使用
	m_d3dppFull.Windowed = TRUE;						//ウィンドウモード
	m_d3dppFull.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	m_d3dppFull.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル
	m_d3dppFull.hDeviceWindow = hWnd;

	float displayAspect = (float)displayWidth / (float)displayHeight;
	float gameAspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	if (displayAspect > gameAspect)
	{//横長
		m_d3dppFull.BackBufferWidth = displayWidth * ((float)SCREEN_HEIGHT / (float)displayHeight);			//画面サイズ（幅）
		m_d3dppFull.BackBufferHeight = SCREEN_HEIGHT;		//画面サイズ（高さ）
	}
	else
	{//縦長・変わらない
		m_d3dppFull.BackBufferWidth = SCREEN_WIDTH;			//画面サイズ（幅）
		m_d3dppFull.BackBufferHeight = displayHeight * ((float)SCREEN_WIDTH / (float)displayWidth);		//画面サイズ（高さ）
	}

	SwitchDisplayMode(m_dispMode);

	//Direct3Dデバイスの生成
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

	//オブジェクトの初期化処理
	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステート設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	return S_OK;
}

//========================
//終了処理
//========================
void CRenderer::Uninit(void)
{
	//終了処理（自分が作ったものを捨てる）
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
//更新処理
//========================
void CRenderer::Update(void)
{
	CObject::UpdateAll();
}

//========================
//描画処理
//========================
void CRenderer::Draw(void)
{
	//画面クリア（バックバッファとZバッファのクリア
	m_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);//マクロにしたら？

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//成功した場合
	 //自分が書く描画処理
		CObject::DrawAll();
		CManager::GetInstance()->GetDebProc()->Draw();

		//描画終了処理
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え（同時にデバイスロスト検知）
	HRESULT hr = m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
	if (hr == D3DERR_DEVICELOST || m_bResetWait)
	{
		SwitchDisplayMode(m_dispMode);
		ResetDevice();
	}
}

//=================================
//ビューポート設定
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
//ブレンディング種類設定
//=================================
void CRenderer::SetBlendType(BLENDTYPE type)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得

	switch (type)
	{
	case BLENDTYPE_NORMAL:	//通常
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
//Zバッファ参照設定
//=================================
void CRenderer::SetZEnable(const bool bEnable)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得

	pDevice->SetRenderState(D3DRS_ZENABLE, (bEnable == true ? TRUE : FALSE));
}

//=================================
//Zテスト設定
//=================================
void CRenderer::SetEnableZTest(bool bEnable)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得

	//Zバッファの有効・無効設定
	pDevice->SetRenderState(D3DRS_ZFUNC, (bEnable == true ? D3DCMP_LESSEQUAL : D3DCMP_ALWAYS));
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, (bEnable == true ? TRUE : FALSE));
}

//=================================
//アルファテスト設定
//=================================
void CRenderer::SetEnableAlplaTest(bool bEnable)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得

	//アルファテスト有効・無効設定
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, (bEnable == true ? TRUE : FALSE));
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, (bEnable == true ? D3DCMP_GREATER : D3DCMP_ALWAYS));
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=================================
//ディスプレイモード切替
//=================================
void CRenderer::SwitchDisplayMode(DISPLAYMODE mode)
{
	D3DDISPLAYMODE d3ddm;
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));

	//現在のスクリーンモードを取得
	m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	//モードに応じてプレゼンテーションパラメータの設定
	if (mode == CRenderer::DISPLAYMODE::MODE_WINDOW)
	{//ウィンドウモード
		m_d3dpp = m_d3dppWindow;

		//ウィンドウスタイル・位置サイズ変更
		SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hWnd, HWND_TOP, 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOZORDER | SWP_FRAMECHANGED);

		//再描画の強制
		ShowWindow(m_hWnd, SW_SHOWNORMAL);
		UpdateWindow(m_hWnd);
	}
	else if (mode == CRenderer::DISPLAYMODE::MODE_FULLSCREEN)
	{//フルスクリーンモード
		m_d3dpp = m_d3dppFull;

		//画面サイズ取得
		RECT desktop;
		GetWindowRect(GetDesktopWindow(), &desktop);

		//ウィンドウスタイル・位置サイズ変更
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, desktop.right, desktop.bottom, SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	m_dispMode = mode;

	//リソースいったん解放
	CLostResourceManager::GetInstance()->Backup();
	m_bResetWait = true;
}

//=================================
//デバイスリセット
//=================================
void CRenderer::ResetDevice()
{
	//デバイスリセット
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

		//リソース再生成
		CLostResourceManager::GetInstance()->Restore();
		m_bResetWait = false;
	}
}