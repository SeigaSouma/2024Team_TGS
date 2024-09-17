//======================================================
//
//マネージャのヘッダ[manager.h]
//Author:石原颯馬
//
//======================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//前方宣言
class CInputKeyboard;
class CInputMouse;
class CInputGamePad;
class CRenderer;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;

//シーンクラス
class CScene
{
public:
	//モード列挙
	typedef enum
	{
		MODE_TITLE = 0,	//タイトル
		MODE_TUTORIAL,	//チュートリアル
		MODE_GAME,		//ゲーム
		MODE_RESULT,	//リザルト
		MODE_RANKING,	//ランキング
		MODE_MAX
	} MODE;
	
	//コンストラクタ・デストラクタ
	CScene();
	~CScene();

	//基本処理
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void)  = 0;

	//生成
	static CScene* Create(MODE mode);

	//取得
	MODE GetMode(void) { return m_mode; }

private:
	MODE m_mode;	//モード
};
class CManager
{
public:
	//コンストラクタ・デストラクタ
	CManager();
	~CManager();

	//基本処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	//FPS計測
	void CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime);

	//取得
	static CManager* GetInstance(void);
	CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }
	CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	CInputGamePad* GetInputGamePad(void) { return m_pInputPad; }
	CSound* GetSound(void) { return m_pSound; }
	CRenderer* GetRenderer(void) { return m_pRenderer; }
	CDebugProc* GetDebProc(void) { return m_pDebProc; }
	CCamera* GetCamera(void) { return m_pCamera; }
	CLight* GetLight(void) { return m_pLight; }
	CTexture* GetTexture(void) { return m_pTexture; }

	//破棄
	static HRESULT Release(void);

	//シーン系
	void SetMode(CScene::MODE mode);
	CScene::MODE GetMode(void) { return m_pScene->GetMode(); }
	CScene* GetScene(void) { return m_pScene; }

	//更新設定・取得
	void SetEnableUpdate(const bool bEnable) { m_bEnableUpdate = bEnable; }
	bool GetEnableUpdate(void) { return m_bEnableUpdate; }

private:
	//変数
	static CManager* m_pManager;			//マネージャ（シングルトン）
	CInputKeyboard* m_pInputKeyboard;		//キーボード
	CInputMouse* m_pInputMouse;				//マウス
	CInputGamePad* m_pInputPad;				//ゲームパッド
	CSound* m_pSound;						//サウンド
	CRenderer* m_pRenderer;					//レンダラー
	CDebugProc* m_pDebProc;					//デバッグ
	CCamera* m_pCamera;						//カメラ
	CLight* m_pLight;						//ライト
	CTexture* m_pTexture;					//テクスチャ
	int m_nFPS;								//FPS
	DWORD m_dwFrameCount;					//回したフレーム数
	CScene* m_pScene;						//シーン
	bool m_bEnableUpdate;					//オブジェ更新するか
	bool m_bWindowed;
};

#endif