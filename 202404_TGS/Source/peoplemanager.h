//=============================================================================
// 
//  人マネージャヘッダー [peoplemanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PEOPLEMANAGER_H_
#define _PEOPLEMANAGER_H_	// 二重インクルード防止

#include "judge.h"

//==========================================================================
// 前方宣言
//==========================================================================
class CPeople;

//==========================================================================
// クラス定義
//==========================================================================
// 人マネージャクラス
class CPeopleManager
{
public:

	// 構造体定義
	struct SPeopleData
	{
		int nType;			// キャラクター種類
		MyLib::Vector3 pos;	// 位置
	};

	struct SPattern
	{
		int nNum;	// 総数
		int nRank;	// ランク
		std::vector<SPeopleData> data;
	};

	// 列挙型定義
	enum STATE
	{
		STATE_NONE = 0,		// 何もない状態
		STATE_MAX
	};

	CPeopleManager();
	~CPeopleManager();

	HRESULT Init();
	void Uninit();
	void Update();

	static CPeopleManager *Create();
	HRESULT ReadText(const std::string& filename);	// 外部ファイル読み込み処理
	void SetPeople(MyLib::Vector3 pos, MyLib::Vector3 rot, int nPattern);	// 敵配置
	void SetByRank();	// ランクごとのセット処理

	void SetRank(const CJudge::JUDGE& judge) { m_Rank = judge; }			// ランク設定

	static CPeopleManager* GetInstance() { return m_ThisPtr; }				// 自身のポインタ


private:

	// メンバ変数
	CJudge::JUDGE m_Rank;							// 現在のランク
	CJudge::JUDGE m_OldRank;						// 前回のランク
	std::vector<SPattern> m_vecPattern;				// 配置パターン
	std::vector<SPattern> m_PatternByRank[CJudge::JUDGE::JUDGE_MAX];			// 配置パターン
	std::vector<std::string> m_vecMotionFileName;	// モーションファイル名
	static CPeopleManager* m_ThisPtr;				// 自身のポインタ
};



#endif