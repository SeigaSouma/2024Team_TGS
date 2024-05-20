//=========================================================================================
//
// avi���� [avi.h]
// Author : takumi hinosawa
//
//=========================================================================================
#ifndef _AVI_H_
#define _AVI_H_

//*****************************************************************************************
// �C���N���[�h
//*****************************************************************************************
#include "main.h"
#include <dshow.h>
#pragma comment(lib, "strmiids.lib")
#include "Vmr9.h"

//*****************************************************************************************
// AVI�N���X
//*****************************************************************************************
class CAvi
{
public:
	CAvi();
	~CAvi();

	bool Init(HWND hwnd);
	void Uninit();
	void PlayVideo(const WCHAR* filename);
	void StopVideo(const WCHAR* filename);
	void SetVideoPosition(RECT* pSrcRect, RECT* pDstRect);

private:
	HWND m_hwnd;                                // �E�B���h�E�n���h��
	IGraphBuilder* m_graphBuilder;              // �t�B���^�[�O���t�}�l�[�W���[
	IMediaControl* m_mediaControl;              // ���f�B�A�R���g���[���C���^�[�t�F�[�X
	IBaseFilter* m_videoRenderer;				// �r�f�I�����_���[�t�B���^�[
	IVMRFilterConfig *m_pVMRCfg;
	RECT m_rcVideo;
	IVMRWindowlessControl9* m_vmr9Control;
	bool SetVideoRenderer();
};

#endif
