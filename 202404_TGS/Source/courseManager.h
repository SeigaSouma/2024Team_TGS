//=============================================================================
// 
//  コースマネージャヘッダー [courseManager.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _COURSEMANAGER_H_
#define _COURSEMANAGER_H_		// 二重インクルード防止

#include "listmanager.h"
class CMapBlock;

//==========================================================================
// クラス定義
//==========================================================================
// コースマネージャクラス
class CCourseManager
{
public:

	CCourseManager();
	~CCourseManager();

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

	std::vector<std::vector<MyLib::Vector3>> GetSegmentPos() { return m_vecAllSegmentPos; }
	std::vector<MyLib::Vector3> GetSegmentPos(int idx) { return m_vecAllSegmentPos[idx]; }
	void SetSegmentPos(const std::vector<std::vector<MyLib::Vector3>>& vec) { m_vecAllSegmentPos = vec; }
	void SetSegmentPos(const std::vector<MyLib::Vector3>& vec, int idx) { m_vecAllSegmentPos[idx] = vec; }
	int GetSegmentSize() { return static_cast<int>(m_vecAllSegmentPos.size()); }	// 基点のサイズ


	//=============================
	// 静的関数
	//=============================
	static CCourseManager* Create();	// 生成
	static CCourseManager* GetInstance() { return m_ThisPtr; }	// インスタンス取得
	static const float GetBlockLength() { return m_fBlockLength; }	// ブロックの長さ取得
private:

	//=============================
	// メンバ変数
	//=============================
	std::vector<std::vector<MyLib::Vector3>> m_vecAllSegmentPos;	// 全ての起点の位置
	static CCourseManager* m_ThisPtr;		// 自身のポインタ
	static const float m_fBlockLength;	// ブロックの長さ
};


#endif