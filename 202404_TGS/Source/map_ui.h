//=============================================================================
//
// マップUIヘッダー [map_ui.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _MAP_UI_H_
#define _MAP_UI_H_	// 二重インクルード防止

#include "object.h"

// 前方宣言
class CObjectX;
class CCamera;
class CPlayer;

//==========================================================================
// クラス定義
//==========================================================================
class CMapUI : public CObject
{
private:

	// 使用オブジェクト列挙型
	enum TYPE
	{
		TYPE_MAP = 0,	// マップ
		TYPE_PLAYER,	// プレイヤー
		TYPE_MAX
	};

private:

	CMapUI();	// コンストラクタ
	~CMapUI();	// デストラクタ

public:

	// 静的メンバ関数
	static CMapUI* Create();	// 生成
	static CMapUI* GetInstance() { return m_pInstance; }	// インスタンス取得
	static void Release();	// 開放
	
	// メンバ関数
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// 設定
	void BindPlayer(CPlayer* pPlayer) { m_pMyPlayer = pPlayer; }

private:

	// 静的メンバ変数
	static CMapUI* m_pInstance;	// シングルトン用インスタンス
	static std::string m_aModelList[TYPE::TYPE_MAX];	// オブジェクトファイル名

	// メンバ関数
	void SetMapRotation();	// マップの回転

	// メンバ変数
	float m_fRatio;			// ゴールまでの割合
	CObjectX* m_apObj[TYPE::TYPE_MAX];	// オブジェクト
	CCamera* m_pMyCamera;	// カメラ
	CPlayer* m_pMyPlayer;	// プレイヤー
};

#endif