//=============================================================================
// 
//  起伏処理 [course.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "course.h"
#include "manager.h"
#include "calculation.h"
#include "debugproc.h"
#include "particle.h"
#include "3D_effect.h"
#include "camera.h"
#include "spline.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int WIDTH_BLOCK = 2;
	const float WIDTH = 200.0f;
	const float CREATEDISTANCE = 50.0f;	// 生成間隔
}

//==========================================================================
// コンストラクタ
//==========================================================================
CCourse::CCourse(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_pCollisionLineBox.clear();	// 当たり判定ボックス
	m_vecSegmentPosition.clear();	// 基点の位置
	m_vecVtxPosition.clear();		// 各頂点の位置
}

//==========================================================================
// デストラクタ
//==========================================================================
CCourse::~CCourse()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCourse* CCourse::Create(const std::string& file)
{
	// メモリの確保
	CCourse* pObjMeshField = DEBUG_NEW CCourse;

	if (pObjMeshField != nullptr)
	{
		pObjMeshField->m_FileName = file;

		// 初期化処理
		pObjMeshField->Init();
	}

	return pObjMeshField;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCourse::Init()
{
	HRESULT hr;

	// ロード処理
	Load(m_FileName);

	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist("");
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);

	// オブジェクト3Dメッシュの初期化処理
	Reset();

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCourse::Uninit()
{
	m_pCollisionLineBox.clear();

	// 終了処理
	CObject3DMesh::Uninit();
}

//==========================================================================
// 各頂点計算
//==========================================================================
void CCourse::CalVtxPosition()
{
	
	// 最初と最後、逆方向に少し出す
	MyLib::Vector3 begin, end;
	float angle = 0.0f;

	// 最初
	angle = m_vecSegmentPosition[1].AngleXZ(m_vecSegmentPosition[0]);
	begin = MyLib::Vector3(
		m_vecSegmentPosition[0].x + sinf(angle) * -10.0f,
		m_vecSegmentPosition[0].y,
		m_vecSegmentPosition[0].z + cosf(angle) * -10.0f);

	// 最後
	int endIdx = m_vecSegmentPosition.size() - 1;
	angle = m_vecSegmentPosition[endIdx].AngleXZ(m_vecSegmentPosition[endIdx - 1]);
	end = MyLib::Vector3(
		m_vecSegmentPosition[endIdx].x + sinf(angle) * 10.0f,
		m_vecSegmentPosition[endIdx].y,
		m_vecSegmentPosition[endIdx].z + cosf(angle) * 10.0f);

	m_vecSegmentPosition.insert(m_vecSegmentPosition.begin(), begin);
	m_vecSegmentPosition.push_back(end);

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
	float toataldistance = 0.0f;
	for (int i = 0; i < segmentSize; i++)
	{
		float distance = 0.0f;

		while (1)
		{
			distance += CREATEDISTANCE;

			if (distance >= vecLength[i])
			{
				toataldistance += CREATEDISTANCE - (distance - vecLength[i]);

				distance = vecLength[i];

				m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, toataldistance, 20.0f));
				break;
			}

			toataldistance += CREATEDISTANCE;
			m_vecVtxPosition.push_back(MySpline::GetSplinePosition_NonLoop(m_vecSegmentPosition, toataldistance));
		}
	}
}

//==========================================================================
// リセット
//==========================================================================
void CCourse::Reset()
{
	// 終了処理
	CObject3DMesh::Uninit();

	// 死亡フラグ強制リセット
	SetEnableDeath(false);




	// テクスチャの割り当て
	int texIdx = CTexture::GetInstance()->Regist("");
	BindTexture(texIdx);

	// 種類設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// 各頂点計算
	CalVtxPosition();

	// 各種変数初期化
	SetPosition(MyLib::Vector3(0.0f, 100.0f, 0.0f));				// 位置
	SetWidthBlock(1);		// 幅分割
	SetHeightBlock(static_cast<int>(m_vecVtxPosition.size()) - 1);	// 縦分割
	SetWidthLen(0.0f);		// 縦長さ
	SetHeightLen(0.0f);		// 横長さ

	// オブジェクト3Dメッシュの初期化処理
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// 頂点情報設定
	SetVtx();


	for (const auto& box : m_pCollisionLineBox)
	{
		box->Kill();
	}
	m_pCollisionLineBox.clear();

	MyLib::AABB aabb(-25.0f, 25.0f);
	for (const auto& vtx : m_vecSegmentPosition)
	{
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CCourse::Update()
{
	// 頂点座標計算
	SetVtxPosition();

	SetVtx();
}

//==========================================================================
// 頂点座標
//==========================================================================
void CCourse::SetVtxPosition()
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

		rot.y = m_vecVtxPosition[next].AngleXZ(m_vecVtxPosition[y]);
		UtilFunc::Transformation::RotNormalize(rot.y);

		ImGui::Text("x:%f y:%f z:%f, rot.y:%f", m_vecVtxPosition[y].x, m_vecVtxPosition[y].y, m_vecVtxPosition[y].z, rot.y);

		mtxRotate.RotationYawPitchRoll(rot.y, rot.x, rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// 位置反映
		mtxTrans.Translation(m_vecVtxPosition[y]);
		mtxParent.Multiply(mtxParent, mtxTrans);


		

		// オフセット反映
		offset = MyLib::Vector3(WIDTH, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-WIDTH, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);

		//mtxLeft.Multiply(mtxLeft, mtxRotate);


		// 頂点座標代入
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

		CEffect3D::Create(
			m_vecVtxPosition[y] + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			pVtxPos[idx] + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			pVtxPos[nextidx] + GetPosition(),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

	}

	int i = 0;
	MyLib::Vector3 fieldpos = GetPosition();
	for (const auto& vtx : m_vecSegmentPosition)
	{
		MyLib::Vector3 setpos = vtx + fieldpos;
		m_pCollisionLineBox[i]->SetPosition(setpos);
		i++;
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CCourse::Draw()
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
void CCourse::SetVtx()
{

	MyLib::Vector3 *pVtxPos = GetVtxPos();
	MyLib::Vector3 *pVtxNor = GetVtxNor();
	MyLib::Vector3 vec1, vec2, nor;
	MyLib::Vector3 VtxRight, VtxLeft, VtxNow;
	int nHeight = GetHeightBlock();
	int nWidth = GetWidthBlock();
	float fWidthLen = GetWidthLen();
	float fHeightLen = GetHeightLen();
	int vtxNum = GetNumVertex();

	int heightBlock = static_cast<int>(m_vecVtxPosition.size());

	for (int nCntHeight = 0; nCntHeight < nHeight + 1; nCntHeight++)
	{// 縦の分割分繰り返す

		for (int nCntWidth = 0; nCntWidth < nWidth + 1; nCntWidth++)
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

	// 頂点情報更新
	CObject3DMesh::SetVtx();
}

//==========================================================================
// ロード処理
//==========================================================================
HRESULT CCourse::Load(const std::string& file)
{
	// ファイルを開く
	std::ifstream File(file, std::ios::binary);
	if (!File.is_open()) {

		// 例外処理
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 0.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 500.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1000.0f });
		m_vecSegmentPosition.push_back({ 0.0f, 0.0f, 1800.0f });

		MyLib::AABB aabb(-25.0f, 25.0f);

		for (int i = 0; i < static_cast<int>(m_vecSegmentPosition.size()); i++)
		{
			m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		}

		Save();
		return E_FAIL;
	}

	// 構造体のサイズを取得
	std::streamsize structSize = sizeof(MyLib::Vector3);
	
	// ファイルの末尾までデータを読み込む
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);
	
	// データの個数を計算
	size_t numVectors = fileSize / structSize;
	
	// ベクトルの配列を用意
	m_vecSegmentPosition.resize(numVectors);
	
	// ファイルからデータを読み込む
	File.read(reinterpret_cast<char*>(m_vecSegmentPosition.data()), fileSize);

	// ファイルを閉じる
	File.close();

	return S_OK;
}

//==========================================================================
// セーブ処理
//==========================================================================
void CCourse::Save()
{
	// ファイルを開く
	std::ofstream File(m_FileName, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	// データをバイナリファイルに書き出す
	File.write(reinterpret_cast<char*>(m_vecSegmentPosition.data()), m_vecSegmentPosition.size() * sizeof(MyLib::Vector3));
		
	// ファイルを閉じる
	File.close();
}

//==========================================================================
// 当たり判定ボックス取得
//==========================================================================
CCollisionLine_Box* CCourse::GetCollisionLineBox(int idx)
{
	if (static_cast<int>(m_pCollisionLineBox.size()) <= idx) return nullptr;

	return m_pCollisionLineBox[idx];
}

//==========================================================================
// 基点の位置取得
//==========================================================================
MyLib::Vector3 CCourse::GetVecPosition(int idx)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecSegmentPosition[idx];
}

//==========================================================================
// 基点の位置設定
//==========================================================================
void CCourse::SetVecPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecSegmentPosition.size()) <= idx) return;

	m_vecSegmentPosition[idx] = pos;
}

//==========================================================================
// 各頂点の位置取得
//==========================================================================
MyLib::Vector3 CCourse::GetVecVtxPosition(int idx)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return MyLib::Vector3();

	return m_vecVtxPosition[idx];
}

//==========================================================================
// 各頂点の位置設定
//==========================================================================
void CCourse::SetVecVtxPosition(int idx, const MyLib::Vector3& pos)
{
	if (static_cast<int>(m_vecVtxPosition.size()) <= idx) return;

	m_vecVtxPosition[idx] = pos;
}
