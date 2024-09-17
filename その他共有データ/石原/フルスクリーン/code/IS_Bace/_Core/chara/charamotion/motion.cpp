//======================================================
//
//���[�V��������[motion.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "motion.h"
#include "..\charaparts\charaparts.h"
#include "..\..\utility\Utility.h"

//========================
//�R���X�g���N�^
//========================
CMotion::CMotion()
{
	for (int cntInfo = 0; cntInfo < INFO_NUM; cntInfo++)
	{
		m_aInfo[cntInfo].m_pKeyInfo = nullptr;
		m_aInfo[cntInfo].m_bLoop = false;
		m_aInfo[cntInfo].m_nNumKey = 0;
	}
}

//========================
//�f�X�g���N�^
//========================
CMotion::~CMotion()
{
}

//========================
//����������
//========================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//========================
//�I������
//========================
void CMotion::Uninit(void)
{
	for (int cntInfo = 0; cntInfo < INFO_NUM; cntInfo++)
	{
		for (int cntKeyInfo = 0; cntKeyInfo < m_aInfo[cntInfo].m_nNumKey; cntKeyInfo++)
		{
			if (m_aInfo[cntInfo].m_pKeyInfo[cntKeyInfo].m_pKey != nullptr)
			{
				delete[] m_aInfo[cntInfo].m_pKeyInfo[cntKeyInfo].m_pKey;
				m_aInfo[cntInfo].m_pKeyInfo[cntKeyInfo].m_pKey = nullptr;
			}
		}
		delete[] m_aInfo[cntInfo].m_pKeyInfo;
		m_aInfo[cntInfo].m_pKeyInfo = nullptr;
	}
}

//========================
//���[�V�����ݒ菈��
//========================
void CMotion::Set(int nType)
{
	//���[�V�����ݒ�
	m_nType = nType;

	//�����ݒ�
	m_bLoop = m_aInfo[m_nType].m_bLoop;
	m_nNumKey = m_aInfo[m_nType].m_nNumKey;
	m_nKey = 0;
	m_nCounter = 0;
	m_bFinish = false;
}

//========================
//�X�V����
//========================
void CMotion::Update(void)
{
	//���̃L�[�Ǝ��̃L�[�����Ă���
	int nNowKey = m_nKey;
	int nNextKey = (m_nKey + 1) % m_nNumKey;

	if (m_bFinish == false)
	{
		for (int CntModel = 0; CntModel < m_nNumModel; CntModel++)
		{
			//�I�t�Z�b�g�����Ă���
			D3DXVECTOR3 posMotioned = m_ppModel[CntModel]->GetPos();
			D3DXVECTOR3 rotMotioned = m_ppModel[CntModel]->GetRot();

			//�����Z�o
			float posDiffX = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].pos.x -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.x;
			float posDiffY = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].pos.y -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.y;
			float posDiffZ = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].pos.z -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.z;
			float rotDiffX = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].rot.x -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.x;
			float rotDiffY = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].rot.y -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.y;
			float rotDiffZ = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].rot.z -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.z;

			//�ʒu�����Z�o
			float posDemandX = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.x +
				posDiffX * ((float)m_nCounter / m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_nFrame);
			float posDemandY = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.y +
				posDiffY * ((float)m_nCounter / m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_nFrame);
			float posDemandZ = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.z +
				posDiffZ * ((float)m_nCounter / m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_nFrame);
			float rotDemandX = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.x +
				rotDiffX * ((float)m_nCounter / m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_nFrame);
			float rotDemandY = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.y +
				rotDiffY * ((float)m_nCounter / m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_nFrame);
			float rotDemandZ = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.z +
				rotDiffZ * ((float)m_nCounter / m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_nFrame);

			//�p�[�c�̈ʒu�����ݒ�
			posMotioned += D3DXVECTOR3(posDemandX, posDemandY, posDemandZ);
			rotMotioned += D3DXVECTOR3(rotDemandX, rotDemandY, rotDemandZ);
			m_ppModel[CntModel]->SetPosMotioned(posMotioned);
			m_ppModel[CntModel]->SetRotMotioned(rotMotioned);
		}
		m_nCounter++;

		//�Đ��t���[����̋���
		if (m_nCounter == m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_nFrame)
		{//�Đ��t���[�����ɒB������
		 //�J�E���^�[�����Z�b�g���ăL�[������₷
			m_nCounter = 0;
			m_nKey++;

			//�L�[�̍ő吔�ɒB�����烋�[�v���邩�ۂ��ɉ����čĐݒ肷��
			if (m_bLoop == true)
			{//���[�v����
				if (m_nKey == m_nNumKey)
				{
					m_nKey = 0;
				}
			}
			else
			{//���[�v�Ȃ�
				if (m_nKey == m_nNumKey - 1)
				{
					m_bFinish = true;
				}
			}
		}
	}
	else
	{//�Ō�̃��[�V�������g�p
		for (int CntModel = 0; CntModel < m_nNumModel; CntModel++)
		{
			nNextKey = m_nNumKey - 1;
			nNowKey = nNextKey - 1;

			//�I�t�Z�b�g�����Ă���
			D3DXVECTOR3 posMotioned = m_ppModel[CntModel]->GetPos();
			D3DXVECTOR3 rotMotioned = m_ppModel[CntModel]->GetRot();

			//�����Z�o
			float posDiffX = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].pos.x -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.x;
			float posDiffY = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].pos.y -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.y;
			float posDiffZ = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].pos.z -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.z;
			float rotDiffX = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].rot.x -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.x;
			float rotDiffY = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].rot.y -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.y;
			float rotDiffZ = m_aInfo[m_nType].m_pKeyInfo[nNextKey].m_pKey[CntModel].rot.z -
				m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.z;

			//�ʒu�����Z�o
			float posDemandX = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.x + posDiffX;
			float posDemandY = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.y + posDiffY;
			float posDemandZ = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.z + posDiffZ;
			float rotDemandX = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.x + rotDiffX;
			float rotDemandY = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.y + rotDiffY;
			float rotDemandZ = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.z + rotDiffZ;

			//�p�[�c�̈ʒu�����ݒ�
			posMotioned += D3DXVECTOR3(posDemandX, posDemandY, posDemandZ);
			rotMotioned += D3DXVECTOR3(rotDemandX, rotDemandY, rotDemandZ);
			m_ppModel[CntModel]->SetPosMotioned(posMotioned);
			m_ppModel[CntModel]->SetRotMotioned(rotMotioned);
		}
	}
}

//========================
//���[�V�������ݒ菈��
//========================
void CMotion::SetInfo(INFO info)
{
	//������ăJ�E���g���₷
	m_aInfo[m_nNumAll] = info;
	m_nNumAll++;
}

//========================
//���f���ݒ菈��
//========================
void CMotion::SetModel(CCharaParts** ppModel, int nNumModel)
{
	//���f������đ����������
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;
}
