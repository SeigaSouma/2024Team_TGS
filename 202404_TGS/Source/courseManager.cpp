//=============================================================================
// 
//  コースマネージャ処理 [courseManager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "courseManager.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"

#include "course.h"
#include "waterfield.h"
#include "stonewall.h"
#include "stonewall_front.h"
#include "map_block.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\map\\chunkdata.bin";
	const int NUM_CHUNK = 5;	// チャンクの数

	const std::vector<MyLib::Vector3> DEFAULT_SEGMENTPOS =
	{
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 2500.0f, 0.0f, 0.0f },
		{ 5000.0f, 0.0f, 0.0f },
		{ 7500.0f, 0.0f, 0.0f },
		{ 9000.0f, 0.0f, 0.0f },
		{ 9000.0f, 0.0f, 0.0f },
	};
	const float DISTANCE_TO_CHUNCK = 3000.0f;	// チャンク同士の間隔
}
CCourseManager* CCourseManager::m_ThisPtr = nullptr;	// 自身のポインタ
const float CCourseManager::m_fBlockLength = 9000.0f;	// ブロックの長さ

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

		// データをバイナリファイルに書き出す
		File.write(reinterpret_cast<char*>(data.data()), data.size() * sizeof(MyLib::Vector3));
	}

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

		m_vecAllSegmentPos.emplace_back();

		m_vecAllSegmentPos[0] = DEFAULT_SEGMENTPOS;
		m_vecAllSegmentPos[0].insert(m_vecAllSegmentPos[0].begin(), MyLib::Vector3(0.0f, 0.0f, 0.0f));
		m_vecAllSegmentPos[0].push_back(DEFAULT_SEGMENTPOS.back());

		Save();
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
	m_vecAllSegmentPos.clear();
	m_vecAllSegmentPos.emplace_back(std::vector<MyLib::Vector3>(numVectors));


	// ファイルからデータを読み込む
	File.read(reinterpret_cast<char*>(m_vecAllSegmentPos[0].data()), fileSize);

	// ファイルを閉じる
	File.close();



	//=============================
	// ランダム選出
	//=============================
	int segmentSize = static_cast<int>(m_vecAllSegmentPos.size()) - 1;
	
	std::vector<int> randIdx;
	for (int i = 0; i < NUM_CHUNK; i++)
	{
		randIdx.push_back(UtilFunc::Transformation::Random(0, segmentSize));
	}

	// 一本のコースにする
	std::vector<MyLib::Vector3> segmentpos;	// 基点の位置
	MyLib::Vector3 start;
	std::vector<MyLib::Vector3> vecstart;	// 基点の位置

	for (const auto& idx : randIdx)
	{
		for (const auto& pos : m_vecAllSegmentPos[idx])
		{
			segmentpos.push_back(start + pos);
		}
		start = segmentpos.back();
		vecstart.push_back(start);
	}

	//=============================
	// コース作成
	//=============================
	CCourse* pCourse = CCourse::Create("data\\TEXT\\map\\course.bin");
	pCourse->SetVecPosition(segmentpos);
	pCourse->Reset();
	CGame::GetInstance()->SetCource(pCourse);

	// ランダム選出されたブロックに付随する、チェックポイント、障害物の生成
	// Blockの読み込み(障害物、チェックポイント)
	CMapBlock::Load();

	// 距離にあわせた配置を行う
	for (int i = 0; i < NUM_CHUNK; i++)
	{
		CMapBlock* pBlock = new CMapBlock;

		if (pBlock != nullptr)
		{
			pBlock->Set(randIdx[i], vecstart[i], CCourseManager::GetBlockLength() * i);
		}
	}


	//この中で障害物、チェックポイント


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

//==========================================================================
// 基点追加
//==========================================================================
void CCourseManager::AddSegmentPos()
{
	m_vecAllSegmentPos.emplace_back();
	m_vecAllSegmentPos.back() = DEFAULT_SEGMENTPOS;
}

//==========================================================================
// 基点削除
//==========================================================================
void CCourseManager::SubSegmentPos()
{
	if (static_cast<int>(m_vecAllSegmentPos.size()) < 1) return;
	m_vecAllSegmentPos.pop_back();
}

//==========================================================================
// 基点削除
//==========================================================================
void CCourseManager::SubSegmentPos(int idx)
{
	if (static_cast<int>(m_vecAllSegmentPos.size()) < 1) return;

	if (static_cast<int>(m_vecAllSegmentPos.size()) <= idx) return;

	std::vector<std::vector<MyLib::Vector3>>::iterator itr = m_vecAllSegmentPos.begin();
	std::advance(itr, idx);
	m_vecAllSegmentPos.erase(itr);

}
