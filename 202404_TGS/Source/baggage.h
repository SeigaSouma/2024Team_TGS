//=============================================================================
// 
//  荷物ヘッダー [baggage.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SAMPLE_OBJX_H_
#define _SAMPLE_OBJX_H_		// 二重インクルード防止

#include "objectX.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// 荷物クラス
class CBaggage : public CObjectX
{
public:
	
	//=============================
	// 列挙型定義
	//=============================
	enum TYPE
	{
		TYPE_CLOTH = 0,	// 布
		TYPE_MAX
	};

	CBaggage(int nPriority = 6);
	~CBaggage();

	//=============================
	// オーバーライド関数
	//=============================
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	//=============================
	// メンバ関数
	//=============================
	void Kill();		// 削除

	//=============================
	// 静的関数
	//=============================
	static CBaggage *Create(TYPE type);
	static CListManager<CBaggage> GetListObj() { return m_List; }	// リスト取得

private:

	
	//=============================
	// メンバ関数
	//=============================
	// その他

	//=============================
	// メンバ変数
	//=============================
	TYPE m_type;		// 種類
	float m_fWeight;	// 重さ
	static CListManager<CBaggage> m_List;	// リスト

};


#endif