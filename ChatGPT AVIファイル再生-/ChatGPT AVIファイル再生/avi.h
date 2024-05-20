//=========================================================================================
//
// avi処理 [avi.h]
// Author : takumi hinosawa
//
//=========================================================================================
#ifndef _AVI_H_
#define _AVI_H_

//*****************************************************************************************
// インクルード
//*****************************************************************************************
#include "main.h"
#include <dshow.h>
#pragma comment(lib, "strmiids.lib")
#include "Vmr9.h"

//*****************************************************************************************
// AVIクラス
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
	HWND m_hwnd;                                // ウィンドウハンドル
	IGraphBuilder* m_graphBuilder;              // フィルターグラフマネージャー
	IMediaControl* m_mediaControl;              // メディアコントロールインターフェース
	IBaseFilter* m_videoRenderer;				// ビデオレンダラーフィルター
	IVMRFilterConfig *m_pVMRCfg;
	RECT m_rcVideo;
	IVMRWindowlessControl9* m_vmr9Control;
	bool SetVideoRenderer();
};

#endif
