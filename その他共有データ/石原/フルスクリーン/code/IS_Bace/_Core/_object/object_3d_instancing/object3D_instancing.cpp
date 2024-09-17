//======================================================
//
//ポリゴン（3D,インスタンシングあり）処理[object3D_instancing.cpp]
//Author:石原颯馬
//
//======================================================
#include "object3D_instancing.h"
#include "..\..\..\..\main.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\utility\Utility.h"
#include <string>
#include "..\..\debugproc\debugproc.h"

//頂点情報設定
D3DVERTEXELEMENT9 CObject3D_Instancing::m_aVtxElem[] =
{
	//大元
	{0,0,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
	{0,12,D3DDECLTYPE_FLOAT3,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_NORMAL,0},
	{0,24,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
	
	//インスタンシング_マトリ
	{1,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},
	{1,16,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2},
	{1,32,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,3},
	{1,48,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,4},
	//インスタンシング_UV
	{2,0,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,5},
	//インスタンシング_カラー
	{3,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR,0},
	D3DDECL_END()
};

//=================================
//コンストラクタ（デフォルト）
//=================================
CObject3D_Instancing::CObject3D_Instancing(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pDecl = nullptr;
	m_pSinglePolyVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_pInstanceMtxVtxBuff = nullptr;
	m_pInstanceUVVtxBuff = nullptr;
	m_pInstanceColVtxBuff = nullptr;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_nIdxTexture = -1;
	m_InstanceList.clear();
	m_nInstanceNum = 0;
	m_pEffect = nullptr;
}

//=================================
//デストラクタ
//=================================
CObject3D_Instancing::~CObject3D_Instancing()
{
}

//========================
//初期化処理
//========================
HRESULT CObject3D_Instancing::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(SinglePolyVtx) * 4,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pSinglePolyVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	//頂点宣言生成
	if (FAILED(pDevice->CreateVertexDeclaration(m_aVtxElem, &m_pDecl)))
	{
		return E_FAIL;
	}

	//効果
	D3DXCreateEffectFromFile(
		pDevice,
		"code\\IS_Bace\\_Core\\_object\\object_3d_instancing\\instancing.fx",
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	//インデックスバッファ設定
	WORD* pIdx;	//インデックス情報へのポインタ
	int nCntWidth;	//インデックスカウンタ

	//バッファロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 2;
	pIdx[4] = 1;
	pIdx[5] = 3;

	//バッファアンロック
	m_pIdxBuff->Unlock();

	//単ポリ設定
	SetVtx();

	return S_OK;
}

//========================
//終了処理
//========================
void CObject3D_Instancing::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pSinglePolyVtxBuff != nullptr)
	{
		m_pSinglePolyVtxBuff->Release();
		m_pSinglePolyVtxBuff = nullptr;
	}
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
	if (m_pEffect != nullptr)
	{
		m_pEffect->Release();
		m_pEffect = nullptr;
	}
	ReleaseInstanceBuff();

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CObject3D_Instancing::Update(void)
{

}

//========================
//描画処理
//========================
void CObject3D_Instancing::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();			//テクスチャオブジェクト取得

	//インスタンス用バッファ類設定
	SetInstanceBuff();

	//描画開始
	//周波数設定
	pDevice->SetStreamSourceFreq(0, D3DSTREAMSOURCE_INDEXEDDATA | m_nInstanceNum);
	pDevice->SetStreamSourceFreq(1, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(2, D3DSTREAMSOURCE_INSTANCEDATA | 1);
	pDevice->SetStreamSourceFreq(3, D3DSTREAMSOURCE_INSTANCEDATA | 1);

	//頂点バッファをデータストリームに設定
	pDevice->SetVertexDeclaration(m_pDecl);
	pDevice->SetStreamSource(0, m_pSinglePolyVtxBuff, 0, sizeof(SinglePolyVtx));
	pDevice->SetStreamSource(1, m_pInstanceMtxVtxBuff, 0, sizeof(D3DXMATRIX));
	pDevice->SetStreamSource(2, m_pInstanceUVVtxBuff, 0, sizeof(InstanceUVVtx));
	pDevice->SetStreamSource(3, m_pInstanceColVtxBuff, 0, sizeof(D3DXCOLOR));
	pDevice->SetIndices(m_pIdxBuff);

	//ビュー・プロジェクションマトリ取得
	D3DXMATRIX mtxView, mtxProj;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	
	//テクニック設定してパスの開始
	m_pEffect->SetTechnique("InstancingTech");
	unsigned int passNum = 0;
	m_pEffect->Begin(&passNum, 0);
	m_pEffect->BeginPass(0);

	//シェーダ内変数に入れる
	m_pEffect->SetTexture("tex", pTexture->GetAddress(m_nIdxTexture));
	m_pEffect->SetMatrix("mtxView", &mtxView);
	m_pEffect->SetMatrix("mtxProj", &mtxProj);

	//描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	m_pEffect->EndPass();
	m_pEffect->End();

	//周波数戻す
	pDevice->SetStreamSourceFreq(0, 1);
	pDevice->SetStreamSourceFreq(1, 1);
	pDevice->SetStreamSourceFreq(2, 1);
	pDevice->SetStreamSourceFreq(3, 1);
}

//========================
//生成処理（テクスチャなし）
//========================
CObject3D_Instancing* CObject3D_Instancing::Create(const float fWidth, const float fHeight, const int nInstanceNum, const int nPriority)
{
	CObject3D_Instancing* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクトの生成
		pObj = new CObject3D_Instancing(nPriority);

		//初期化
		pObj->Init();

		//値設定
		pObj->SetSize(fWidth, fHeight);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//========================
//サイズ変更
//========================
HRESULT CObject3D_Instancing::SetSize(const float fWidth, const float fHeight)
{
	//値変更
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	//頂点配置
	return SetVtx();
}

//========================
//テクスチャ設定
//========================
void CObject3D_Instancing::BindTexture(int nIdx)
{
	m_nIdxTexture = nIdx;

	SetVtx();
}

//========================
//インスタンス数設定
//========================
void CObject3D_Instancing::SetInstanceNum(const int num)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得

	m_nInstanceNum = num;

	//バッファがあるなら破棄
	ReleaseInstanceBuff();

	//バッファ生成
	//位置バッファの生成
	pDevice->CreateVertexBuffer(sizeof(D3DXMATRIX) * m_nInstanceNum,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pInstanceMtxVtxBuff,
		nullptr);
	//UVバッファの生成
	pDevice->CreateVertexBuffer(sizeof(InstanceUVVtx) * m_nInstanceNum,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pInstanceUVVtxBuff,
		nullptr);
	//カラーバッファの生成
	pDevice->CreateVertexBuffer(sizeof(D3DXCOLOR) * m_nInstanceNum,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pInstanceColVtxBuff,
		nullptr);
}

//========================
//インスタンス追加
//========================
void CObject3D_Instancing::AddInstance(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nPatternU, const int nPatternV, D3DXCOLOR col)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	int patWidth = IS_Utility::Clamp(pTexture->GetPatWidth(m_nIdxTexture), INT_MAX, 1);
	int patHeight = IS_Utility::Clamp(pTexture->GetPatHeight(m_nIdxTexture), INT_MAX, 1);

	Instance ins;

	//仮マトリ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&ins.mtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&ins.mtx, &ins.mtx, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&ins.mtx, &ins.mtx, &mtxTrans);

	//UV
	ins.uv.u = (float)nPatternU / patWidth;
	ins.uv.v = (float)nPatternV / patHeight;

	//カラー
	ins.col = col;

	AddList(ins);
}

//========================
//リスト追加
//========================
void CObject3D_Instancing::AddList(Instance ins)
{
	if (m_InstanceList.size() + 1 <= m_nInstanceNum)
	{
		m_InstanceList.emplace_back(ins);
	}
}

//========================
//インスタンス用バッファに入れる
//========================
void CObject3D_Instancing::SetInstanceBuff()
{
	//リストのイテレータもらう
	auto itr = m_InstanceList.begin();

	//バッファのロックと頂点情報へのポインタを取得
	D3DXMATRIX* pMtx;	//設定用ポインタ
	InstanceUVVtx* pTex;
	D3DXCOLOR* pCol;

	m_pInstanceMtxVtxBuff->Lock(0, 0, (void**)&pMtx, 0);
	m_pInstanceUVVtxBuff->Lock(0, 0, (void**)&pTex, 0);
	m_pInstanceColVtxBuff->Lock(0, 0, (void**)&pCol, 0);

	for (int cnt = 0; cnt < m_nInstanceNum; cnt++, itr++)
	{
		pMtx[cnt] = (*itr).mtx;
		pTex[cnt] = (*itr).uv;
		pCol[cnt] = (*itr).col;
	}

	//頂点バッファをアンロック
	m_pInstanceColVtxBuff->Unlock();
	m_pInstanceUVVtxBuff->Unlock();
	m_pInstanceMtxVtxBuff->Unlock();
}

//========================
//インスタンスバッファ破棄
//========================
void CObject3D_Instancing::ReleaseInstanceBuff()
{
	if (m_pInstanceMtxVtxBuff != nullptr)
	{
		m_pInstanceMtxVtxBuff->Release();
		m_pInstanceMtxVtxBuff = nullptr;
	}
	if (m_pInstanceUVVtxBuff != nullptr)
	{
		m_pInstanceUVVtxBuff->Release();
		m_pInstanceUVVtxBuff = nullptr;
	}
	if (m_pInstanceColVtxBuff != nullptr)
	{
		m_pInstanceColVtxBuff->Release();
		m_pInstanceColVtxBuff = nullptr;
	}
}

//========================
//頂点座標設定
//========================
HRESULT CObject3D_Instancing::SetVtx(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	int patWidth = IS_Utility::Clamp(pTexture->GetPatWidth(m_nIdxTexture),INT_MAX,1);
	int patHeight = IS_Utility::Clamp(pTexture->GetPatHeight(m_nIdxTexture), INT_MAX, 1);

	SinglePolyVtx singleVtx[4];
	singleVtx[0] = { -m_fWidth * 0.5f,0.0f,m_fHeight * 0.5f,0.0f,0.0f,-1.0f,0.0f,0.0f };
	singleVtx[1] = { m_fWidth * 0.5f,0.0f,m_fHeight * 0.5f,0.0f,0.0f,-1.0f,1.0f / patWidth,0.0f };
	singleVtx[2] = { -m_fWidth * 0.5f,0.0f,-m_fHeight * 0.5f,0.0f,0.0f,-1.0f,0.0f,1.0f / patHeight };
	singleVtx[3] = { m_fWidth * 0.5f,0.0f,-m_fHeight * 0.5f,0.0f,0.0f,-1.0f,1.0f / patWidth, 1.0f / patHeight };

	//頂点バッファのロックと頂点情報へのポインタを取得
	SinglePolyVtx* pVtx;	//設定用ポインタ
	if (FAILED(m_pSinglePolyVtxBuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	for (int cnt = 0; cnt < 4; cnt++)
	{
		pVtx[cnt] = singleVtx[cnt];
	}

	//頂点バッファをアンロック
	if (FAILED(m_pSinglePolyVtxBuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}
