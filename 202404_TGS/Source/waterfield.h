//=============================================================================
// 
//  水ヘッダー [waterfield.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _WATERFIELD_H_
#define _WATERFIELD_H_	// 二重インクルード防止

#include "object3DMesh.h"

//==========================================================================
// クラス定義
//==========================================================================
// 水クラス定義
class CWaterField : public CObject3DMesh
{
public:

	CWaterField(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CWaterField();

	// オーバーライドされた関数
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	static CWaterField *Create();

private:

	float m_fTexU;	// Uスクロール用
	float m_fTexV;	// Vスクロール用
};



#endif