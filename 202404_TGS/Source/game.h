//=============================================================================
// 
//  ゲームヘッダー [game.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_	// 二重インクルード防止

#include "scene.h"
#include "gamemanager.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPlayer;
class CScore;
class CTimer;
class CPowerGauge;
class CStage;
class CHitScore;
class CLimitArea;
class CPlayer;
class CEdit;
class CMap_ObstacleManager;

//==========================================================================
// クラス定義
//==========================================================================
// ゲームクラス定義
class CGame : public CScene
{
public:

	// エディットタイプ
	enum EditType
	{
		EDITTYPE_OFF = 0,		// オフ
		EDITTYPE_ENEMYBASE,		// 敵拠点
		EDITTYPE_MAP,			// マップ
		EDITTYPE_OBSTACLE,		// 障害物
		EDITTYPE_MAX
	};

	CGame();
	~CGame();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	void ResetBeforeBoss();
	EditType GetEditType() { return m_EditType; }
	CScore *GetScore();
	CTimer *GetTimer() { return m_pTimer; }	// タイマー取得
	CStage *GetStage();
	CGameManager *GetGameManager();
	void SetEnableClear(bool bClear) { m_clear = bClear; }		// クリアのフラグ
	bool IsClearFrag() { return m_clear; }						// クリアフラグの取得
	bool IsEdit() { return m_bEdit; }							// エディットの判定
	float GetRockOnDistance() { return m_fMaxRokOnDistance; }	// ロックオンの最大距離取得

	// 静的関数
	static CGame* GetInstance();	// インスタンス取得
	static CGame* Create(CScene::MODE mode);		// 生成処理

protected:

	virtual void InitByMode();	// モード別初期化


	//=============================
	// メンバ関数
	//=============================
	void EditReset();
	void ChangeEdit();	// エディット切り替え処理

	//=============================
	// メンバ変数
	//=============================
	CScore *m_pScore;						// スコアのオブジェクト
	CTimer *m_pTimer;						// タイマーのオブジェクト
	CLimitArea *m_pLimitArea;				// エリア制限のオブジェクト
	CStage *m_pStage;						// ステージのオブジェクト
	CGameManager *m_pGameManager;			// ゲームマネージャのオブジェクト
	EditType m_EditType;					// エディットの種類
	CEdit* m_pEdit;							// エディター
	CMap_ObstacleManager* m_pObstacleManager;	// 障害物マネージャ
	bool m_bEdit;				// エディットの判定
	bool m_clear; // クリア判定
	float m_fMaxRokOnDistance;			// ロックオンの最大距離

	static CGame* m_pThisPtr;	// 自身のポインタ
};



#endif