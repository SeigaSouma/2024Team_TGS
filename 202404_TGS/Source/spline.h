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
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// 次回のインデックス(ループ)
			int next = (i + 1) % posvecSize;

			// 点同士の距離
			vecLength[i] = posvec[i].Distance(posvec[next]);

			// 全ての長さ計算
			totalLength += vecLength[i];
		}

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
}

#endif