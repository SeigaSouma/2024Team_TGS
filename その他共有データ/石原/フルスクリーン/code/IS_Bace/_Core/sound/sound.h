//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 石原颯馬
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//サウンドクラス
class CSound
{
public:
	//*****************************************************************************
	// サウンド一覧
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM_OUT = 0,	//アウトゲームBGM
		SOUND_LABEL_BGM_IN,			//インゲームBGM
		SOUND_LABEL_SE_SELECT,		//選択
		SOUND_LABEL_SE_JUMP,		//ジャンプ
		SOUND_LABEL_SE_ITEM,		//アイテム獲得
		SOUND_LABEL_SE_CHANGE,		//キャラ切替
		SOUND_LABEL_SE_MOVE,		//歩行
		SOUND_LABEL_MAX,
	} SOUND_LABEL;
	//*****************************************************************************

	//コンストラクタ・デストラクタ
	CSound();
	~CSound();

	//基本処理
	HRESULT Init(HWND hWnd);
	void Uninit(void);

	//流す処理
	HRESULT Play(SOUND_LABEL label);	//流す
	void Stop(SOUND_LABEL label);		//1つ止める
	void Stop(void);					//全部止める
	void Pause(SOUND_LABEL label);		//ポーズ
	void Restart(SOUND_LABEL label);	//再開
	
	//状態取得
	bool IsPlay(SOUND_LABEL label);		//流しているか
private:
	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
};

#endif
