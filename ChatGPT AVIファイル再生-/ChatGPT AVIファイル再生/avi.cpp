//=============================================================================
//
// avi���� [avi.cpp]
// Author : ChatGPT
//
//=============================================================================

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "avi.h"
#include "main.h"

//=============================================================================
//�R���X�g���N�^
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
//�f�X�g���N�^
//=============================================================================
CAvi::~CAvi()
{

}

//=============================================================================
//����������
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
//�I������
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
//����Đ�
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

	// �r�f�I�����_���[�t�B���^�[�̐ݒ�
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
			pWindowlessControl->SetBorderColor(0x00000000); // �w�i�̓��ߐF���w��i�����ł͍��F�j
			pWindowlessControl->SetVideoPosition(&m_rcVideo, &m_rcVideo); // ����̕\���ʒu�ƃT�C�Y��ݒ�
			pWindowlessControl->Release();
		}
		pRenderer->Release();
	}

	m_mediaControl->Run();
}

//=============================================================================
//�����~
//=============================================================================
void CAvi::StopVideo(const WCHAR* filename)
{
	//�Đ��I��
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
//VMR-9�t�B���^�[�̐ݒ�
//=============================================================================
bool CAvi::SetVideoRenderer()
{
	IBaseFilter* pVideoRenderer = NULL;

	// VMR-9�t�B���^�[�̍쐬
	HRESULT hr = CoCreateInstance(CLSID_VideoMixingRenderer9, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&pVideoRenderer);
	if (FAILED(hr))
	{
		return false;
	}

	// �t�B���^�[�O���t��VMR-9�t�B���^�[��ǉ�
	hr = m_graphBuilder->AddFilter(pVideoRenderer, L"Video Mixing Renderer 9");
	if (FAILED(hr))
	{
		pVideoRenderer->Release();
		return false;
	}

	// VMR-9�E�B���h�E���X�R���g���[���̃C���^�[�t�F�[�X���擾
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
// ����̕\���ʒu�ƃT�C�Y��ݒ�
//=============================================================================
void CAvi::SetVideoPosition(RECT* pSrcRect, RECT* pDstRect)
{
	if (m_vmr9Control)
	{
		m_vmr9Control->SetVideoPosition(pSrcRect, pDstRect);
	}
}