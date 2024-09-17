//======================================================
//
//便利なものたち[Utility.cpp]
//Author:石原颯馬
//
//======================================================
#include "Utility.h"

namespace IS_Utility
{
	//=============================================================
	// 角度を-PI~PIに調整
	//=============================================================
	float FixRot(const float fRaw)
	{
		return fmodf(fRaw + (D3DX_PI * 3), D3DX_PI * 2) - D3DX_PI;
	}

	//=============================================================
	// ある頂点をもう一つの頂点を中心に回転する
	//=============================================================
	D3DXVECTOR3 RotateVtx(const D3DXVECTOR3 vtx, const D3DXVECTOR3 rot, const D3DXVECTOR3 posCenter)
	{
		//頂点の回転に必要な変数
		//各頂点を求めるのに必要な変数
		D3DXMATRIX mtxWorld;	//回転のベースとなるワールドマトリ
		D3DXMATRIX mtxRot;		//回転行列
		D3DXMATRIX mtxTrans;	//すべて変換後の行列
		D3DXVECTOR3 vtxTrans;	//変換後の点

		//頂点の回転
		//-mtx----------------------------------------------------------------------------------------------------------------------------
		//回転行列を作る
		D3DXMatrixIdentity(&mtxWorld);

		//向き反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxRot, &mtxWorld);

		//位置反映
		mtxWorld._41 = posCenter.x;
		mtxWorld._42 = posCenter.y;
		mtxWorld._43 = posCenter.z;

		//-mtx----------------------------------------------------------------------------------------------------------------------------

		//-pos0---------------------------------------------------------------------------------------------------------------------------
		//回転行列をもとに頂点を回転する
		//ベースをコピー
		mtxTrans = mtxWorld;

		//0度のときの点を置く
		D3DXVECTOR3 vtxPos = vtx;

		//回転行列とかけ合わせる
		D3DXVec3TransformCoord(&vtxTrans, &vtxPos, &mtxTrans);

		//-pos0---------------------------------------------------------------------------------------------------------------------------

		return vtxTrans;
	}

	//=============================================================
	// XZのみの外積計算
	//=============================================================
	float Vec2Closs(Vec3 a, Vec3 b)
	{
		return ((a.z * b.x) - (a.x * b.z));
	}

	//=============================================================
	// 外積計算(3D)
	// C++20にしてから↓こんなのが使えなくなったので代替
	// D3DXVec3Cross(&dust, &(pVtx[nVtx0].pos - pVtx[nVtx3].pos), &(posNew - (pVtx[nVtx3].pos + m_pos)))
	//=============================================================
	Vec3* Vec3Closs(Vec3 a, Vec3 b)
	{
		D3DXVECTOR3 dustBox;	//ゴミ箱。関数埋める用
		Vec3* pVec = D3DXVec3Cross(&dustBox, &a, &b);
		return pVec;
	}
}