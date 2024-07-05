//=============================================================================
// 
//  コースマネージャ処理 [courseManager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "courseManager.h"
#include "manager.h"
#include "calculation.h"

#include "course.h"
#include "waterfield.h"
#include "stonewall.h"
#include "stonewall_front.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\map\\chunkdata.bin";
	const int NUM_CHUNK = 5;	// チャンクの数

}
CCourseManager* CCourseManager::m_ThisPtr = nullptr;	// 自身のポインタ

//==========================================================================
// コンストラクタ
//==========================================================================
CCourseManager::CCourseManager()
{

}

//==========================================================================
// デストラクタ
//==========================================================================
CCourseManager::~CCourseManager()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CCourseManager *CCourseManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// メモリの確保
		m_ThisPtr = DEBUG_NEW CCourseManager;

		if (m_ThisPtr != nullptr)
		{
			// 初期化処理
			m_ThisPtr->Init();
		}
	}

	return m_ThisPtr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCourseManager::Init()
{

	// 読み込み
	Load();
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCourseManager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 削除
//==========================================================================
void CCourseManager::Kill()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CCourseManager::Update()
{
	

}

//==========================================================================
// セーブ
//==========================================================================
void CCourseManager::Save()
{
	// ファイルを開く
	std::ofstream File(FILENAME, std::ios::binary);
	if (!File.is_open()) {
		return;
	}

	std::vector<std::vector<MyLib::Vector3>> savedata = m_vecAllSegmentPos;

	// それぞれのブロックの最初と最後消す
	for (auto& data : savedata)
	{
		data.erase(data.begin());
		data.pop_back();
	}


	// データをバイナリファイルに書き出す
	File.write(reinterpret_cast<char*>(savedata.data()), savedata.size() * sizeof(MyLib::Vector3));

	// ファイルを閉じる
	File.close();
}

//==========================================================================
// ロード
//==========================================================================
void CCourseManager::Load()
{
	
	//=============================
	// 全ブロック情報(コース)を読み込む。基点の位置
	//=============================
	// ファイルを開く
	std::ifstream File(FILENAME, std::ios::binary);
	if (!File.is_open()) {
		// 例外処理
		return;
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
	m_vecAllSegmentPos.resize(numVectors);

	// ファイルからデータを読み込む
	File.read(reinterpret_cast<char*>(m_vecAllSegmentPos.data()), fileSize);

	// ファイルを閉じる
	File.close();



	//=============================
	// ランダム選出
	//=============================
	int segmentSize = static_cast<int>(m_vecAllSegmentPos.size());
	
	std::vector<int> randIdx;
	for (int i = 0; i < NUM_CHUNK; i++)
	{
		randIdx.push_back(UtilFunc::Transformation::Random(0, segmentSize));
	}

	// 一本のコースにする
	std::vector<MyLib::Vector3> segmentpos;	// 基点の位置
	MyLib::Vector3 start;

	for (const auto& idx : randIdx)
	{
		for (const auto& pos : m_vecAllSegmentPos[idx])
		{
			segmentpos.push_back(start + pos);
		}
		start = segmentpos.back();
	}


	// ランダム選出されたブロックに付随する、チェックポイント、障害物の生成
	// Blockの読み込み(障害物、チェックポイント)

	// pBlock->Set(0, start位置);
	//この中で障害物、チェックポイント


	//=============================
	// コース作成
	//=============================
	CCourse* pCourse = CCourse::Create("data\\TEXT\\map\\course.bin");
	pCourse->SetVecPosition(segmentpos);
	pCourse->Reset();

	//=============================
	// 石垣(奥)
	//=============================
	CStoneWall* pStoneWall = CStoneWall::Create();

	// 基点地点設定
	pStoneWall->SetVecPosition(pCourse->GetVecPosition());
	pStoneWall->Reset();

	std::vector<CCourse::VtxInfo> vtxInfo = pCourse->GetVecVtxinfo();
	std::vector<MyLib::Vector3> vecpos;

	MyLib::Vector3 setpos;
	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * -600.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * -600.0f;
		vecpos.push_back(setpos);
	}

	// 各頂点座標
	pStoneWall->SetVecVtxPosition(vecpos);
	pStoneWall->BindVtxPosition();


	//=============================
	// 石垣(手前)
	//=============================
	CStoneWall* pStoneWall_Front = CStoneWall_Front::Create();

	// 基点地点設定
	pStoneWall_Front->SetVecPosition(pCourse->GetVecPosition());
	pStoneWall_Front->Reset();

	vtxInfo = pCourse->GetVecVtxinfo();
	vecpos.clear();

	for (const auto& info : vtxInfo)
	{
		setpos.x = info.pos.x + sinf(D3DX_PI + info.rot.y) * 800.0f;
		setpos.y = info.pos.y;
		setpos.z = info.pos.z + cosf(D3DX_PI + info.rot.y) * 800.0f;
		vecpos.push_back(setpos);
	}

	// 各頂点座標
	pStoneWall_Front->SetVecVtxPosition(vecpos);
	pStoneWall_Front->BindVtxPosition();


	//=============================
	// うねりの街フィールド
	//=============================
	CMapMesh* pTownField = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS);
	pTownField->SetVecPosition(pCourse->GetVecPosition());
	pTownField->Reset();

	// 石垣の頂上に頂点をそろえる
	pTownField->SetVecVtxPosition(pStoneWall->GetVecTopPosition());
	pTownField->BindVtxPosition();


	//=============================
	// うねりの街フィールド(手前)
	//=============================
	CMapMesh* pTownFieldFront = CMapMesh::Create(CMapMesh::MeshType::TYPE_TOWNFIELD_SINUOUS_FRONT);
	pTownFieldFront->SetVecPosition(pCourse->GetVecPosition());
	pTownFieldFront->Reset();

	// 石垣の頂上に頂点をそろえる
	pTownFieldFront->SetVecVtxPosition(pStoneWall_Front->GetVecTopPosition());
	pTownFieldFront->BindVtxPosition();

}

