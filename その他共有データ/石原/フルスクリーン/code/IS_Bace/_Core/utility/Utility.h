//======================================================
//
//便利なものたち[Utility.h]
//Author:石原颯馬
//
//======================================================
#ifndef _UTILITY_H_
#define _UTILITY_H_

//警告 
#define STRING2(x) #x
#define STRING(x) STRING2(x)
#define PutWarning(x) __pragma(message(__FILE__ "(" STRING(__LINE__) "):warning:" x));

namespace IS_Utility
{
	//=============================================================
	// 入力がめんどくさいものを再定義
	//=============================================================
	typedef D3DXVECTOR2 Vec2;
	typedef D3DXVECTOR3 Vec3;
	typedef D3DCOLOR Color;

	//=============================================================
	// 打ち込みめんどいもの
	//=============================================================
	const D3DXVECTOR2 VEC2_ZERO = D3DXVECTOR2(0.0f,0.0f);			//vec2型の0
	const D3DXVECTOR3 VEC3_ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//vec3型の0

	//=============================================================
	// 数学系で使えそうなもの
	//=============================================================
	const float ACCELERATION_GRAVITY = 9.8f;	//重力加速度
	const float PI = D3DX_PI;					//1PI[rad]
	const float TWO_PI = 2.0f * D3DX_PI;		//2PI[rad]

	//=============================================================
	// 方向
	//=============================================================
	const float LEFT_UP		= -0.75f * D3DX_PI;	//左上
	const float RIGHT_UP	= 0.75f * D3DX_PI;	//右上
	const float LEFT_DOWN	= -0.25f * D3DX_PI;	//左下
	const float RIGHT_DOWN	= 0.25f * D3DX_PI;	//右下
	const float UP			= 1.0f * D3DX_PI;	//真上
	const float DOWN		= 0.0f * D3DX_PI;	//真下
	const float LEFT		= -0.5f * D3DX_PI;	//左
	const float RIGHT		= 0.5f * D3DX_PI;	//右

	//=============================================================
	// 計算関数
	//=============================================================
	//*************************************************************
	// 数値を設定した範囲内に調整
	//*************************************************************
	template<typename T> inline T Clamp(T num, T max, T min)
	{
		if (num >= max)
		{//最大値以上なら最大値
			return max;
		}
		else if (num <= min)
		{//最低値以下なら最低値
			return min;
		}
		else
		{//最大と最低の間
			return num;
		}
	}

	//*************************************************************
	// その他
	//*************************************************************
	float FixRot(const float fRaw);
	D3DXVECTOR3 RotateVtx(const D3DXVECTOR3 vtx, const D3DXVECTOR3 rot, const D3DXVECTOR3 posCenter);
	inline float fatof(const char* str) { return (float)atof(str); }
	inline float Pythagoras(float fa, float fb) { return sqrtf(powf(fa, 2) + powf(fb, 2)); }
	float Vec2Closs(Vec3 a, Vec3 b);
	Vec3* Vec3Closs(Vec3 a, Vec3 b);
}

#endif // !_UTILITY_H_
