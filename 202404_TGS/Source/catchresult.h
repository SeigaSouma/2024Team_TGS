//=============================================================================
// 
// キャッチ結果処理 [catchresult.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _CATCHRESULT_H_
#define _CATCHRESULT_H_		// 二重インクルード防止のマクロを定義する

#include "object2D.h"

//==========================================================================
// クラス定義
//==========================================================================
// キャッチ結果
class CCatchResult : public CObject2D
{
public:

	CCatchResult(int nPriority = 8);
	~CCatchResult();

	//  オーバーライドされた関数
	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVtx();

	static CCatchResult* Create(const MyLib::Vector3& pos, bool bClear);

private:

	// 状態列挙
	enum State
	{
		STATE_EXPANSION = 0,
		STATE_EXPNONE,
		STATE_FADEOUT,
		STATE_MAX
	};

	//=============================
	// 関数リスト
	//=============================
	typedef void(CCatchResult::* STATE_FUNC)();	// 状態処理のリスト
	static STATE_FUNC m_StateFuncList[];

	//=============================
	// メンバ関数
	//=============================
	void StateExpansion();	// 拡大
	void StateExpNone();	// 拡大後何もしない
	void StateFadeOut();	// フェードアウト状態

	//=============================
	// メンバ変数
	//=============================
	State m_state;			// 状態
	float m_fStateTimer;	// 状態タイマー
};

#endif