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

	//=============================
	// 列挙型定義
	//=============================
	enum Sample
	{
		SAMPLE_WAO = 0,
		SAMPLE_WHO,
		SAMPLE_MAX
	};

	CCheckpoint(int nPriority = 6);
	~CCheckpoint();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// メンバ関数
	void Kill();		// 削除

	//=============================
	// 静的関数
	//=============================
	static CCheckpoint* Create(MyLib::Vector3 pos);
	static int GetSaveID() { return m_nSaveID; }	// セーブID取得
	static CListManager<CCheckpoint> GetListObj() { return m_List; }	// リスト取得

private:

	//=============================
	// 関数リスト
	//=============================
	typedef void(CCheckpoint::* CHECKPOINT_FUNC)();
	static CHECKPOINT_FUNC m_CheckpointFuncList[];	// 関数のリスト

	//=============================
	// メンバ関数
	//=============================
	// 状態関数
	void SampleWho();	// フー
	void SampleWao();	// ワオ

	//=============================
	// メンバ変数
	//=============================
	float m_fStateTime;				// 状態カウンター
	Sample m_state;					// 状態
	static CListManager<CCheckpoint> m_List;	// リスト

	static int m_nAll;		// 総数
	static int m_nSaveID;	// save id
	int m_MyIndex;			// 自分のID
};


#endif