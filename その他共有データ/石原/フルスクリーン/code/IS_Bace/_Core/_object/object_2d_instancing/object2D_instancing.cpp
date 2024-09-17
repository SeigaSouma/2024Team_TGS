//======================================================
//
//ポリゴン（2D,インスタンシングあり）処理[object2D_instancing.cpp]
//Author:石原颯馬
//
//======================================================
#include "object2D_instancing.h"
#include "..\..\..\..\main.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\utility\Utility.h"
#include <string>

//頂点情報設定
D3DVERTEXELEMENT9 CObject2D_Instancing::m_aVtxElem[] =
{
	{0,0,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_POSITION,0},
	{0,8,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,0},
	{1,0,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,1},
	{2,0,D3DDECLTYPE_FLOAT2,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_TEXCOORD,2},
	{3,0,D3DDECLTYPE_FLOAT4,D3DDECLMETHOD_DEFAULT,D3DDECLUSAGE_COLOR,0},
	D3DDECL_END()
};

//=================================
//コンストラクタ（デフォルト）
//=================================
CObject2D_Instancing::CObject2D_Instancing(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pDecl = nullptr;
	m_pSinglePolyVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_pInstancePosVtxBuff = nullptr;
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
CObject2D_Instancing::~CObject2D_Instancing()
{
}

//========================
//初期化処理
//========================
HRESULT CObject2D_Instancing::Init(void)
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
		"code\\IS_Bace\\_Core\\_object\\object_2d_instancing\\instancing.fx",
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
void CObject2D_Instancing::Uninit(void)
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
void CObject2D_Instancing::Update(void)
{

}

//========================
//描画処理
//========================
void CObject2D_Instancing::Draw(void)
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
	pDevice->SetStreamSource(1, m_pInstancePosVtxBuff, 0, sizeof(InstancePosVtx));
	pDevice->SetStreamSource(2, m_pInstanceUVVtxBuff, 0, sizeof(InstanceUVVtx));
	pDevice->SetStreamSource(3, m_pInstanceColVtxBuff, 0, sizeof(D3DXCOLOR));
	pDevice->SetIndices(m_pIdxBuff);
	
	m_pEffect->SetTechnique("InstancingTech");
	unsigned int passNum = 0;
	m_pEffect->Begin(&passNum, 0);
	m_pEffect->BeginPass(0);

	m_pEffect->SetTexture("tex", pTexture->GetAddress(m_nIdxTexture));
	m_pEffect->SetFloat("screenWidth", SCREEN_WIDTH * 0.5f);
	m_pEffect->SetFloat("screenHeight", SCREEN_HEIGHT * 0.5f);

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
CObject2D_Instancing* CObject2D_Instancing::Create(const float fWidth, const float fHeight, const int nInstanceNum, const int nPriority)
{
	CObject2D_Instancing* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクトの生成
		pObj = new CObject2D_Instancing(nPriority);

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
HRESULT CObject2D_Instancing::SetSize(const float fWidth, const float fHeight)
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
void CObject2D_Instancing::BindTexture(int nIdx)
{
	m_nIdxTexture = nIdx;

	SetVtx();
}

//========================
//インスタンス数設定
//========================
void CObject2D_Instancing::SetInstanceNum(const int num)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得

	m_nInstanceNum = num;

	//バッファがあるなら破棄
	ReleaseInstanceBuff();

	//バッファ生成
	//位置バッファの生成
	pDevice->CreateVertexBuffer(sizeof(InstancePosVtx) * m_nInstanceNum,
		0,
		0,
		D3DPOOL_MANAGED,
		&m_pInstancePosVtxBuff,
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
//インスタンス追加（直打ち）
//========================
void CObject2D_Instancing::AddInstance(float x, float y, float u, float v, D3DXCOLOR col)
{
	Instance ins;
	ins.pos.x = x;
	ins.pos.y = y;
	ins.uv.u = u;
	ins.uv.v = v;
	ins.col = col;

	AddList(ins);
}

//========================
//インスタンス追加（簡略化）
//========================
void CObject2D_Instancing::AddInstance(D3DXVECTOR3 pos, const int nPatternU, const int nPatternV, D3DXCOLOR col)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	int patWidth = IS_Utility::Clamp(pTexture->GetPatWidth(m_nIdxTexture), INT_MAX, 1);
	int patHeight = IS_Utility::Clamp(pTexture->GetPatHeight(m_nIdxTexture), INT_MAX, 1);

	Instance ins;
	ins.pos.x = pos.x;
	ins.pos.y = pos.y;
	ins.uv.u = (float)nPatternU / patWidth;
	ins.uv.v = (float)nPatternV / patHeight;

	ins.col.r = col.r;
	ins.col.g = col.g;
	ins.col.b = col.b;
	ins.col.a = col.a;

	AddList(ins);
}

//========================
//リスト追加
//========================
void CObject2D_Instancing::AddList(Instance ins)
{
	if (m_InstanceList.size() + 1 <= m_nInstanceNum)
	{
		m_InstanceList.emplace_back(ins);
	}
}

//========================
//インスタンス用バッファに入れる
//========================
void CObject2D_Instancing::SetInstanceBuff()
{
	//リストのイテレータもらう
	auto itr = m_InstanceList.begin();

	//バッファのロックと頂点情報へのポインタを取得
	InstancePosVtx* pPos;	//設定用ポインタ
	InstanceUVVtx* pTex;
	D3DXCOLOR* pCol;

	m_pInstancePosVtxBuff->Lock(0, 0, (void**)&pPos, 0);
	m_pInstanceUVVtxBuff->Lock(0, 0, (void**)&pTex, 0);
	m_pInstanceColVtxBuff->Lock(0, 0, (void**)&pCol, 0);

	for (int cnt = 0; cnt < m_nInstanceNum; cnt++, itr++)
	{
		pPos[cnt] = (*itr).pos;
		pTex[cnt] = (*itr).uv;
		pCol[cnt] = (*itr).col;
	}

	//頂点バッファをアンロック
	m_pInstanceColVtxBuff->Unlock();
	m_pInstanceUVVtxBuff->Unlock();
	m_pInstancePosVtxBuff->Unlock();
}

//========================
//インスタンスバッファ破棄
//========================
void CObject2D_Instancing::ReleaseInstanceBuff()
{
	if (m_pInstancePosVtxBuff != nullptr)
	{
		m_pInstancePosVtxBuff->Release();
		m_pInstancePosVtxBuff = nullptr;
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
HRESULT CObject2D_Instancing::SetVtx(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	int patWidth = IS_Utility::Clamp(pTexture->GetPatWidth(m_nIdxTexture), INT_MAX, 1);
	int patHeight = IS_Utility::Clamp(pTexture->GetPatHeight(m_nIdxTexture), INT_MAX, 1);

	SinglePolyVtx singleVtx[4];
	singleVtx[0] = { 0.0f,0.0f,0.0f,0.0f };
	singleVtx[1] = { m_fWidth,0.0f,1.0f / patWidth,0.0f };
	singleVtx[2] = { 0.0f,m_fHeight,0.0f,1.0f / patHeight };
	singleVtx[3] = { m_fWidth,m_fHeight,1.0f / patWidth, 1.0f / patHeight };

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
