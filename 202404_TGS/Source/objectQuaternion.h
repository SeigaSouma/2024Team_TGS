//=============================================================================
// 
//  クォータニオンヘッダー [objectQuaternion.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECTQUATERNION_H_
#define _OBJECTQUATERNION_H_	// 二重インクルード防止

#include "objectX.h"

//==========================================================================
// クラス定義
//==========================================================================
// クォータニオンクラス定義
class CObjectQuaternion : public CObjectX
{
public:

	CObjectQuaternion(int nPriority = mylib_const::PRIORITY_DEFAULT);
	~CObjectQuaternion();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual HRESULT Init(const std::string& file);
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	// 情報設定
	void SetValueRot(float rot) { m_fValueRot = rot; }	// 回転角設定
	float GetValueRot() { return m_fValueRot; }			// 回転角取得

	static CObjectQuaternion *Create();	// 生成

private:

	// メンバ関数
	void CalWorldMtx();		// ワールドマトリックスの計算処理

	// メンバ変数
	float m_fRotDest;			// 半径
	D3DXQUATERNION m_quaternion;	// クォータニオン
	MyLib::Vector3 m_vecAxis;		// 回転軸
	float m_fValueRot;				// 回転角
	MyLib::Matrix m_RotationMtx;	// 計算用マトリックス宣言

};



#endif