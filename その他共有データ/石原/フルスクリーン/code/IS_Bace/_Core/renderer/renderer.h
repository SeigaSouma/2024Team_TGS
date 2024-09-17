//======================================================
//
//レンダリング処理のヘッダ[renderer.h]
//Author:石原颯馬
//
//======================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

class CRenderer
{
public:
	//ブレンディング種類列挙
	typedef enum
	{
		BLENDTYPE_NORMAL = 0,	//通常
		BLENDTYPE_ADD,			//加算	
		BLENDTYPE_SUB,			//減算
		BLENDTYPE_MAX
	} BLENDTYPE;
	enum class DISPLAYMODE
	{
		MODE_WINDOW,
		MODE_FULLSCREEN,
	};

	//コンストラクタ・デストラクタ
	CRenderer();
	~CRenderer();

	//基本処理
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//設定
	void SetViewport(float x, float y, float width, float height);
	void SetBlendType(BLENDTYPE type);
	void SetZEnable(const bool bEnable);
	void SetEnableZTest(bool bEnable);
	void SetEnableAlplaTest(bool bEnable);

	//取得
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	void SwitchDisplayMode(DISPLAYMODE mode);

private:
	void ResetDevice();

	LPDIRECT3D9 m_pD3D;						//Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Direct3Dデバイスへのポインタ
	DISPLAYMODE m_dispMode;					//ディスプレイモード
	D3DPRESENT_PARAMETERS m_d3dpp;			//デバイスリセット時に使用するプレゼンテーションパラメータ
	D3DPRESENT_PARAMETERS m_d3dppWindow;	//デバイスリセット時に使用するプレゼンテーションパラメータ
	D3DPRESENT_PARAMETERS m_d3dppFull;		//デバイスリセット時に使用するプレゼンテーションパラメータ
	bool m_bResetWait;						//デバイスリセット待ち状態
	HWND m_hWnd;
};

#endif // !_RENDERER_H_