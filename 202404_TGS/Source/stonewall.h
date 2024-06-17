//=============================================================================
// 
//  石垣ヘッダー [stonewall.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _STONEWALL_H_
#define _STONEWALL_H_	// 二重インクルード防止

#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// 石垣クラス
class CStoneWall : public CObject3DMesh
{

public:

	CStoneWall(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CStoneWall();

	// オーバーライドされた関数
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Reset();	// リセット
	void CalVtxPosition();	// 各頂点計算
	void BindVtxPosition();	// 頂点座標割り当て

	// 情報取得・設定
	std::vector<MyLib::Vector3> GetVecPosition() { return m_vecSegmentPosition; }	// 基点の位置取得
	MyLib::Vector3 GetVecPosition(int idx);											// 基点の位置取得
	void SetVecPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecSegmentPosition = vecpos; }
	void SetVecPosition(int idx, const MyLib::Vector3& pos);						// 基点の位置設定
	std::vector<MyLib::Vector3> GetVecVtxPosition() { return m_vecVtxPosition; }	// 各頂点の位置取得
	MyLib::Vector3 GetVecVtxPosition(int idx);										// 各頂点の位置取得
	void SetVecVtxPosition(const std::vector<MyLib::Vector3>& vecpos) { m_vecVtxPosition = vecpos; }
	void SetVecVtxPosition(int idx, const MyLib::Vector3& pos);						// 各頂点の位置設定

	static CStoneWall *Create();	// 生成

private:

	// メンバ変数
	std::vector<MyLib::Vector3> m_vecSegmentPosition;	// 基点の位置
	std::vector<MyLib::Vector3> m_vecVtxPosition;		// 各頂点の位置
	float m_courceLength;
};



#endif