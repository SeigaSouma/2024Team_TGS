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
		TYPE_TMP_YMDHOUSE,
		TYPE_TMP_FLOWER,
		TYPE_MAX
	};

	//=============================
	// 荷物情報
	//=============================
	struct SBaggageInfo
	{
		std::string path;	// モデルファイルパス
		float weight;		// 重さ
		float width;		// ぶれ幅
		float cycle;		// 周期

		SBaggageInfo() : path(""), weight(0.0f), width(0.0f), cycle(0.0f) {}
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
	bool IsAway() { return m_bAway; }	// 吹っ飛び判定
	void SetModelPath(const std::string path) { m_baggageInfo.path = path; }
	void SetCycle(const float& deviation) { m_baggageInfo.cycle = deviation; }
	void SetWidth(const float& deviWidth) { m_baggageInfo.width = deviWidth; }
	void SetWeight(const float& weight) { m_baggageInfo.weight = weight; }
	bool IsHit() { return m_bHit; }		//Hit()での判定を返す

	//=============================
	// 静的関数
	//=============================
	static CBaggage* Create(TYPE type, SBaggageInfo info);
	static CListManager<CBaggage> GetListObj() { return m_List; }	// リスト取得

private:

	
	//=============================
	// メンバ関数
	//=============================
	bool Hit();
	void DeadMove();	// 死亡時移動

	//=============================
	// メンバ変数
	//=============================
	TYPE m_type;				// 種類
	float m_fWeight;			// 重さ
	MyLib::Vector3 m_force;		// 力
	MyLib::Vector3 m_velorot;	// 回転速度
	MyLib::Vector3 m_posAwayStart;		// 吹っ飛びスタート地点
	bool m_bLand;				// 着地判定
	bool m_bHit;				// 障害物衝突判定
	bool m_bAway;				// 吹っ飛び判定
	static CListManager<CBaggage> m_List;	// リスト
	SBaggageInfo m_baggageInfo;
	float m_fDeviation;
	float m_fDeadMoveTime;	// 死亡時の移動時間
};


#endif