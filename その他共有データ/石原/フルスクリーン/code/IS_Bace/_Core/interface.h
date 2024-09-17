//======================================================
//
//インターフェース一覧[interface.h]
//Author:石原颯馬
//
//======================================================
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

//前方宣言
class CObject;

//当たり判定関係インターフェース
__interface ICollisionReader
{
	D3DXVECTOR3 GetMove(void) = 0;
	D3DXVECTOR3 GetPosOld(void) = 0;
	void SetPos(const D3DXVECTOR3 pos) = 0;
	float GetWidth(void) = 0;
	float GetHeight(void) = 0;
	float GetDepth(void) = 0;
	CObject* GetObj(void) = 0;
};

#endif // !_INTERFACE_H_
