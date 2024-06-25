//=============================================================================
// 
//  水中石マネージャヘッダー [waterstoneManager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _WATERSTONE_MANAGER_H_
#define _WATERSTONE_MANAGER_H_		// 二重インクルード防止

#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水中石マネージャクラス
class CWaterStone_Manager
{
public:

	//=============================
	// 構造体定義
	//=============================
	// 石情報
	struct SStoneInfo
	{
		MyLib::Vector3 pos;
		MyLib::Vector3 rot;

		SStoneInfo() : pos(), rot() {}
		SStoneInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot) : pos(_pos), rot(_rot) {}
	};


	CWaterStone_Manager();
	~CWaterStone_Manager();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();

	//=============================
	// メンバ関数
	//=============================
	void Kill();	// 削除

	// 入出力
	void Save();	// セーブ
	void Load();	// ロード

	//=============================
	// 静的関数
	//=============================
	static CWaterStone_Manager* Create();	// 生成
	static CWaterStone_Manager* GetInstance() { return m_ThisPtr; }	// インスタンス取得
private:

	//=============================
	// メンバ関数
	//=============================
	void CreateStone(const std::vector<SStoneInfo>& vecinfo);	// 石生成

	//=============================
	// メンバ変数
	//=============================
	static CWaterStone_Manager* m_ThisPtr;		// 自身のポインタ
};


#endif