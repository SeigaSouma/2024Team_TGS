//======================================================
//
//オブジェクト（ビルボード）処理のヘッダ[objectBillboard.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECT_BILLBOARD_H_	//多重インクルード対策
#define _OBJECT_BILLBOARD_H_

#include "..\object_3d\object3D.h"

//ビルボードクラス
class CObjectBillboard : public CObject3D
{
public:
	//コンストラクタ・デストラクタ
	CObjectBillboard(int nPriority = PRIORITY_DEFAULT);		//デフォルト
	virtual ~CObjectBillboard();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObjectBillboard* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nIdxTexture);
};

#endif	//多重