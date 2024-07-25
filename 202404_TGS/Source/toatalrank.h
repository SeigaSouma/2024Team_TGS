//=============================================================================
// 
//  トータルランクヘッダー [toatalrank.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _TOATALRANK_H_
#define _TOATALRANK_H_	// 二重インクルード防止

#include "object2d.h"
#include "judge.h"

//==========================================================================
// クラス定義
//==========================================================================
// トータルランククラス
class CToatalRank : public CObject2D
{
public:

	//=============================
	// 状態列挙
	//=============================
	enum State
	{
		STATE_NONE = 0,		// なにもない
		STATE_MAX
	};

	CToatalRank(int nPriority = 3);
	~CToatalRank();

	//=============================
	// オーバーライド関数
	//=============================
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	//=============================
	// 静的関数
	//=============================
	static CToatalRank* Create(CJudge::JUDGE rank, float time);

private:

	//=============================
	// メンバ関数
	//=============================
	// 状態系

	// その他関数
	CJudge::JUDGE CalRank(CJudge::JUDGE rank, float time);	// ランク計算

	//=============================
	// メンバ変数
	//=============================
	// 状態系
	float m_fStateTime;		// 状態カウンター
	State m_state;			// 状態

	// その他
	CJudge::JUDGE m_Rank;	// ランク

};


#endif