//=============================================================================
// 
// スプラインヘッダー [spline.h]
// Author : 相馬靜雅
// 
//=============================================================================

#ifndef _SPLINE_H_
#define _SPLINE_H_		// 二重インクルード防止

#include "calculation.h"

/**
@brief	スプライン
*/
namespace MySpline
{
	/**
	@brief	セグメントの長さ計算（ループ）
	@param	posvec		[in]	全ての地点(vector)
	@param	pVecLength	[inout]	各セグメントの長さ
	@return	セグメントの長さ
	*/
	inline float CalSegmentLength(const std::vector<MyLib::Vector3>& posvec, std::vector<float> *pVecLength)
	{
		int posvecSize = posvec.size();

		// セグメントの長さを計算
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// 次回のインデックス(ループ)
			int next = (i + 1) % posvecSize;

			// 点同士の距離
			(*pVecLength)[i] = posvec[i].Distance(posvec[next]);

			// 全ての長さ計算
			totalLength += (*pVecLength)[i];
		}

		return totalLength;
	}

	/**
	@brief	セグメントの長さ計算（非ループ）
	@param	posvec		[in]	全ての地点(vector)
	@param	pVecLength	[inout]	各セグメントの長さ
	@param	endDistance	[in]	終端の長さ
	@return	セグメントの長さ
	*/
	inline float CalSegmentLength_NonLoop(const std::vector<MyLib::Vector3>& posvec, std::vector<float>* pVecLength, float endDistance)
	{
		int posvecSize = posvec.size();

		// セグメントの長さを計算
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// 次回のインデックス(ループ)
			int next = (i + 1) % posvecSize;

			// 点同士の距離
			if (next == 0)
			{
				(*pVecLength)[i] = endDistance;
			}
			else
			{
				(*pVecLength)[i] = posvec[i].Distance(posvec[next]);
			}

			// 全ての長さ計算
			totalLength += (*pVecLength)[i];
		}

		return totalLength;
	}

	/**
	@brief	スプライン上の位置取得
	@param	posvec		[in]	全ての地点(vector)
	@param	distance	[in]	進んだ距離
	@return	void
	*/
	inline MyLib::Vector3 GetSplinePosition(const std::vector<MyLib::Vector3>& posvec, float _distance)
	{
		int posvecSize = posvec.size();

		// セグメントの長さを計算
		std::vector<float> vecLength(posvecSize);
		float totalLength = CalSegmentLength(posvec, &vecLength);

		// 距離をループさせる
		float distance = fmod(_distance, totalLength);

		// どの地点にいるかを計算
		int idx = 0;
		while (1)
		{
			if (distance < vecLength[idx])
			{
				break;
			}
			distance -= vecLength[idx];
			idx = (idx + 1) % posvecSize;
		}

		// 割合を計算
		float ratio = distance / vecLength[idx];

		int n0 = (idx + (posvecSize - 1)) % posvecSize;
		int n1 = (n0 + 1) % posvecSize;
		int n2 = (n1 + 1) % posvecSize;
		int n3 = (n2 + 1) % posvecSize;

		return UtilFunc::Correction::CatmullRomSplineInterp(posvec[n0], posvec[n1], posvec[n2], posvec[n3], ratio);
	}

	/**
	@brief	スプライン上の位置取得（非ループ）
	@param	posvec		[in]	全ての地点(vector)
	@param	distance	[in]	進んだ距離
	@param	endDistance	[in]	終端の長さ
	@return	void
	*/
	inline MyLib::Vector3 GetSplinePosition_NonLoop(const std::vector<MyLib::Vector3>& posvec, float _distance, float endDistance = 10.0f, int* pIdx = nullptr)
	{
		int posvecSize = posvec.size();

		// セグメントの長さを計算
		std::vector<float> vecLength(posvecSize);
		float totalLength = CalSegmentLength_NonLoop(posvec, &vecLength, endDistance);

		// 距離をループさせる
		float distance = fmod(_distance, totalLength);

		// どの地点にいるかを計算
		int idx = 0;
		while (1)
		{
			if (distance < vecLength[idx])
			{
				break;
			}
			distance -= vecLength[idx];
			idx = (idx + 1) % posvecSize;
		}

		if (pIdx != nullptr)
		{
			*pIdx = idx;
		}

		// 割合を計算
		float ratio = distance / vecLength[idx];

		int n0 = (idx + (posvecSize - 1)) % posvecSize;
		int n1 = (n0 + 1) % posvecSize;
		int n2 = (n1 + 1) % posvecSize;
		int n3 = (n2 + 1) % posvecSize;

		MyLib::Vector3 pos2 = posvec[n2], pos3 = posvec[n3];
		if (idx >= posvecSize - 1)
		{
			pos2 = posvec[n1] * 1.1f;
			pos3 = posvec[n1] * 1.2f;
		}

		return UtilFunc::Correction::CatmullRomSplineInterp(posvec[n0], posvec[n1], pos2, pos3, ratio);
	}

}

#endif