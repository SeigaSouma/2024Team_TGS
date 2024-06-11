//=============================================================================
// 
//  荷物ヘッダー [baggage.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _BAGGAGE_H_
#define _BAGGAGE_H_		// 二重インクルード防止

#include "objectQuaternion.h"
#include "listmanager.h"

//==========================================================================
// クラス定義
//==========================================================================
// 荷物クラス
class CBaggage : public CObjectQuaternion
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

	CBaggage(int nPriority = 4);
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
	void SetForce(const MyLib::Vector3& power) { m_force = power; }				// 力設定
	void AddForce(const MyLib::Vector3& power, const MyLib::Vector3& ActPos);	// 力追加
	bool IsLand() { return m_bLand; }	// 着地判定
	void SetAddDeviation(const float& deviation) { m_fAddDeviation = deviation; }
	void SetDeviationWidth(const float& deviWidth) { m_fDeviationWidth = deviWidth; }

	//=============================
	// 静的関数
	//=============================
	static CBaggage *Create(TYPE type);
	static CListManager<CBaggage> GetListObj() { return m_List; }	// リスト取得

private:

	
	//=============================
	// メンバ関数
	//=============================
	bool Hit();
	// その他

	//=============================
	// メンバ変数
	//=============================
	TYPE m_type;			// 種類
	float m_fWeight;		// 重さ
	MyLib::Vector3 m_force;	// 力
	MyLib::Vector3 m_velorot;	// 回転速度
	bool m_bLand;			// 着地判定
	static CListManager<CBaggage> m_List;	// リスト
	float m_fDeviation;		// 現在のぶれ
	float m_fAddDeviation;	// 加算するぶれ
	float m_fDeviationWidth;// ぶれ幅

};


#endif