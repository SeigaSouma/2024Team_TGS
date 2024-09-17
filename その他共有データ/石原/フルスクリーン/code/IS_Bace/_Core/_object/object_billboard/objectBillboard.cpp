//======================================================
//
//オブジェクト（ビルボード）処理[objectBillboard.cpp]
//Author:石原颯馬
//
//======================================================
#include "objectBillboard.h"
#include "..\..\manager\manager.h"
#include "..\..\renderer\renderer.h"
#include "..\..\texture\texture.h"
#include "..\..\input\input.h"
#include "..\..\utility\Utility.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject3D(nPriority)
{

}

//=================================
//デストラクタ
//=================================
CObjectBillboard::~CObjectBillboard()
{
}

//========================
//初期化処理
//========================
HRESULT CObjectBillboard::Init(void)
{
	return CObject3D::Init();
}

//========================
//終了処理
//========================
void CObjectBillboard::Uninit(void)
{
	CObject3D::Uninit();
}

//========================
//更新処理
//========================
void CObjectBillboard::Update(void)
{
	CObject3D::Update();
}

//========================
//描画処理
//========================
void CObjectBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetInstance()->GetInstance()->GetTexture();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DXMATRIX mtxView;				//ビューマトリ取得用
	D3DXVECTOR3 pos = CObject3D::GetPos();
	D3DXMATRIX* mtxWorld = CObject3D::GetMtxWorld();
	LPDIRECT3DVERTEXBUFFER9 pVtxbuff = CObject3D::GetVtxBuff();

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(mtxWorld);

	//ビューマトリ取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(mtxWorld, nullptr, &mtxView);
	//修正
	mtxWorld->_41 = 0.0f;
	mtxWorld->_42 = 0.0f;
	mtxWorld->_43 = 0.0f;

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, pVtxbuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	pDevice->SetTexture(0, pTexture->GetAddress(CObject3D::GetIdxTexture()));

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//生成処理
//========================
CObjectBillboard* CObjectBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, const int nIdxTexture)
{
	CObjectBillboard* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクトの生成
		pObj = new CObjectBillboard;

		//初期化
		pObj->Init();

		//設定
		pObj->SetPos(pos);
		pObj->SetRot(rot);
		pObj->SetSize(fWidth, fDepth);
		pObj->BindTexture(nIdxTexture);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}
