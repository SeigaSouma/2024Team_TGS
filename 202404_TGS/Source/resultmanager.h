//=============================================================================
// 
//  リザルトマネージャヘッダー [resultmanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _RESULTMANAGER_H_
#define _RESULTMANAGER_H_	// 二重インクルード防止

#include "main.h"
#include "constans.h"
#include "judge.h"

// 前方宣言
class CEnemy;
class CBoss;

//==========================================================================
// クラス定義
//==========================================================================
// リザルトマネージャクラス定義
class CResultManager
{
public:

	CResultManager();
	~CResultManager();

	HRESULT Init();
	void Uninit();
	void Update();
	void Reset();	// リセット

	void SetJudgeRank(CJudge::JUDGE rank) { m_JudgeRank = rank; }	// 最終評価設定
	CJudge::JUDGE GetJudgeRank() { return m_JudgeRank; }			// 最終評価取得
	void SetClearTime(float time) { m_fClearTime = time; }			// クリア時間設定
	float GetClearTime() { return m_fClearTime; }					// クリア時間取得


	static CResultManager *Create();	// 生成処理
	static CResultManager* GetInstance() { return m_pThisPtr; }
	
private:

	CJudge::JUDGE m_JudgeRank;	// 最終評価
	float m_fClearTime;			// クリア時間
	static CResultManager* m_pThisPtr;	// 自身のポインタ
};



#endif