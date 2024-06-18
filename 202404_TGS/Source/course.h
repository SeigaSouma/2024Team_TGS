//=============================================================================
// 
//  コースヘッダー [course.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _COURSE_H_
#define _COURSE_H_	// 二重インクルード防止

#include "object3DMesh.h"
#include "collisionLine_Box.h"

//==========================================================================
// クラス定義
//==========================================================================
// コースクラス
class CCourse : public CObject3DMesh
{

public:

	//=============================
	// 構造体定義
	//=============================
	// 辺の内部情報
	struct LineInfo
	{
		MyLib::Vector3 pos;	// 位置
		MyLib::Vector3 rot;	// 向き
		float width;		// 幅

		LineInfo() : pos(0.0f), rot(0.0f), width(0.0f) {}
		LineInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot, float _width) : pos(_pos), rot(_rot), width(_width) {}
	};

	// 辺情報
	struct Line
	{
		LineInfo lineInfo;	// 辺の内部情報
	};

	// 頂点情報
	struct VtxInfo
	{
		MyLib::Vector3 pos;	// 位置
		MyLib::Vector3 rot;	// 向き

		VtxInfo() : pos(), rot() {}
		VtxInfo(const MyLib::Vector3& _pos, const MyLib::Vector3& _rot) : pos(_pos), rot(_rot) {}
	};

	CCourse(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CCourse();

	// オーバーライドされた関数
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Reset();	// リセット
	void ReCreateVtx();	// リセット
	void CalVtxPosition();	// 各頂点計算

	// 情報取得・設定
	CCollisionLine_Box* GetCollisionLineBox(int idx);								// 当たり判定ボックス取得
	std::vector<MyLib::Vector3> GetVecPosition() { return m_vecSegmentPosition; }	// 基点の位置取得
	MyLib::Vector3 GetVecPosition(int idx);											// 基点の位置取得
	void SetVecPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecSegmentPosition = vecpos; }
	void SetVecPosition(int idx, const MyLib::Vector3& pos);						// 基点の位置設定
	float GetCourceLength() { return m_courceLength; }								// コースの長さ
	static float GetCreateDistance() { return m_fCreateDistance; }					// 生成間隔取得

	std::vector<VtxInfo> GetVecVtxinfo() { return m_vecVtxInfo; }						// 各頂点の情報取得
	VtxInfo GetVecVtxinfo(int idx);														// 各頂点の情報取得
	void SetVecVtxinfo(const std::vector<VtxInfo>& vecinfo) { m_vecVtxInfo = vecinfo; }	// 各頂点の情報設定
	void SetVecVtxinfo(int idx, const VtxInfo& vecinfo);								// 各頂点の情報設定

	// ファイル操作
	void Save();	// セーブ
	HRESULT Load(const std::string& file);	// ロード

	static CCourse *Create(const std::string& file);	// 生成

private:

	//=============================
	// メンバ関数
	//=============================
	void SetVtxPosition();	// 頂点座標

	//=============================
	// メンバ変数
	//=============================
	std::string m_FileName;	// ファイル名
	std::vector<MyLib::Vector3> m_vecSegmentPosition;	// 基点の位置
	std::vector<VtxInfo> m_vecVtxInfo;		// 各頂点の情報
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// 当たり判定ボックス
	float m_courceLength;
	static const float m_fCreateDistance;	// 生成間隔
};



#endif