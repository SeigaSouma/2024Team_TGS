//=============================================================================
// 
//  石垣処理 [stonewall.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "stonewall.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"
#include "course.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE = "data\\TEXTURE\\map_object\\ishigaki000.png";
	const int WIDTH_BLOCK = 2;
	const float WIDTH = 200.0f;		// 斜めの幅
	const float HEIGHT = 500.0f;	// 高さ
	const float INTERVAL_TEXU = 500.0f;	// U座標の間隔
}

//==========================================================================
// コンストラクタ
//==========================================================================
CStoneWall::CStoneWall(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_vecSegmentPosition.clear();	// 基点の位置
	m_vecVtxPosition.clear();		// 各頂点の位置
	m_courceLength = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
CStoneWall::~CStoneWall()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CStoneWall* CStoneWall::Create()
{
	// メモリの確保
	CStoneWall* pObjMeshField = DEBUG_NEW CStoneWall;

	if (pObjMeshField != nullptr)
	{
		// 初期化処理
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CStoneWall::Init()
{
	HRESULT hr;

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist(TEXTURE);
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 頂点座標割り当て
	BindVtxPosition();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CStoneWall::Uninit()
{
	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 各頂点計算
//==========================================================================
void CStoneWall::CalVtxPosition()
{

	// セグメントの長さを計算
	int segmentSize = m_vecSegmentPosition.size();
	std::vector<float> vecLength(segmentSize);

	for (int i = 0; i < segmentSize; ++i)
	{
		// 次回のインデックス(ループ)
		int next = (i + 1) % segmentSize;

		if (next == 0)
		{
			vecLength[i] = 10.0f;
			break;
		}

		// 点同士の距離
		vecLength[i] = m_vecSegmentPosition[i].Distance(m_vecSegmentPosition[next]);
	}


	// 頂点情報クリア
	m_vecVtxPosition.clear();

	// 各頂点格納
	m_courceLength = 0.0f;
	for (int i = 0; i < segmentSize; i++)
	{
		float distance = 0.0f;

		while (1)
		{
			distance += CCourse::GetCreateDistance();

			if (distance >= vecLength[i])
			{
				m_courceLength += CCourse::GetCreateDistance() - (distance - vecLength[i]);

				distance = vecLength[i];

				m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, m_courceLength, 20.0f));
				break;
			}

			m_courceLength += CCourse::GetCreateDistance();
			m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, m_courceLength));
		}
	}
}

//==========================================================================
// リセット
//==========================================================================
void CStoneWall::Reset()
{
	// 終了処理
	CObject3DMesh::Uninit();

	// 死亡フラグ強制リセット
	SetEnableDeath(false);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// 各頂点計算
	CalVtxPosition();

	// 各種変数初期化
	SetPosition(MyLib::Vector3(0.0f, 10.0f, 0.0f));				// 位置
	SetWidthBlock(1);		// 幅分割
	SetHeightBlock(static_cast<int>(m_vecVtxPosition.size()) - 1);	// 縦分割
	SetWidthLen(0.0f);		// 縦長さ
	SetHeightLen(0.0f);		// 横長さ

	// オブジェクト3Dメッシュの初期化処理
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// 頂点情報設定
	SetVtx();

	// 頂点座標割り当て
	BindVtxPosition();
}

//==========================================================================
// 更新処理
//==========================================================================
void CStoneWall::Update()
{

}

//==========================================================================
// 頂点座標
//==========================================================================
void CStoneWall::BindVtxPosition()
{
	// 計算用変数
	MyLib::Vector3 offset;
	MyLib::Matrix mtxParent, mtxTrans, mtxRotate;
	MyLib::Matrix mtxLeft, mtxRight;

	MyLib::Vector3* pVtxPos = GetVtxPos();
	MyLib::Vector3 rot;

	for (int y = 0; y < static_cast<int>(m_vecVtxPosition.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// 向き反映
		int next = (y + 1) % static_cast<int>(m_vecVtxPosition.size());

		bool bEnd = false;
		if (next == 0)
		{
			next = y - 1;
			bEnd = true;
		}

		rot.y = m_vecVtxPosition[next].AngleXZ(m_vecVtxPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		if (bEnd)
		{
			rot.y *= -1;
		}

		// 回転反映
		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// 位置反映
		mtxTrans.Translation(m_vecVtxPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);


		/*CEffect3D::Create(
			m_vecVtxPosition[y] + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);*/

		// オフセット反映
		offset = MyLib::Vector3(0.0f, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, HEIGHT, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);


		// 頂点座標代入
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

	}

	// 頂点情報設定
	SetVtx();
}

//==========================================================================
// 描画処理
//==========================================================================
void CStoneWall::Draw()
{
	//  デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (CManager::GetInstance()->IsWireframe())
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	// ワイヤーフレームモード
	}

	// 描画処理
	CObject3DMesh::Draw();

	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	// 埋めるモード
}

//==========================================================================
// 頂点情報設定処理
//==========================================================================
void CStoneWall::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3* pVtxNor = GetVtxNor();
	D3DXVECTOR2* pTex = GetVtxTex();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	int heightBlock = static_cast<int>(m_vecVtxPosition.size());

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// 縦の分割分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH_BLOCK; nCntWidth++)
		{// 横の分割分繰り返す

			// 今回の頂点
			int nNowPoint = (nCntWidth + 1) + (nCntHeight * (WIDTH_BLOCK));
			int nVerTexW = (WIDTH_BLOCK) + 1;

			int nLeft = nCntWidth + (nCntHeight * (WIDTH_BLOCK));
			int nRight = nCntWidth + (nCntHeight * (WIDTH_BLOCK)) + nVerTexW;

			if (nNowPoint >= vtxNum)
			{
				continue;
			}

			if (nRight >= vtxNum)
			{// 頂点数超えたら

				// 頂点座標の設定
				VtxRight = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
			}
			else
			{
				VtxRight = pVtxPos[nRight];
			}

			if (nLeft >= vtxNum)
			{// 頂点数超えたら

				// 頂点座標の設定
				VtxLeft = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
			}
			else
			{
				VtxLeft = pVtxPos[nLeft];
			}

			if (nNowPoint >= vtxNum)
			{// 頂点数超えたら

				// 頂点座標の設定
				VtxNow = MyLib::Vector3(
					(fWidthLen * nCntWidth) - ((fWidthLen * WIDTH_BLOCK) * 0.5f),
					0.0f,
					-((fHeightLen * nCntHeight) - ((fHeightLen * heightBlock) * 0.5f)));
			}
			else
			{
				VtxNow = pVtxPos[nNowPoint];
			}

			// ベクトルを計算
			vec1 = VtxRight - VtxNow;
			vec2 = VtxLeft - VtxNow;

			// 外積を求める
			D3DXVec3Cross(&nor, &vec1, &vec2);

			// 外積の正規化をして法線にする
			D3DXVec3Normalize(&nor, &nor);

			// 法線
			pVtxNor[nNowPoint] = nor;
		}
	}

	if (pTex != nullptr)
	{
		float posU = 0.0f, posV = 0.0f;
		int idx = 0;

		int texID = CTexture::GetInstance()->Regist(TEXTURE);
		float intervalV = UtilFunc::Transformation::AdjustSizeByWidth(CTexture::GetInstance()->GetImageSize(texID), INTERVAL_TEXU).y;

		for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
		{// 縦の分割分繰り返す

			// リセット
			posV = 0.0f;

			for (int nCntWidth = 0; nCntWidth < WIDTH_BLOCK; nCntWidth++)
			{// 横の分割分繰り返す

				idx = nCntWidth + (nCntHeight * WIDTH_BLOCK);
				pTex[idx] = D3DXVECTOR2(posU, posV);

				// 縦の割合分進める
				posV += sqrtf((pVtxPos[idx].y - pVtxPos[idx + 1].y) * (pVtxPos[idx].y - pVtxPos[idx + 1].y)) / intervalV;
				
			}

			// 横の割合分進める
			posU += pVtxPos[idx + 1].DistanceXZ(pVtxPos[idx - 1]) / INTERVAL_TEXU;
			
		}
	}

	// 頂点情報更新
	CObject3DMesh::SetVtx();
}

//==========================================================================
// 基点の位置取得
//==========================================================================
MyLib::Vector3 CStoneWall::GetVecPosition(int idx)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecSegmentPosition[idx];
}

//==========================================================================
// 基点の位置設定
//==========================================================================
void CStoneWall::SetVecPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return;

	m_vecSegmentPosition[idx] = pos;
}

//==========================================================================
// 各頂点の位置取得
//==========================================================================
MyLib::Vector3 CStoneWall::GetVecVtxPosition(int idx)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecVtxPosition[idx];
}

//==========================================================================
// 各頂点の位置設定
//==========================================================================
void CStoneWall::SetVecVtxPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return;

	m_vecVtxPosition[idx] = pos;
}