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

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int WIDTH_BLOCK = 2;
}

//==========================================================================
// コンストラクタ
//==========================================================================
CCourse::CCourse(int nPriority, const LAYER layer) : CObject3DMesh(nPriority, layer)
{
	m_pCollisionLineBox.clear();	// 当たり判定ボックス

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

	// 各種変数初期化
	SetPosition(MyLib::Vector3(0.0f, 100.0f, 0.0f));				// 位置
	SetWidthBlock(1);		// 幅分割
	SetHeightBlock(static_cast<int>(m_LineInfo.size()) - 1);	// 縦分割
	SetWidthLen(0.0f);		// 縦長さ
	SetHeightLen(0.0f);		// 横長さ

	// オブジェクト3Dメッシュの初期化処理
	hr = CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCourse::Uninit()
{

	// 終了処理
	CObject3DMesh::Uninit();
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

	// 各種変数初期化
	SetPosition(MyLib::Vector3(0.0f, 100.0f, 0.0f));				// 位置
	SetWidthBlock(1);		// 幅分割
	SetHeightBlock(static_cast<int>(m_LineInfo.size()) - 1);	// 縦分割
	SetWidthLen(0.0f);		// 縦長さ
	SetHeightLen(0.0f);		// 横長さ

	// オブジェクト3Dメッシュの初期化処理
	CObject3DMesh::Init(CObject3DMesh::TYPE_FIELD);

	// 頂点情報設定
	SetVtx();
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

	for (int y = 0; y < static_cast<int>(m_LineInfo.size()); y++)
	{
		int idx = (WIDTH_BLOCK * y);
		int nextidx = (WIDTH_BLOCK * y) + 1;

		// 今回の情報
		LineInfo info = m_LineInfo[y];

		mtxParent.Identity();
		mtxLeft.Identity();
		mtxRight.Identity();

		// 位置反映
		mtxTrans.Translation(info.pos);
		mtxParent.Multiply(mtxParent, mtxTrans);

		// 向き反映
		mtxRotate.RotationYawPitchRoll(info.rot.y, info.rot.x, info.rot.z);
		mtxParent.Multiply(mtxParent, mtxRotate);

		// オフセット反映
		offset = MyLib::Vector3(info.width, 0.0f, 0.0f);
		mtxLeft.Translation(offset);

		offset = MyLib::Vector3(-info.width, 0.0f, 0.0f);
		mtxRight.Translation(offset);

		mtxLeft.Multiply(mtxLeft, mtxParent);
		mtxRight.Multiply(mtxRight, mtxParent);


		// 頂点座標代入
		pVtxPos[idx] = mtxLeft.GetWorldPosition();
		pVtxPos[nextidx] = mtxRight.GetWorldPosition();

		MyLib::Vector3 setpos = (pVtxPos[idx] + pVtxPos[nextidx]) * 0.5f;
		setpos.y = GetPosition().y;

		m_pCollisionLineBox[y]->SetPosition(setpos);


		/*CEffect3D::Create(
			pVtxPos[idx] + MyLib::Vector3(0.0f, setpos.y, 0.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

		CEffect3D::Create(
			pVtxPos[nextidx] + MyLib::Vector3(0.0f, setpos.y, 0.0f),
			MyLib::Vector3(0.0f, 0.0f, 0.0f),
			D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
			20.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);*/
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

	int heightBlock = static_cast<int>(m_LineInfo.size());

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

		LineInfo info;
		info.pos = MyLib::Vector3(0.0f, 0.0f, 0.0f);
		info.rot = 0.0f;
		info.width = 200.0f;

		m_LineInfo.push_back(info);

		info.pos = MyLib::Vector3(0.0f, 0.0f, 200.0f);
		m_LineInfo.push_back(info);

		MyLib::AABB aabb(-25.0f, 25.0f);
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)));
		m_pCollisionLineBox.push_back(CCollisionLine_Box::Create(aabb, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)));

		Save();
		return E_FAIL;
	}

	// 構造体のサイズを取得
	std::streamsize structSize = sizeof(LineInfo);
	
	// ファイルの末尾までデータを読み込む
	File.seekg(0, std::ios::end);
	std::streampos fileSize = File.tellg();
	File.seekg(0, std::ios::beg);
	
	// データの個数を計算
	size_t numVectors = fileSize / structSize;
	
	// ベクトルの配列を用意
	m_LineInfo.resize(numVectors);
	
	// ファイルからデータを読み込む
	File.read(reinterpret_cast<char*>(m_LineInfo.data()), fileSize);

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
	File.write(reinterpret_cast<char*>(m_LineInfo.data()), m_LineInfo.size() * sizeof(LineInfo));
		
	// ファイルを閉じる
	File.close();
}

//==========================================================================
// 辺情報取得
//==========================================================================
CCourse::LineInfo CCourse::GetLineInfo(int idx)
{
	if (static_cast<int>(m_LineInfo.size()) <= idx) return LineInfo();

	return m_LineInfo[idx];
}

//==========================================================================
// 辺情報設定
//==========================================================================
void CCourse::SetLineInfo(int idx, const LineInfo& info)
{
	if (static_cast<int>(m_LineInfo.size()) <= idx) return;

	m_LineInfo[idx] = info;
}

//==========================================================================
// 辺情報追加
//==========================================================================
void CCourse::PushLineInfo()
{
	m_LineInfo.push_back(LineInfo());
}

//==========================================================================
// 辺情報削除
//==========================================================================
void CCourse::PopLineInfo()
{
	if (static_cast<int>(m_LineInfo.size()) >= 1)
	{
		m_LineInfo.pop_back();
	}
}