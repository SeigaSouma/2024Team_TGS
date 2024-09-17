//======================================================
//
//モーション処理[motion.cpp]
//Author:石原颯馬
//
//======================================================
#include "motion.h"
#include "..\charaparts\charaparts.h"
#include "..\..\utility\Utility.h"

//========================
//コンストラクタ
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
//デストラクタ
//========================
CMotion::~CMotion()
{
}

//========================
//初期化処理
//========================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//========================
//終了処理
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
//モーション設定処理
//========================
void CMotion::Set(int nType)
{
	//モーション設定
	m_nType = nType;

	//初期設定
	m_bLoop = m_aInfo[m_nType].m_bLoop;
	m_nNumKey = m_aInfo[m_nType].m_nNumKey;
	m_nKey = 0;
	m_nCounter = 0;
	m_bFinish = false;
}

//========================
//更新処理
//========================
void CMotion::Update(void)
{
	//今のキーと次のキーを入れておく
	int nNowKey = m_nKey;
	int nNextKey = (m_nKey + 1) % m_nNumKey;

	if (m_bFinish == false)
	{
		for (int CntModel = 0; CntModel < m_nNumModel; CntModel++)
		{
			//オフセットを入れておく
			D3DXVECTOR3 posMotioned = m_ppModel[CntModel]->GetPos();
			D3DXVECTOR3 rotMotioned = m_ppModel[CntModel]->GetRot();

			//差分算出
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

			//位置向き算出
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

			//パーツの位置向き設定
			posMotioned += D3DXVECTOR3(posDemandX, posDemandY, posDemandZ);
			rotMotioned += D3DXVECTOR3(rotDemandX, rotDemandY, rotDemandZ);
			m_ppModel[CntModel]->SetPosMotioned(posMotioned);
			m_ppModel[CntModel]->SetRotMotioned(rotMotioned);
		}
		m_nCounter++;

		//再生フレーム後の挙動
		if (m_nCounter == m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_nFrame)
		{//再生フレーム数に達したら
		 //カウンターをリセットしてキーを一つ増やす
			m_nCounter = 0;
			m_nKey++;

			//キーの最大数に達したらループするか否かに応じて再設定する
			if (m_bLoop == true)
			{//ループあり
				if (m_nKey == m_nNumKey)
				{
					m_nKey = 0;
				}
			}
			else
			{//ループなし
				if (m_nKey == m_nNumKey - 1)
				{
					m_bFinish = true;
				}
			}
		}
	}
	else
	{//最後のモーションを使用
		for (int CntModel = 0; CntModel < m_nNumModel; CntModel++)
		{
			nNextKey = m_nNumKey - 1;
			nNowKey = nNextKey - 1;

			//オフセットを入れておく
			D3DXVECTOR3 posMotioned = m_ppModel[CntModel]->GetPos();
			D3DXVECTOR3 rotMotioned = m_ppModel[CntModel]->GetRot();

			//差分算出
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

			//位置向き算出
			float posDemandX = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.x + posDiffX;
			float posDemandY = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.y + posDiffY;
			float posDemandZ = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].pos.z + posDiffZ;
			float rotDemandX = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.x + rotDiffX;
			float rotDemandY = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.y + rotDiffY;
			float rotDemandZ = m_aInfo[m_nType].m_pKeyInfo[nNowKey].m_pKey[CntModel].rot.z + rotDiffZ;

			//パーツの位置向き設定
			posMotioned += D3DXVECTOR3(posDemandX, posDemandY, posDemandZ);
			rotMotioned += D3DXVECTOR3(rotDemandX, rotDemandY, rotDemandZ);
			m_ppModel[CntModel]->SetPosMotioned(posMotioned);
			m_ppModel[CntModel]->SetRotMotioned(rotMotioned);
		}
	}
}

//========================
//モーション情報設定処理
//========================
void CMotion::SetInfo(INFO info)
{
	//情報入れてカウント増やす
	m_aInfo[m_nNumAll] = info;
	m_nNumAll++;
}

//========================
//モデル設定処理
//========================
void CMotion::SetModel(CCharaParts** ppModel, int nNumModel)
{
	//モデル入れて総数も入れる
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;
}
