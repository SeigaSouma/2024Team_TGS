//======================================================
//
//背景（2D）処理のヘッダ[bg.h]
//Author:石原颯馬
//
//======================================================
#ifndef _BG_H_	//多重インクルード対策
#define _BG_H_
#include "..\..\_Core\_object\object_2d\object2D.h"

class CBG : public CObject2D
{
public:
	//コンストラクタ・デストラクタ
	CBG(int nPriority = PRIORITY_BG);
	virtual ~CBG();

	//生成
	static CBG* Create(const int nPriority = PRIORITY_BG);

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

private:
	
};

#endif	//多重