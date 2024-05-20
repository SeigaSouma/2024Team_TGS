//=============================================================================
//
// avi処理 [avi.cpp]
// Author : ChatGPT
//
//=============================================================================

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "avi.h"
#include "main.h"

//=============================================================================
//コンストラクタ
//=============================================================================
CAvi::CAvi()
{
	m_graphBuilder = NULL;
	m_hwnd = NULL;
	m_mediaControl = NULL;
	m_pVMRCfg = NULL;
	m_vmr9Control = NULL;
	m_videoRenderer = NULL;
	m_rcVideo = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

//=============================================================================
//デストラクタ
//=============================================================================
CAvi::~CAvi()
{

}

//=============================================================================
//初期化処理
//=============================================================================
bool CAvi::Init(HWND hwnd)
{
	m_hwnd = hwnd;

	CoInitialize(NULL);

	HRESULT hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&m_graphBuilder);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_graphBuilder->QueryInterface(IID_IMediaControl, (void**)&m_mediaControl);
	if (FAILED(hr))
	{
		return false;
	}

	if (!SetVideoRenderer())
	{
		return false;
	}

	return true;
}

//=============================================================================
//終了処理
//=============================================================================
void CAvi::Uninit()
{
	if (m_mediaControl)
	{
		m_mediaControl->Stop();
		m_mediaControl->Release();
		m_mediaControl = NULL;
	}

	if (m_graphBuilder)
	{
		m_graphBuilder->Release();
		m_graphBuilder = NULL;
	}

	CoUninitialize();
}

//=============================================================================
//動画再生
//=============================================================================
void CAvi::PlayVideo(const WCHAR* filename)
{
	if (!m_graphBuilder || !m_mediaControl)
	{
		return;
	}

	HRESULT hr = m_graphBuilder->RenderFile(filename, NULL);
	if (FAILED(hr))
	{
		return;
	}

	// ビデオレンダラーフィルターの設定
	IBaseFilter* pRenderer = NULL;
	hr = m_graphBuilder->FindFilterByName(L"Video Mixing Renderer 9", &pRenderer);
	if (SUCCEEDED(hr))
	{
		IVMRWindowlessControl9* pWindowlessControl = NULL;
		hr = pRenderer->QueryInterface(IID_IVMRWindowlessControl9, (void**)&pWindowlessControl);
		if (SUCCEEDED(hr))
		{
			pWindowlessControl->SetVideoClippingWindow((HWND)m_hwnd);
			pWindowlessControl->SetAspectRatioMode(VMR9ARMode_None);
			pWindowlessControl->SetBorderColor(0x00000000); // 背景の透過色を指定（ここでは黒色）
			pWindowlessControl->SetVideoPosition(&m_rcVideo, &m_rcVideo); // 動画の表示位置とサイズを設定
			pWindowlessControl->Release();
		}
		pRenderer->Release();
	}

	m_mediaControl->Run();
}

//=============================================================================
//動画停止
//=============================================================================
void CAvi::StopVideo(const WCHAR* filename)
{
	//再生終了
	if (m_mediaControl)
	{
		m_mediaControl->Stop();
		m_mediaControl->Release();
		m_mediaControl = NULL;
	}

	if (m_graphBuilder)
	{
		m_graphBuilder->Release();
		m_graphBuilder = NULL;
	}
}

//=============================================================================
//VMR-9フィルターの設定
//=============================================================================
bool CAvi::SetVideoRenderer()
{
	IBaseFilter* pVideoRenderer = NULL;

	// VMR-9フィルターの作成
	HRESULT hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&pVideoRenderer);
	if (FAILED(hr))
	{
		return false;
	}

	// フィルターグラフにVMR-9フィルターを追加
	hr = m_graphBuilder->AddFilter(pVideoRenderer, L"Video Mixing Renderer 9");
	if (FAILED(hr))
	{
		pVideoRenderer->Release();
		return false;
	}

	// VMR-9ウィンドウレスコントロールのインターフェースを取得
	hr = pVideoRenderer->QueryInterface(IID_IVMRWindowlessControl9, (void**)&m_vmr9Control);
	if (FAILED(hr))
	{
		pVideoRenderer->Release();
		return false;
	}

	pVideoRenderer->Release();

	return true;
}

//=============================================================================
// 動画の表示位置とサイズを設定
//=============================================================================
void CAvi::SetVideoPosition(RECT* pSrcRect, RECT* pDstRect)
{
	if (m_vmr9Control)
	{
		m_vmr9Control->SetVideoPosition(pSrcRect, pDstRect);
	}
}