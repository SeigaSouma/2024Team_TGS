//=============================================================================
// 
//  チェックポイント[checkpoint.h]
//  Author : 日野澤匠泉のプログラム(お借りしてません)
// 
//=============================================================================

#ifndef _CHECKPOINT_H_
#define _CHECKPOINT_H_		// 二重インクルード防止

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
//チェックポイントクラス
class CCheckpoint : public CObjectX
{
public:

	CCheckpoint(int nPriority = 6);
	~CCheckpoint();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// メンバ関数
	void Kill();		// 削除
	float GetLength() { return m_fLength; }	// 距離取得

	//=============================
	// 静的関数
	//=============================
	static CCheckpoint* Create(const float length);
	static void Load(const std::string filename);
	static int GetSaveID() { return m_nSaveID; }	// セーブID取得
	static void ResetSaveID() { m_nSaveID = -1; }	// セーブIDリセット
	static CListManager<CCheckpoint> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================

	//=============================
	// メンバ関数
	//=============================
	void SetLength(const float length);	// 距離設定

	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;				// 状態カウンター
	float m_fLength;				// 距離
	static CListManager<CCheckpoint> m_List;	// リスト

	static int m_nAll;		// 総数
	static int m_nSaveID;	// save id
	int m_MyIndex;			// 自分のID
};


#endif