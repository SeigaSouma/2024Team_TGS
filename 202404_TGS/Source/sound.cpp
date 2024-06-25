//==========================================================================
//
// サウンド処理 [sound.cpp]
// Author : 相馬靜雅
//
//==========================================================================
#include "sound.h"
#include "calculation.h"

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CSound::SOUNDINFO CSound::m_aSoundInfo[LABEL_MAX] = 
{
	{ "data/BGM/title.wav", -1 },			// タイトル
	{ "data/BGM/BGM_game_000.wav", -1 },	// ゲーム
	{ "data/BGM/BGM_water_flow.wav", -1 },	// 水流
	{ "data/BGM/result.wav", -1 },			// リザルト
	{ "data/BGM/tutorial.wav", -1 },	// チュートリアル
	{ "data/BGM/ranking.wav", -1 },		// ランキング
	{ "data/SE/walk01.wav", 0 },			// 歩行1
	{ "data/SE/cursor_move.wav", 0 },			// カーソル移動
	{ "data/SE/cursor_end.wav", 0 },			// カーソル閉じ
	{ "data/SE/cancel01.wav", 0 },				// キャンセル1
	{ "data/SE/cancel02.wav", 0 },				// キャンセル2
	{ "data/SE/SE_drown.wav", -1 },				// 溺れ
	{ "data/SE/SE_collide.wav", 0 },				// ヒット
	{ "data/SE/SE_cracks_grass.wav", 0 },				// 画面割れ
	{ "data/SE/SE_breath_stream.wav", -1 },				// 息ループ

};	// サウンドの情報
CSound* CSound::m_pThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// マクロ定義
//==========================================================================
CSound::CSound()
{
	
}

//==========================================================================
// マクロ定義
//==========================================================================
CSound::~CSound()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSound* CSound::Create(HWND hWnd)
{
	if (m_pThisPtr != nullptr)
	{
		return m_pThisPtr;
	}

	m_pThisPtr = DEBUG_NEW CSound;
	if (m_pThisPtr != nullptr)
	{
		m_pThisPtr->Init(hWnd);
	}

	return m_pThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(m_pXAudio2 != nullptr)
		{
			// XAudio2オブジェクトの開放
			m_pXAudio2->Release();
			m_pXAudio2 = nullptr;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aSoundInfo[nCntSound].file.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = m_aSoundInfo[nCntSound].nCntLoop;
		SetVolume.dwFlags = DSBCAPS_CTRLVOLUME;		//音量調整のフラグ

		m_fVolume = 1.0f;

		//音量をセットする
		m_pMasteringVoice->SetVolume(1.0f);

		// オーディオバッファの登録
		m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		m_wfx[nCntSound] = wfx;

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSound::Uninit()
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != nullptr)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
	
			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = nullptr;
		}
	}
	
	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = nullptr;
	
	if(m_pXAudio2 != nullptr)
	{
		// XAudio2オブジェクトの開放
		m_pXAudio2->Release();
		m_pXAudio2 = nullptr;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//==========================================================================
// セグメント再生(再生中なら停止)
//==========================================================================
HRESULT CSound::PlaySound(LABEL label, bool stop)
{
#if 0
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = m_aSoundInfo[label].nCntLoop;

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	// 周波数リセット
	m_apSourceVoice[label]->SetFrequencyRatio(1.0f);

#else
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[label];
	buffer.pAudioData = m_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aSoundInfo[label].nCntLoop;

	if (m_apSourceVoice[label] != nullptr)
	{
		// 状態取得
		XAUDIO2_VOICE_STATE xa2state;
		m_apSourceVoice[label]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0 &&
			(buffer.LoopCount == -1 || stop))
		{// 再生中
			// 一時停止
			m_apSourceVoice[label]->Stop(0);

			// オーディオバッファの削除
			m_apSourceVoice[label]->FlushSourceBuffers();
		}
	}

	// ソースボイスを新しく作成して再生
	HRESULT hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[label], &(m_wfx[label].Format));
	if (FAILED(hr))
	{
		MessageBox(GetWnd(), "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
		return S_FALSE;
	}

	// オーディオバッファの登録
	m_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	m_apSourceVoice[label]->Start(0);

	// 周波数リセット
	m_apSourceVoice[label]->SetFrequencyRatio(1.0f);

	// 音量リセット
	m_apSourceVoice[label]->SetVolume(1.0f);

#endif
	return S_OK;

}

//==========================================================================
// セグメント停止(ラベル指定)
//==========================================================================
void CSound::StopSound(LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	if (m_apSourceVoice[label] == nullptr)
	{
		return;
	}

	// 状態取得
	m_apSourceVoice[label]->GetState(&xa2state);

	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[label]->FlushSourceBuffers();

		// 周波数リセット
		m_apSourceVoice[label]->SetFrequencyRatio(1.0f);
	}

	/*if (m_apSourceVoice[label] != nullptr)
	{
		m_apSourceVoice[label]->Stop(0);
		m_apSourceVoice[label]->FlushSourceBuffers();
		m_apSourceVoice[label]->DestroyVoice();
		m_apSourceVoice[label] = nullptr;
	}*/
}

//==========================================================================
// セグメント停止(全て)
//==========================================================================
void CSound::StopSound()
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < LABEL_MAX; nCntSound++)
	{
		if(m_apSourceVoice[nCntSound] != nullptr)
		{
			// 一時停止
			m_apSourceVoice[nCntSound]->Stop(0);

			// 周波数リセット
			m_apSourceVoice[nCntSound]->SetFrequencyRatio(1.0f);
		}

		/*if (m_apSourceVoice[nCntSound] != nullptr)
		{
			m_apSourceVoice[nCntSound]->Stop(0);
			m_apSourceVoice[nCntSound]->FlushSourceBuffers();
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = nullptr;
		}*/
	}
}

//==========================================================================
// チャンクのチェック
//==========================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//==========================================================================
// チャンクデータの読み込み
//==========================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, nullptr) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//==========================================================================
// 音量設定
//==========================================================================
void CSound::VolumeChange(LABEL label, float volume)
{
	if (m_apSourceVoice[label] == nullptr)
	{
		return;
	}
	UtilFunc::Transformation::ValueNormalize(volume, 2.0f, 0.0f);

	// 音量設定
	m_apSourceVoice[label]->SetVolume(volume);
}

//==========================================================================
// 音量調整(マスターボリューム設定)
//==========================================================================
void CSound::VolumeChange(float fVolume)
{
	m_fVolume += fVolume;
	UtilFunc::Transformation::ValueNormalize(m_fVolume, 2.0f, 0.0f);

	//音量をセットする
	m_pMasteringVoice->SetVolume(m_fVolume);
}

//==========================================================================
// 音量取得(3桁表示)
//==========================================================================
int CSound::GetVolume()
{
	return (int)((m_fVolume + 0.009) * 100);
}

//==========================================================================
// 周波数設定
//==========================================================================
void CSound::SetFrequency(LABEL label, float fValue)
{
	XAUDIO2_VOICE_STATE state;

	if (m_apSourceVoice[label] == nullptr){
		return;
	}

	// 状態取得
	m_apSourceVoice[label]->GetState(&state);

	if (state.BuffersQueued != 0)
	{// 再生中
		m_apSourceVoice[label]->SetFrequencyRatio(fValue);
	}
}
