//=============================================================================
// 
//  エフェクシアヘッダー [MyEffekseer.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _MYEFFEKSEER_H_
#define _MYEFFEKSEER_H_	// 二重インクルード防止

#include <EffekseerRendererDX9.h>
#include <XAudio2.h>
#include <d3d9.h>
#include <functional>
#include <wrl/client.h>
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// エフェクシアクラス定義
class CMyEffekseer
{
public:
	
	struct MyEffekseerInfo
	{

		MyLib::Vector3 pos;
		MyLib::Vector3 rot;
		float scale;
		MyLib::Vector3 move;		// 移動量
		Effekseer::EffectRef efcRef;
		Effekseer::Handle handle;	// エフェクトのオブジェクト
		bool bAutoDeath;			// 自動削除のフラグ
	};

	enum EFKLABEL
	{
		EFKLABEL_SAMPLE_LASER = 0,	// サンプルのレーザー
		EFKLABEL_BRESS,				// 息
		EFKLABEL_RIVER_SAMPLE,		// 川サンプル
		EFKLABEL_IMPACT,			// 衝撃
		EFKLABEL_MAX
	};

	template <typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	CMyEffekseer();
	~CMyEffekseer();

	void Uninit();
	void Update();

	/**
	@brief	エフェクトの設定
	@details パスの拡張子は[.efkefc]です
	@param	efkpath		[in]	読み込むエフェクトのパス
	@param	pos			[in]	位置
	@param	rot			[in]	向き
	@param	move		[in]	移動量
	@param	scale		[in]	拡大率
	@param	bAutoDeath	[in]	自動削除
	@return	Effekseer::Handle
	*/
	Effekseer::Handle SetEffect(EFKLABEL label, MyLib::Vector3 pos, MyLib::Vector3 rot, MyLib::Vector3 move, float scale, bool bAutoDeath = true);
	Effekseer::Handle SetEffect(Effekseer::Handle **pHandle, EFKLABEL label, MyLib::Vector3 pos, MyLib::Vector3 rot, MyLib::Vector3 move, float scale, bool bAutoDeath = true);
	void SetPosition(Effekseer::Handle handle, MyLib::Vector3 pos);	// 位置設定
	void SetRotation(Effekseer::Handle handle, MyLib::Vector3 rot);	// 向き設定
	void SetMatrix(Effekseer::Handle handle, D3DXMATRIX mtx);	// マトリックス設定
	void SetScale(Effekseer::Handle handle, float scale);	// スケール設定
	Effekseer::Matrix43 GetMatrix(Effekseer::Handle handle);	// マトリックス取得
	void SetTransform(Effekseer::Handle handle, MyLib::Vector3 pos, MyLib::Vector3 rot);	// マトリックス設定
	bool IsDeath(Effekseer::Handle handle);	// 死亡フラグ
	void StopAll();	// 全て停止
	void Stop(Effekseer::Handle handle);	// 停止
	void SetTrigger(Effekseer::Handle handle, int idx);

	static CMyEffekseer* GetInstance() { return m_pMyEffekseer; }	// インスタンス取得
	static CMyEffekseer* Create();								// インスタンス生成

private:

	// メンバ関数
	HRESULT Init();
	void Draw();
	void SetupEffekseerModules(::Effekseer::ManagerRef efkManager);	// モジュールのセットアップ
	::EffekseerRendererDX9::RendererRef GetEffekseerRenderer() { return efkRenderer; }	// エフェクシアのレンダラー取得
	Effekseer::EffectRef LoadEffect(std::string efkpath);
	
	// メンバ変数
	int32_t time = 0;
	Effekseer::Handle efkHandle = 0;
	EffekseerRendererDX9::RendererRef efkRenderer;
	Effekseer::ManagerRef efkManager;
	Effekseer::Vector3D viewerPosition;
	Effekseer::Matrix44 projectionMatrix;
	Effekseer::Matrix44 cameraMatrix;
	std::function<void()> onLostDevice;
	std::function<void()> onResetDevice;

	// 自作変数
	Effekseer::EffectRef m_LoadEffect;				// 読み込んだエフェクトのスマートポインタ
	std::vector<MyEffekseerInfo> m_EffectObj;		// エフェクトのオブジェクト
	std::vector<Effekseer::Handle> m_Handle;		// エフェクトのハンドル
	static std::string m_EffectName[EFKLABEL_MAX];	// エフェクトのファイル名
	static CMyEffekseer* m_pMyEffekseer;			// 自身のポインタ
};



#endif