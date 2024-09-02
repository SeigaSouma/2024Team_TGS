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
	@brief	�Z�O�����g�̒����v�Z�i���[�v�j
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	pVecLength	[inout]	�e�Z�O�����g�̒���
	@return	�Z�O�����g�̒���
	*/
	inline float CalSegmentLength(const std::vector<MyLib::Vector3>& posvec, std::vector<float> *pVecLength)
	{
		int posvecSize = posvec.size();

		// �Z�O�����g�̒������v�Z
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// ����̃C���f�b�N�X(���[�v)
			int next = (i + 1) % posvecSize;

			// �_���m�̋���
			(*pVecLength)[i] = posvec[i].Distance(posvec[next]);

			// �S�Ă̒����v�Z
			totalLength += (*pVecLength)[i];
		}

		return totalLength;
	}

	/**
	@brief	�Z�O�����g�̒����v�Z�i�񃋁[�v�j
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	pVecLength	[inout]	�e�Z�O�����g�̒���
	@param	endDistance	[in]	�I�[�̒���
	@return	�Z�O�����g�̒���
	*/
	inline float CalSegmentLength_NonLoop(const std::vector<MyLib::Vector3>& posvec, std::vector<float>* pVecLength, float endDistance)
	{
		int posvecSize = posvec.size();

		// �Z�O�����g�̒������v�Z
		float totalLength = 0.0f;
		for (int i = 0; i < posvecSize; ++i)
		{
			// ����̃C���f�b�N�X(���[�v)
			int next = (i + 1) % posvecSize;

			// �_���m�̋���
			if (next == 0)
			{
				(*pVecLength)[i] = endDistance;
			}
			else
			{
				(*pVecLength)[i] = posvec[i].Distance(posvec[next]);
			}

			// �S�Ă̒����v�Z
			totalLength += (*pVecLength)[i];
		}

		return totalLength;
	}

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
		float totalLength = CalSegmentLength(posvec, &vecLength);

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

	/**
	@brief	�X�v���C����̈ʒu�擾�i�񃋁[�v�j
	@param	posvec		[in]	�S�Ă̒n�_(vector)
	@param	distance	[in]	�i�񂾋���
	@param	endDistance	[in]	�I�[�̒���
	@return	void
	*/
	inline MyLib::Vector3 GetSplinePosition_NonLoop(const std::vector<MyLib::Vector3>& posvec, float _distance, float endDistance = 10.0f, int* pIdx = nullptr)
	{
		int posvecSize = posvec.size();

		// �Z�O�����g�̒������v�Z
		std::vector<float> vecLength(posvecSize);
		float totalLength = CalSegmentLength_NonLoop(posvec, &vecLength, endDistance);

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

		if (pIdx != nullptr)
		{
			*pIdx = idx;
		}

		// �������v�Z
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