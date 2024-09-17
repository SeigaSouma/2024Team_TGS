//======================================================
//
//テクスチャ保存クラスのヘッダ[texture.h]
//Author:石原颯馬
//
//======================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#define MAX_STR		(256)
#define MAX_TEXTURE	(128)	//テクスチャ数

//テクスチャ保存クラス
class CTexture
{
public:
	//事前読み込みテクスチャ列挙
	enum PRELOAD
	{
		PRELOAD_00_TITLELOGO = 0,	//タイトルロゴ
		PRELOAD_01_STARTKB,			//スタート文字（KB）
		PRELOAD_02_STARTGP,			//スタート文字（GP）
		PRELOAD_03_NUMBER,			//数字
		PRELOAD_04_TIMERSTR,		//タイマー文字
		PRELOAD_05_SCORESTR,		//スコア文字
		PRELOAD_06_BONUSSTR,		//ボーナス文字
		PRELOAD_07_SCENERANKKB,		//ランキング遷移文字（KB）
		PRELOAD_08_SCENERANKGP,		//ランキング遷移文字（GP）
		PRELOAD_09_RANKSTR,			//ランキング文字
		PRELOAD_10_RANK_01,			//ランキング（1位～）
		PRELOAD_11_RANK_02,
		PRELOAD_12_RANK_03,
		PRELOAD_13_RANK_04,
		PRELOAD_14_RANK_05,			//ランキング（～5位）
		PRELOAD_15_SCENETITLEKB,	//タイトル遷移文字（KB）
		PRELOAD_16_SCENETITLEGP,	//タイトル遷移文字（GP）
		PRELOAD_17_TUTORIAL_01,		//チュートリアル
		PRELOAD_18_TUTORIAL_02,
		PRELOAD_19_TUTORIAL_03,
		PRELOAD_20_TUTORIAL_04,
		PRELOAD_28_TUTORIAL_05_KB,	//移動（KB）
		PRELOAD_29_TUTORIAL_06_KB,	//切替（KB）
		PRELOAD_30_TUTORIAL_05_GP,	//移動（GP）
		PRELOAD_31_TUTORIAL_06_GP,	//切替（GP）
		PRELOAD_21_PAUSE_STR,		//ポーズ文字
		PRELOAD_22_PAUSE_RESUME,	//ゲームに戻る
		PRELOAD_23_PAUSE_RESTART,	//再スタート
		PRELOAD_24_PAUSE_EXIT,		//タイトルに戻る
		PRELOAD_25_PAUSE_SCREENSHOT,//スクショ
		PRELOAD_26_BG_01,			//背景1
		PRELOAD_27_BG_02,			//背景2
		PRELOAD_32_POINT,			//操作中ポイント
		PRELOAD_MAX
	};

	//パターンテクスチャ構造体定義
	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTexture;	//テクスチャポインタ
		int nPatternWidth;				//パターン幅
		int nPatternHeight;				//パターン高さ
	} PatternTexture;
	//コンストラクタ・デストラクタ
	CTexture();
	~CTexture();

	//全読み込み・破棄
	HRESULT Load(const char* pPath);	//あらかじめ読み込みたいテクスチャ読み込み
	void Unload(void);					//追加した分も含めてすべて破棄

	int Regist(const char* pPath, const int nPatWidth = 1, const int nPatHeight = 1);	//追加する分の読み込み
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);											//テクスチャ取得
	int GetPatWidth(int nIdx);					//パターン幅取得
	int GetPatHeight(int nIdx);					//パターン高さ取得

private:
	PatternTexture m_apTexture[MAX_TEXTURE];		//テクスチャポインタ
	char m_aFilepath[MAX_TEXTURE][MAX_STR];			//テクスチャパス
	static int m_nNumAll;							//テクスチャ総数
};

#endif // !_TEXTURE_H_
