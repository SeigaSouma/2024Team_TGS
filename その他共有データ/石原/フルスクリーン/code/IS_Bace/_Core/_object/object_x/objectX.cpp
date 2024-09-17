//======================================================
//
//オブジェクト（X）処理[object3D.cpp]
//Author:石原颯馬
//
//======================================================
#include "objectX.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\input\input.h"
#include "..\..\xmodel\xmodel.h"
#include "..\object_empty\object.h"
#include "..\..\utility\Utility.h"
#include <assert.h>

//静的メンバ変数
CObjectX* CObjectX::m_pTop = nullptr;
CObjectX* CObjectX::m_pCur = nullptr;
int CObjectX::m_nNumAll = 0;

//=================================
//コンストラクタ（デフォルト）
//=================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pos = IS_Utility::VEC3_ZERO;
	m_rot = IS_Utility::VEC3_ZERO;

	if (m_pCur == nullptr)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;		//俺が先頭
		m_pPrev = nullptr;		//前後誰もいない
		m_pNext = nullptr;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = nullptr;			//自分の次のオブジェはいない
	}
	m_pCur = this;				//俺が最後尾
	m_bExclusion = false;		//生きてる
	m_pModel = nullptr;
	m_bEnable = false;
	m_nNumAll++;
}

//=================================
//デストラクタ
//=================================
CObjectX::~CObjectX()
{
}

//========================
//初期化処理
//========================
HRESULT CObjectX::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CObjectX::Uninit(void)
{
	m_bExclusion = true;		//除外予定

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CObjectX::Update(void)
{
	
}

//========================
//描画処理
//========================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//計算用
	D3DMATERIAL9 matDef;												//現在のマテリアル保存用
	D3DXMATERIAL *pMat;													//マテリアルデータへのポインタ

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//モデル取得
	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxWorld);

	//拡縮を反映
	//D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
	//D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pModel->GetBufMat()->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModel->GetNumMat(); nCntMat++)
	{
		//マテリアル変更
		D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;

		if (m_bEnable == true)
		{//色を変更する場合
		 //メイン色変更
			changeMat.Diffuse = m_changeColor;
		}

		//マテリアル設定
		pDevice->SetMaterial(&changeMat);

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
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CObjectX* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクトの生成
		pObj = new CObjectX;

		//初期化
		pObj->Init();

		//設定
		pObj->SetPos(pos);
		pObj->SetRot(rot);
		pObj->SetModel(pModel);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//========================
//モデル設定とサイズ計測
//========================
void CObjectX::SetModel(CXModel * pModel)
{
	m_pModel = pModel;

	D3DXVECTOR3 vtxMin, vtxMax;
	m_fWidth = vtxMax.x - vtxMin.x;
	m_fHeight = vtxMax.y - vtxMin.y;
	m_fDepth = vtxMax.z - vtxMin.z;
}

//========================
//色変更設定
//========================
void CObjectX::SetColor(const bool bEnable, const D3DXCOLOR col)
{
	m_bEnable = bEnable;

	if (bEnable == true)
	{
		m_changeColor = col;
	}
}

//========================
//オブジェクト単位除外処理
//========================
void CObjectX::Delete(CXModel* pTarget)
{
	CObjectX* pObject = m_pTop;	//先頭を入れる

	while (pObject != nullptr)
	{//最後尾まで回し続ける
		CObjectX* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

		if (pObject->GetModel() == pTarget)
		{
			pObject->Uninit();	//除外
		}

		pObject = pObjectNext;	//次を入れる
	}
}

//========================
//除外処理
//========================
void CObjectX::Exclusion(void)
{
	if (m_pPrev != nullptr)
	{//前にオブジェがいる
		m_pPrev->m_pNext = m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
	}
	if (m_pNext != nullptr)
	{
		m_pNext->m_pPrev = m_pPrev;	//次のオブジェの前のオブジェは自分の前のオブジェ
	}

	if (m_pCur == this)
	{//最後尾でした
		m_pCur = m_pPrev;	//最後尾を自分の前のオブジェにする
	}
	if (m_pTop == this)
	{
		m_pTop = m_pNext;	//先頭を自分の次のオブジェにする
	}

	//成仏
	m_nNumAll--;	//総数減らす
}