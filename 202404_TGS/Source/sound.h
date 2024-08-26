//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//==========================================================================
// クラス定義
//==========================================================================
// サウンドクラス
class CSound
{
public:

	// 列挙型定義
	enum LABEL
	{
		LABEL_BGM_TITLE = 0,			// タイトル
		LABEL_BGM_GAME,					// ゲーム
		LABEL_BGM_WATER_FLOW,			// 水流
		LABEL_BGM_RESULT,				// リザルト
		LABEL_BGM_TUTORIAL,				// 観客
		LABEL_BGM_RANKING,				// 観客
		LABEL_SE_WALK1,					// 歩行1
		LABEL_SE_CURSOR_MOVE,			// カーソル移動
		LABEL_SE_CURSOR_END,			// カーソル閉じ
		LABEL_SE_CANCEL1,				// キャンセル1
		LABEL_SE_CANCEL2,				// キャンセル2
		LABEL_SE_DROWN,				// 溺れ
		LABEL_SE_HIT,				// ヒット
		LABEL_SE_CRACK_GRASS,				// 画面割れ
		LABEL_SE_BRESS_STREAM,				// 息ループ
		LABEL_SE_OKSD_000,
		LABEL_SE_OKSD_001,
		LABEL_SE_OKSD_002,
		LABEL_SE_SEGA_000,
		LABEL_SE_SEGA_001,
		LABEL_SE_SEGA_002,
		LABEL_SE_HNSW_000,
		LABEL_SE_HNSW_001,
		LABEL_SE_HNSW_002,
		LABEL_SE_KARAKURI,		// からくりサーカス

		LABEL_MAX,
	};

	typedef struct
	{
		std::string file;	// ファイル名
		int nCntLoop;		// ループカウント
	} SOUNDINFO;

	CSound();
	~CSound();

	HRESULT Init(HWND hWnd);
	void Uninit();
	HRESULT PlaySound(LABEL label, bool stop = false);
	void StopSound();
	void StopSound(LABEL label);
	void VolumeChange(LABEL label, float volume);	// 音量設定
	void VolumeChange(float fVolume);				// マスターボリューム設定
	int GetVolume();
	void SetFrequency(LABEL label, float fValue);	// 周波数設定

	static CSound* Create(HWND hWnd);	// 生成処理
	static CSound* GetInstance() { return m_pThisPtr; }

private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[LABEL_MAX];						// オーディオデータ
	DWORD m_aSizeAudio[LABEL_MAX];						// オーディオデータサイズ
	DSBUFFERDESC SetVolume;								// 音量調整
	float m_fVolume;									// 音量
	static SOUNDINFO m_aSoundInfo[LABEL_MAX];			// サウンドの情報
	WAVEFORMATEXTENSIBLE m_wfx[LABEL_MAX];
	static CSound* m_pThisPtr;	// 自身のポインタ
};


#endif
