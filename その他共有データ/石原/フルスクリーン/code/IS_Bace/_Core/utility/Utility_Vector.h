//======================================================
//
//便利なベクトル[Utility_Vector.h]
//Author:石原颯馬
//
//======================================================
#ifndef _UTILITY_VECTOR_H_
#define _UTILITY_VECTOR_H_

namespace IS_Utility
{
	//便利ベクトル
	struct Utility_Vector3 : public D3DXVECTOR3
	{
	public:
		Utility_Vector3();
		Utility_Vector3(float fx, float fy, float fz);

		Utility_Vector3 Normalize(void);
	private:
	};
	//長いので略
	typedef Utility_Vector3 Vector3;


	//便利向き
	struct Utility_Rotate3 : public D3DXVECTOR3
	{
	public:
		Utility_Rotate3();
		Utility_Rotate3(float fx, float fy, float fz);

		void FixRot(void);
	private:
	};
	//長いので略
	typedef Utility_Rotate3 Rotate3;
}

#endif // _UTILITY_VECTOR_H_
