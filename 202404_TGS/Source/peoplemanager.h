//=============================================================================
// 
//  人マネージャヘッダー [peoplemanager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _PEOPLEMANAGER_H_
#define _PEOPLEMANAGER_H_	// 二重インクルード防止

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
	struct SEnemyData
	{
		int nType;			// キャラクター種類
		MyLib::Vector3 pos;	// 位置
	};

	struct SPattern
	{
		int nNumEnemy;	// 敵の数
		std::vector<SEnemyData> enemydata;
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

protected:


private:

	std::vector<SPattern> m_vecPattern;				// 配置パターン
	std::vector<std::string> m_vecMotionFileName;	// モーションファイル名
};



#endif