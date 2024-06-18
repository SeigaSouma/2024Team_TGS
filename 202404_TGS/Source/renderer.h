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
private:

	// マルチターゲットレンダリング用情報
	struct MultiTargetInfo
	{
		float fStartColAlpha;	// 開始透明度
		float fStartMulti;		// 開始サイズ倍率
		float fAddTimer;		// タイマー増加数
		float fColAlpha;		// 透明度
		float fTimer;			// 遷移タイマー
		float fMulti;			// サイズ倍率
		bool bActive;			// 稼働中

		MultiTargetInfo()	// コンストラクタ
		{
			// 値のクリア
			fStartColAlpha = 0.0f;
			fStartMulti = 0.0f;
			fAddTimer = 0.0f;
			fColAlpha = 0.0f;
			fMulti = 0.0f;
			fTimer = 0.0f;
			bActive = false;
		}
	};

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

	/**
	@brief	マルチターゲット画面の描画判定	
	@param	fGoalAlpha	[in]	目標透明色
	@param	fGoalMulti	[in]	目標画面倍率
	@param	fTimer		[in]	目標までの時間
	@return	void
	*/
	void SetEnableDrawMultiScreen(float fGoalAlpha, float fGoalMulti, float fTimer);

private:

	void ResetRendererState();
	void InitMTRender();	// マルチターゲットレンダラーの初期化
	void SetMultiTarget();

	void DrawMultiTargetScreen(int texIdx, const D3DXCOLOR& col, const D3DXVECTOR2& size);	// マルチターゲット画面の描画

	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ
	D3DPRESENT_PARAMETERS m_d3dpp;	// プレゼンテーションモード
	MultiTargetInfo m_MultitargetInfo;
	MultiTarget m_Multitarget;	// マルチターゲット用
};



#endif