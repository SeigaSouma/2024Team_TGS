//======================================================
//
//キャラのパーツ処理[charaparts.cpp]
//Author:石原颯馬
//
//======================================================
#include "charaparts.h"
#include "..\..\..\_Core\manager\manager.h"
#include "..\..\..\_Core\renderer\renderer.h"
#include "..\..\..\_Core\texture\texture.h"
#include "..\..\utility\Utility.h"
#include "..\..\xmodel\xmodel.h"
#include <assert.h>

//=================================
//コンストラクタ
//=================================
CCharaParts::CCharaParts()
{
	m_pModel = nullptr;
	m_pos = IS_Utility::VEC3_ZERO;
	m_rot = IS_Utility::VEC3_ZERO;
	m_posMotioned = IS_Utility::VEC3_ZERO;
	m_rotMotioned = IS_Utility::VEC3_ZERO;
	m_pParent = nullptr;
}

//=================================
//デストラクタ
//=================================
CCharaParts::~CCharaParts()
{
}

//========================
//初期化処理
//========================
HRESULT CCharaParts::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CCharaParts::Uninit(void)
{
	
}

//========================
//更新処理
//========================
void CCharaParts::Update(void)
{
	//いったん仮の値を入れる
	m_posMotioned = m_pos;
	m_rotMotioned = m_rot;
}

//========================
//描画処理
//========================
void CCharaParts::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DXMATRIX mtxParent;			//親のマトリ
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//モデル取得
	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotMotioned.y, m_rotMotioned.x, m_rotMotioned.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_posMotioned.x, m_posMotioned.y, m_posMotioned.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//パーツの親マトリ設定
	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMtx();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//パーツのマトリと親マトリをかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pModel->GetBufMat()->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModel->GetNumMat(); nCntMat++)
	{
		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャ設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_pModel->GetIdxTexture()[nCntMat]));

		//モデル描画
		m_pModel->GetMesh()->DrawSubset(nCntMat);
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//生成処理
//========================
CCharaParts* CCharaParts::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CCharaParts* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクト2Dの生成
		pObj = new CCharaParts;

		//初期化
		pObj->Init();
		pObj->SetPos(pos);
		pObj->SetRot(rot);
		pObj->m_pModel = pModel;

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

