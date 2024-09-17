//======================================================
//
//オブジェクト（3D）処理[object3D.cpp]
//Author:石原颯馬
//
//======================================================
#include "object3D.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\utility\Utility.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pVtxbuff = nullptr;
	m_nIdxTexture = -1;
	m_pos = IS_Utility::VEC3_ZERO;
	m_rot = IS_Utility::VEC3_ZERO;
	m_fWidth = 0.0f;
	m_fDepth = 0.0f;
}

//=================================
//デストラクタ
//=================================
CObject3D::~CObject3D()
{
}

//========================
//初期化処理
//========================
HRESULT CObject3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		nullptr)))
	{
		return E_FAIL;
	}

	//頂点座標（サイズ）・法線設定
	SetSize(m_fWidth, m_fDepth);

	//色設定
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	//テクスチャ座標設定
	SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	return S_OK;
}

//========================
//終了処理
//========================
void CObject3D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxbuff != nullptr)
	{
		m_pVtxbuff->Release();
		m_pVtxbuff = nullptr;
	}

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CObject3D::Update(void)
{
	
}

//========================
//描画処理
//========================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用

	CManager::GetInstance()->GetRenderer()->SetEnableAlplaTest(true);
	CManager::GetInstance()->GetRenderer()->SetEnableZTest(false);

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxbuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	CManager::GetInstance()->GetRenderer()->SetEnableAlplaTest(false);
	CManager::GetInstance()->GetRenderer()->SetEnableZTest(true);
}

//========================
//生成処理
//========================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, int nPriority)
{
	CObject3D* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクトの生成
		pObj = new CObject3D(nPriority);

		//初期化
		pObj->Init();

		//データ設定
		pObj->m_pos = pos;
		pObj->m_rot = rot;
		pObj->SetSize(fWidth, fDepth);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//========================
//色設定
//========================
HRESULT CObject3D::SetCol(const D3DXCOLOR col)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_3D *pVtx;	//設定用ポインタ
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//頂点カラーの変換
	D3DCOLOR exchange = D3DCOLOR_COLORVALUE(col.r, col.g, col.b, col.a);

	//頂点カラーの設定
	pVtx[0].col = exchange;
	pVtx[1].col = exchange;
	pVtx[2].col = exchange;
	pVtx[3].col = exchange;

	//頂点バッファをアンロック
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================
//サイズ設定
//========================
void CObject3D::SetSize(const float fWidth, const float fDepth)
{
	VERTEX_3D *pVtx;	//頂点再設定に使用

	//サイズ値設定
	m_fWidth = fWidth;
	m_fDepth = fDepth;

	//頂点再設定
	//頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標（相対座標）
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth * 0.5f, 0.0f, m_fDepth * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth * 0.5f, 0.0f, m_fDepth * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth * 0.5f, 0.0f, -m_fDepth * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth * 0.5f, 0.0f, -m_fDepth * 0.5f);

	//法線計算
	CulcNor(pVtx);

	//頂点バッファをアンロック
	m_pVtxbuff->Unlock();
}

//========================
//テクスチャ座標設定（Vec2指定）
//========================
HRESULT CObject3D::SetTex(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex3)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_3D* pVtx;	//設定用ポインタ
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void**)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//テクスチャ座標
	pVtx[0].tex = tex0;
	pVtx[1].tex = D3DXVECTOR2(tex3.x, tex0.y);
	pVtx[2].tex = D3DXVECTOR2(tex0.x, tex3.y);
	pVtx[3].tex = tex3;

	//頂点バッファをアンロック
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================
//テクスチャ座標設定（番号指定）
//========================
HRESULT CObject3D::SetTex(const int nPatternU, const int nPatternV)
{
	//テクスチャオブジェクト
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	//テクスチャ座標
	D3DXVECTOR2 tex0, tex3;
	tex0 = tex3 = D3DXVECTOR2(0.0f, 0.0f);
	//パターン幅高さ
	int nTexPatU = pTexture->GetPatWidth(m_nIdxTexture);
	int nTexPatV = pTexture->GetPatHeight(m_nIdxTexture);

	//パターン指定が合っているか
	if (nPatternU >= nTexPatU || nPatternV >= nTexPatV)
	{
		return E_FAIL;
	}

	//座標計算
	tex0.x = (float)nPatternU / nTexPatU;
	tex0.y = (float)nPatternV / nTexPatV;
	tex3.x = (float)(nPatternU + 1) / nTexPatU;
	tex3.x = (float)(nPatternV + 1) / nTexPatV;

	return SetTex(tex0, tex3);
}

//========================
//法線計算
//========================
void CObject3D::CulcNor(VERTEX_3D* pVtx)
{
	//法線ベクトル
	D3DXVECTOR3 vec1 = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vec2 = pVtx[2].pos - pVtx[0].pos;
	D3DXVec3Cross(&pVtx[0].nor, &vec1, &vec2);
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	vec1 = pVtx[2].pos - pVtx[3].pos;
	vec2 = pVtx[1].pos - pVtx[3].pos;
	D3DXVec3Cross(&pVtx[3].nor, &vec1, &vec2);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);

	pVtx[1].nor = (pVtx[0].nor + pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);

	pVtx[2].nor = (pVtx[0].nor + pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);
}
