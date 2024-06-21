//==========================================================================
// 
// 変換処理 [convert.cpp]
// Author : 相馬靜雅
// 
//==========================================================================
#include "convert.h"

/**
	@brief	VtxInfoをVector3に変換
	@return	Vector3
*/
MyLib::Vector3 VtxInfoToVector3(const CCourse::VtxInfo& v)
{
	return v.pos;
}


/**
	@brief	Vector3をVtxInfoに変換
	@return	MyStruct
*/
CCourse::VtxInfo MyConvert::Vector3ToVtxInfo(const MyLib::Vector3& v)
{
	return CCourse::VtxInfo(v, 0.0f);
}
