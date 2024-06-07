//=============================================================================
// 
//  レンダラー処理 [renderer.cpp]
//  Author : 相馬靜雅
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
// コンストラクタ
//==========================================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;			// Direct3Dオブジェクトへのポインタ
	m_pD3DDevice = nullptr;	// Direct3Dデバイスへのポインタ
}

//==========================================================================
// デストラクタ
//==========================================================================
CRenderer::~CRenderer()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{// 失敗したとき
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// 失敗したとき
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							// パラメータのゼロクリア

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	m_d3dpp.BackBufferCount = 1;									// バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// デバイスバッファとして16bitを使う
	m_d3dpp.Windowed = bWindow;									// ウィンドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル


	// Direct3Dデバイスの生成
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

	// レンダラーステート設定
	ResetRendererState();
	

	// 乱数の種を設定
	srand((unsigned int)time(0));


	// マルチターゲットレンダラーの初期化
	InitMTRender();

	return S_OK;
}

//==========================================================================
// レンダラーステート設定
//==========================================================================
void CRenderer::ResetRendererState()
{
	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}


//==========================================================================
// マルチターゲットレンダラーの初期化
//==========================================================================
void CRenderer::InitMTRender()
{
	// マルチターゲット画面の描画判定
	m_bDrawMultiScreen = false;

	// 保存用バッファ
	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	// レンダリングターゲット用テクスチャの生成
	for (int i = 0; i < 2; i++)
	{
		m_pD3DDevice->CreateTexture(SCREEN_WIDTH, SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_Multitarget.pTextureMT[i],
			NULL);


		// テクスチャレンダリング用インターフェースの生成
		m_Multitarget.pTextureMT[i]->GetSurfaceLevel(0, &m_Multitarget.pRenderMT[i]);
	}

	// テクスチャレンダリング用Zバッファの生成
	m_pD3DDevice->CreateDepthStencilSurface(
		SCREEN_WIDTH, SCREEN_HEIGHT,
		D3DFMT_D16,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_Multitarget.pZBuffMT,
		NULL);

	// 現在のレンダリングターゲットを取得(保存)
	m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファを取得(保存)
	m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[0]);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

	// レンダリングターゲット用のテクスチャのクリア
	m_pD3DDevice->Clear(
		0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0);


	// レンダリングターゲットを元に戻す
	m_pD3DDevice->SetRenderTarget(0, pRenderDef);

	// Zバッファを元に戻す
	m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

	// テクスチャ用ビューポートの設定
	m_Multitarget.viewportMT.X = 0;						// 描画する画面の左上X座標
	m_Multitarget.viewportMT.Y = 0;						// 描画する画面の左上Y座標
	m_Multitarget.viewportMT.Width = SCREEN_WIDTH;		// 描画する画面の幅
	m_Multitarget.viewportMT.Height = SCREEN_HEIGHT;	// 描画する画面の高さ
	m_Multitarget.viewportMT.MinZ = 0.0f;
	m_Multitarget.viewportMT.MaxZ = 1.0f;


	m_pD3DDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_Multitarget.pVtxBuff,
		nullptr);



	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_Multitarget.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 size = NORMALSIZE;

	// 頂点座標の設定
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

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	pVtx[0].col = ALPHACOLOR;
	pVtx[1].col = ALPHACOLOR;
	pVtx[2].col = ALPHACOLOR;
	pVtx[3].col = ALPHACOLOR;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックロック
	m_Multitarget.pVtxBuff->Unlock();

}

//==========================================================================
// 終了処理
//==========================================================================
void CRenderer::Uninit()
{
	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CRenderer::Update()
{
	// 全ての更新
	CObject::UpdateAll();
}

//==========================================================================
// 描画処理
//==========================================================================
void CRenderer::Draw()
{
	static bool bDisp = true;

	// キーボード情報取得
	CInputKeyboard *pInputKeyboard = CInputKeyboard::GetInstance();

//#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_F1))
	{// 描画切り替え
		bDisp = bDisp ? false : true;
	}
//#endif

	// 画面クリア(バックバッファとZバッファのクリア)
	m_pD3DDevice->Clear
	(	0, nullptr,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
		1.0f,
		0
	);

	// カメラの設定
	CCamera* pCamerea = CManager::GetInstance()->GetCamera();

	// 切替用
	LPDIRECT3DTEXTURE9 pTextureWk;	// 切替用用テクスチャ
	LPDIRECT3DSURFACE9 pRenderWk;	// テクスチャレンダリング用インターフェース

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画が成功したとき


		// フォグを解除
		MyFog::DisableFog(m_pD3DDevice);

		// フォグ有効
		MyFog::SetFog();

		D3DVIEWPORT9 viewportDef;	//ビューポート保存

		// 現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);

		if (CManager::GetInstance()->IsLoadComplete())
		{

			// 保存用バッファ
			LPDIRECT3DSURFACE9 pRenderDef = nullptr, pZBuffDef = nullptr;
			D3DXMATRIX mtxView, mtxProjection;

			
			// デフォルトのレンダラーターゲットを取得
			GetDefaultRenderTarget(&pRenderDef, &pZBuffDef, &mtxView, &mtxProjection);


			// ターゲット切替
			if (m_bDrawMultiScreen)
			{
				CManager::GetInstance()->GetRenderer()->ChangeTarget(pCamerea->GetPositionV(), pCamerea->GetPositionR(), MyLib::Vector3(0.0f, 1.0f, 0.0f));

				// テクスチャ[0]のクリア
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

				// テクスチャ[0]のクリア
				m_pD3DDevice->Clear(
					0, nullptr,
					(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
					1.0f,
					0);

				// レンダリングターゲットを生成したテクスチャに設定
				m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[1]);

				// Zバッファを生成したZバッファに設定
				m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

				// テクスチャ[0]のクリア
				m_pD3DDevice->Clear(
					0, nullptr,
					(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
					D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f),
					1.0f,
					0);

				CManager::GetInstance()->GetRenderer()->ChangeRendertarget(pRenderDef, pZBuffDef, mtxView, mtxProjection);
			}

			// 全ての描画
			CObject::DrawAll();

			// フィードバックエフェクトにテクスチャ[1]を貼り付けて描画
			if (m_bDrawMultiScreen)
			{
				DrawMultiTargetScreen(1, ALPHACOLOR, MINISIZE);
			}

			// カメラの設定
			CManager::GetInstance()->GetCamera()->SetCamera();

			if (m_bDrawMultiScreen)
			{
				// レンダーターゲットをもとに戻す
				CManager::GetInstance()->GetRenderer()->ChangeRendertarget(pRenderDef, pZBuffDef, mtxView, mtxProjection);
			}

			// フィードバックエフェクトにテクスチャ[0]を貼り付けて描画
			if (m_bDrawMultiScreen)
			{
				DrawMultiTargetScreen(0, NONE_ALPHACOLOR, NORMALSIZE);
			}
			

			// テクスチャ0と1の切替
			pTextureWk = m_Multitarget.pTextureMT[0];
			m_Multitarget.pTextureMT[0] = m_Multitarget.pTextureMT[1];
			m_Multitarget.pTextureMT[1] = pTextureWk;

			pRenderWk = m_Multitarget.pRenderMT[0];
			m_Multitarget.pRenderMT[0] = m_Multitarget.pRenderMT[1];
			m_Multitarget.pRenderMT[1] = pRenderWk;


			// デバッグ表示の描画処理
			CManager::GetInstance()->GetDebugProc()->Draw();

			// テキストの設定
			CDebugProc::SetText();

			// ポーズ描画処理
			if (bDisp)
			{
				CManager::GetInstance()->GetPause()->Draw();
			}

			// 黒フレーム
			CManager::GetInstance()->GetBlackFrame()->Draw();
		}
		else
			// ロードマネージャの更新
		{
			GetLoadManager()->Draw();
		}

		// 遷移なしフェード描画処理
		CInstantFade* pfalsefade = CManager::GetInstance()->GetInstantFade();
		if (pfalsefade != nullptr)
		{
			pfalsefade->Draw();
		}

		// フェード描画処理
		CFade* pfade = CManager::GetInstance()->GetFade();
		if (pfade != nullptr)
		{
			pfade->Draw();
		}

		// ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		// 描画終了
		m_pD3DDevice->EndScene();
	}


	// Imguiの描画
	ImguiMgr::Draw();

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//==========================================================================
// マルチターゲット画面の描画
//==========================================================================
void CRenderer::DrawMultiTargetScreen(int texIdx, const D3DXCOLOR& col, const D3DXVECTOR2& size)
{
	// 頂点情報へのポインタ
	VERTEX_2D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_Multitarget.pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
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

	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// 頂点バッファをアンロックロック
	m_Multitarget.pVtxBuff->Unlock();


	// 頂点バッファをデータストリームに設定
	m_pD3DDevice->SetStreamSource(0, m_Multitarget.pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	m_pD3DDevice->SetTexture(0, m_Multitarget.pTextureMT[texIdx]);

	// ポリゴンの描画
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//==========================================================================
// デバイスの取得
//==========================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice() const
{
	return m_pD3DDevice;
}

HRESULT CRenderer::SetFullScreen()
{
	m_pD3DDevice->Reset(&m_d3dpp);

	D3DDISPLAYMODE d3ddm;			// ディスプレイモード

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}


	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{// 失敗したとき
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));							// パラメータのゼロクリア

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	m_d3dpp.BackBufferCount = 1;									// バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デバイスバッファとして16bitを使う
	m_d3dpp.Windowed = TRUE;										// ウィンドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル


	ResetWnd();

	// Direct3Dデバイスの生成
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

	// 分解能を設定
	timeBeginPeriod(1);

	// ウインドウの表示
	ShowWindow(GetWnd(), GetCmbShow());		// ウインドウの表示状態を設定
	UpdateWindow(GetWnd());				// クライアント領域を更新

	return S_OK;
}

//==========================================================================
// デフォルトのレンダーターゲット取得
//==========================================================================
void CRenderer::GetDefaultRenderTarget(LPDIRECT3DSURFACE9* pRender, LPDIRECT3DSURFACE9* pZBuff, D3DXMATRIX* viewport, D3DXMATRIX* projection)
{
	// 現在のレンダリングターゲットを取得(保存)
	m_pD3DDevice->GetRenderTarget(0, &(*pRender));

	// 現在のZバッファを取得(保存)
	m_pD3DDevice->GetDepthStencilSurface(&(*pZBuff));

	m_pD3DDevice->GetTransform(D3DTS_VIEW, &(*viewport));
	m_pD3DDevice->GetTransform(D3DTS_PROJECTION, &(*projection));
}

//==========================================================================
// レンダリングターゲットの切替
//==========================================================================
void CRenderer::ChangeRendertarget(LPDIRECT3DSURFACE9 pRender, LPDIRECT3DSURFACE9 pZBuff, D3DXMATRIX viewport, D3DXMATRIX projection)
{
	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, pRender);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(pZBuff);

	// テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &viewport);

	// テクスチャレンダリング用のプロジェクションマトリックスを設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &projection);
}

//==========================================================================
// レンダリングターゲットの切替
//==========================================================================
void CRenderer::ChangeTarget(MyLib::Vector3 posV, MyLib::Vector3 posR, MyLib::Vector3 vecU)
{
	D3DXMATRIX mtxview, mtxProjection;


	// レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_Multitarget.pRenderMT[0]);

	// Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_Multitarget.pZBuffMT);

	// テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetViewport(&m_Multitarget.viewportMT);


	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(
		&mtxProjection,
		D3DXToRadian(45.0f),	// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,		// 手前の制限
		100000.0f);	// 奥行きの制限

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxview);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(
		&mtxview,
		&posV,	// 視点
		&posR,	// 注視点
		&vecU);	// 上方向ベクトル

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxview);
}