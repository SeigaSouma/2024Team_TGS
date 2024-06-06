//=============================================================================
// 
//  レンダラーヘッダー [renderer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// 二重インクルード防止

#include "main.h"

//==========================================================================
// クラス定義
//==========================================================================
// レンダラークラス定義
class CRenderer
{
public:

	struct MultiTarget
	{
		LPDIRECT3DTEXTURE9 pTextureMT[2];	// レンダリングターゲット用テクスチャ
		LPDIRECT3DSURFACE9 pRenderMT[2];	// テクスチャレンダリング用インターフェース
		LPDIRECT3DSURFACE9 pZBuffMT;		// テクスチャレンダリング用Zバッファ
		D3DVIEWPORT9 viewportMT;			// テクスチャレンダリング用ビューポート
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// 頂点バッファ
	};

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice() const;
	LPDIRECT3D9 GetD3D() const { return m_pD3D; }

	HRESULT SetFullScreen();

	// マルチターゲット関連
	void GetDefaultRenderTarget(LPDIRECT3DSURFACE9* pRender, LPDIRECT3DSURFACE9* pZBuff, D3DXMATRIX* viewport, D3DXMATRIX* projection);	// デフォルトのレンダーターゲット取得
	void ChangeRendertarget(LPDIRECT3DSURFACE9 pRender, LPDIRECT3DSURFACE9 pZBuff, D3DXMATRIX viewport, D3DXMATRIX projection);	// ターゲット切明
	void ChangeTarget(MyLib::Vector3 posV, MyLib::Vector3 posR, MyLib::Vector3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(int idx) { return m_Multitarget.pTextureMT[idx]; }	// レンダリングターゲット用テクスチャ取得
	bool IsDrawMultiScreen() { return m_bDrawMultiScreen; };					// マルチターゲット画面の描画判定
	void SetEnableDrawMultiScreen(bool bDraw) { m_bDrawMultiScreen = bDraw; }	// マルチターゲット画面の描画判定

private:

	void ResetRendererState();
	void InitMTRender();	// マルチターゲットレンダラーの初期化

	void DrawMultiTargetScreen(int texIdx, const D3DXCOLOR& col, const D3DXVECTOR2& size);	// マルチターゲット画面の描画

	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ
	D3DPRESENT_PARAMETERS m_d3dpp;	// プレゼンテーションモード

	MultiTarget m_Multitarget;	// マルチターゲット用
	bool m_bDrawMultiScreen;	// マルチターゲット画面の描画判定
};



#endif