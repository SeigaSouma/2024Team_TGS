//=============================================================================
// 
// �X�v���C���w�b�_�[ [spline.h]
// Author : ���n�Ή�
// 
//=============================================================================

#ifndef _SPLINE_H_
#define _SPLINE_H_		// ��d�C���N���[�h�h�~

#include "calculation.h"

/**
@brief	�X�v���C��
*/
namespace MySpline
{
	/**
	@brief	�X�v���C����̈ʒu�擾
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	distance	[in]	�i�񂾋���
	@return	void
	*/
	inline MyLib::Vector3 GetSplinePosition(const std::vector<MyLib::Vector3>& posvec, float _distance)
	{
		int posvecSize = posvec.size();

		// �Z�O�����g�̒������v�Z
		std::vector<float> vecLength(posvecSize);
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// ����̃C���f�b�N�X(���[�v)
			int next = (i + 1) % posvecSize;

			// �_���m�̋���
			vecLength[i] = posvec[i].Distance(posvec[next]);

			// �S�Ă̒����v�Z
			totalLength += vecLength[i];
		}

		// ���������[�v������
		float distance = fmod(_distance, totalLength);

		// �ǂ̒n�_�ɂ��邩���v�Z
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

		// �������v�Z
		float ratio = distance / vecLength[idx];

		int n0 = (idx + (posvecSize - 1)) % posvecSize;
		int n1 = (n0 + 1) % posvecSize;
		int n2 = (n1 + 1) % posvecSize;
		int n3 = (n2 + 1) % posvecSize;

		return UtilFunc::Correction::CatmullRomSplineInterp(posvec[n0], posvec[n1], posvec[n2], posvec[n3], ratio);
	}
}

#endif