//=============================================================================
// 
//  マップブロック管理処理 [map_block.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "map_block.h"
#include "checkpoint.h"
#include "map_obstacle.h"
#include "map_obstacleManager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\map\\block.txt";
	const std::string FILE_CHECKPOINT = "data\\TEXT\\map\\checkpoint.bin";	// チェックポイントのセーブファイル
	const std::string FILE_OBSTACLE = "data\\TEXT\\map\\obstacle.bin";		// 障害物のセーブファイル
	const int NUM_CHUNK = 5;	// チャンクの数

}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CListManager<CMapBlock> CMapBlock::m_List = {};
CListManager<CMapBlockInfo> CMapBlock::m_InfoList = {};

//==========================================================================
// コンストラクタ
//==========================================================================
CMapBlock::CMapBlock()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CMapBlock::~CMapBlock()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CListManager<CMapBlockInfo>* CMapBlock::Load()
{
	if (m_InfoList.GetNumAll() > 0) {
		return &m_InfoList;
	}

#if 0
	// ファイルを開く
	std::ifstream File(FILENAME);
	if (!File.is_open()) {
		return &m_InfoList;
	}

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// ストリーム作成
		std::istringstream lineStream(line);

		if (line.find("BLOCKSET") != std::string::npos)
		{// MODELSETで配置情報読み込み

			// 生成してリストの管理下に
			CMapBlockInfo* pBlock = new CMapBlockInfo;
			pBlock->Init();
			pBlock->Load(&File);
			m_InfoList.Regist(pBlock);
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{	// 終了文字
			break;
		}
	}

	// ファイルを閉じる
	File.close();
#else
	LoadBin();
#endif

	return &m_InfoList;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMapBlock::Init()
{
	m_InfoList.Init();
	m_CheckpointList.Init();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMapBlock::Uninit()
{
	// チェックポイントの終了
	m_CheckpointList.Uninit();

	// 障害物の終了
	m_ObstacleList.Uninit();

	delete this;
}

//==========================================================================
// 削除
//==========================================================================
void CMapBlock::Kill()
{
	// マップブロックの終了
	for (int i = 0; i < m_List.GetNumAll(); i++)
	{
		m_List.GetData(i)->Uninit();
	}
	m_List.Uninit();

	// ブロック配置情報の終了
	for (int i = 0; i < m_InfoList.GetNumAll(); i++)
	{
		m_InfoList.GetData(i)->Uninit();
	}

	m_List.Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CMapBlock::Update()
{


}

//==========================================================================
// チェックポイントセーブ
//==========================================================================
void CMapBlock::SaveBin_CheckPoint()
{
	// ファイルを開く
	std::ofstream File(FILE_CHECKPOINT, std::ios::binary);
	if (!File.is_open()) {
		return;
	}



	// 先頭を保存
	std::list<CMapBlockInfo*>::iterator itr = m_InfoList.GetEnd();
	CMapBlockInfo* pObj = nullptr;
	
	std::vector<std::vector<float>> savedata;

	// データコピー
	while (m_InfoList.ListLoop(itr))
	{
		savedata.push_back((*itr)->GetCheckpointInfo());
	}

	// 外側ベクトルのサイズを書き込む
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));


	for (const auto& inner : savedata)
	{
		// データの個数を計算
		size_t vecSize = inner.size();

		// ベクトルのサイズをセーブ
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// ベクトル内の要素をセーブ
		File.write(reinterpret_cast<const char*>(inner.data()), vecSize * sizeof(float));
	}



	// ファイルを閉じる
	File.close();
}

//==========================================================================
// 障害物セーブ
//==========================================================================
void CMapBlock::SaveBin_Obstacle()
{
	// ファイルを開く
	std::ofstream File(FILE_OBSTACLE, std::ios::binary);
	if (!File.is_open()) {
		return;
	}


	// 先頭を保存
	std::list<CMapBlockInfo*>::iterator itr = m_InfoList.GetEnd();
	CMapBlockInfo* pObj = nullptr;

	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> savedata;

	// データコピー
	while (m_InfoList.ListLoop(itr))
	{
		savedata.push_back((*itr)->GetObstacleInfo());
	}

	// 外側ベクトルのサイズを書き込む
	size_t outer_size = savedata.size();
	File.write(reinterpret_cast<const char*>(&outer_size), sizeof(outer_size));


	for (const auto& inner : savedata)
	{
		// データの個数を計算
		size_t vecSize = inner.size();

		// ベクトルのサイズをセーブ
		File.write(reinterpret_cast<const char*>(&vecSize), sizeof(vecSize));

		// ベクトル内の要素をセーブ
		File.write(reinterpret_cast<const char*>(inner.data()), vecSize * sizeof(CMapBlockInfo::SObsacleInfo));
	}


	// ファイルを閉じる
	File.close();
}

//==========================================================================
// セーブ
//==========================================================================
void CMapBlock::SaveBin()
{
	// チェックポイントセーブ
	SaveBin_CheckPoint();

	// 障害物セーブ
	SaveBin_Obstacle();
}

//==========================================================================
// ロード
//==========================================================================
void CMapBlock::LoadBin()
{
	// チェックポイントロード
	std::vector<std::vector<float>> checkpoint = LoadBin_CheckPoint();
	if (checkpoint.empty())
	{
		checkpoint.emplace_back();
		checkpoint.back().push_back(0.0f);
	}

	// 障害物ロード
	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> obstacle = LoadBin_Obstacle();
	if (obstacle.empty())
	{
		obstacle.emplace_back();
		obstacle.back().push_back(CMapBlockInfo::SObsacleInfo());
	}

	// 全削除
	m_InfoList.KillAll();

	// ブロック生成
	int i = 0;
	for (int i = 0; i < static_cast<int>(checkpoint.size()); i++)
	{
		// 生成してリストの管理下に
		CMapBlockInfo* pBlock = DEBUG_NEW CMapBlockInfo;
		pBlock->Init();

		pBlock->SetCheckpointInfo(checkpoint[i]);
		pBlock->SetObstacleInfo(obstacle[i]);

		m_InfoList.Regist(pBlock);
	}
}

//==========================================================================
// チェックポイント読み込み
//==========================================================================
std::vector<std::vector<float>> CMapBlock::LoadBin_CheckPoint()
{
	// ファイルを開く
	std::ifstream File(FILE_CHECKPOINT, std::ios::binary);
	if (!File.is_open()) {
		// 例外処理
		return std::vector<std::vector<float>>();
	}

	// まず、外側のベクトルのサイズを読み込む
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

	std::vector<std::vector<float>> loaddata(outer_size);

	// 各内側のベクトルに対してデータを読み込む
	for (auto& inner_vector : loaddata)
	{
		// 内側のベクトルのサイズを読み込む
		size_t inner_size;
		File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

		inner_vector.resize(inner_size);

		// データ読み込み
		File.read(reinterpret_cast<char*>(inner_vector.data()), inner_size * sizeof(float));
	}

	// ファイルを閉じる
	File.close();

	return loaddata;
}

//==========================================================================
// 障害物読み込み
//==========================================================================
std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> CMapBlock::LoadBin_Obstacle()
{
	// ファイルを開く
	std::ifstream File(FILE_OBSTACLE, std::ios::binary);
	if (!File.is_open()) {
		// 例外処理
		return std::vector<std::vector<CMapBlockInfo::SObsacleInfo>>();
	}

	// まず、外側のベクトルのサイズを読み込む
	size_t outer_size;
	File.read(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));

	std::vector<std::vector<CMapBlockInfo::SObsacleInfo>> loaddata(outer_size);

	// 各内側のベクトルに対してデータを読み込む
	for (auto& inner_vector : loaddata)
	{
		// 内側のベクトルのサイズを読み込む
		size_t inner_size;
		File.read(reinterpret_cast<char*>(&inner_size), sizeof(inner_size));

		inner_vector.resize(inner_size);

		// データ読み込み
		File.read(reinterpret_cast<char*>(inner_vector.data()), inner_size * sizeof(CMapBlockInfo::SObsacleInfo));
	}

	// ファイルを閉じる
	File.close();

	return loaddata;
}

//==========================================================================
// 配置位置設定
//==========================================================================
void CMapBlock::Set(const int Idx, const MyLib::Vector3& startpos, float startlength)
{
	CMapBlockInfo* pInfo = m_InfoList.GetData(Idx);
	if (pInfo == nullptr)return;

	// オブジェクトの配置
	{
		CMap_ObstacleManager* pManager = CMap_ObstacleManager::GetInstance();

		for (const auto& it : pInfo->GetObstacleInfo())
		{
			CMap_Obstacle* pObj = CMap_Obstacle::Create(pManager->GetObstacleInfo(it.nType), it.pos + startpos);
			pObj->SetRotation(it.rot);
			pObj->SetScale(it.scale);
		}
	}

	// チェックポイントの配置
	{
		for (const auto& it : pInfo->GetCheckpointInfo())
		{
			CCheckpoint* p = CCheckpoint::Create(it + startlength);
			m_CheckpointList.Regist(p);
		}
	}

	m_List.Regist(this);
}

//==========================================================================
// 情報追加
//==========================================================================
void CMapBlock::AddInfo()
{
	// 生成してリストの管理下に
	CMapBlockInfo* pBlock = DEBUG_NEW CMapBlockInfo;
	pBlock->Init();
	m_InfoList.Regist(pBlock);
}

//==========================================================================
// 情報削除
//==========================================================================
void CMapBlock::SubInfo()
{
	m_InfoList.Delete(m_InfoList.GetData(m_InfoList.GetNumAll() - 1));
}

//==========================================================================
// データの削除
//==========================================================================
void CMapBlock::DataDelete()
{
	//// チェックポイントの終了
	//for (int i = 0; i < m_CheckpointList.GetNumAll(); i++)
	//{
	//	m_CheckpointList.GetData(i)->Uninit();
	//}

	//m_CheckpointList.Uninit();


	//// 障害物の終了
	//for (int i = 0; i < m_ObstacleList.GetNumAll(); i++)
	//{
	//	m_ObstacleList.GetData(i)->Uninit();
	//}

	//m_ObstacleList.Uninit();
}

//void CMapBlock::DataDelete()
//{
//
//}
//
//void CMapBlock::InfoDelete()
//{
//
//}

//==========================================================================
// コンストラクタ
//==========================================================================
CMapBlockInfo::CMapBlockInfo()
{
	m_nLevel = 0;
}

//==========================================================================
// デストラクタ
//==========================================================================
CMapBlockInfo::~CMapBlockInfo()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMapBlockInfo::Init()
{
	m_CheckpointList.clear();
	m_ObstacleList.clear();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMapBlockInfo::Uninit()
{
	m_CheckpointList.clear();
	m_ObstacleList.clear();

	delete this;
}

//==========================================================================
// ロード
//==========================================================================
void CMapBlockInfo::Load(std::ifstream* pFile)
{
	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;

	while (line.find("END_BLOCKSET") == std::string::npos)
	{
		std::getline(*pFile, line);

		if (line.find("CHECKPOINTSET") != std::string::npos)
		{// チェックポイント配置

			float length;

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>	// ＝
				length;	// 配置物の種類

			// 生成して管理
			m_CheckpointList.push_back(length);

			continue;
		}

		if (line.find("LEVELSET") != std::string::npos)
		{// 難易度設定

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>	// ＝
				m_nLevel;	// 難易度の設定

			continue;
		}

		if (line.find("MODELSET") != std::string::npos)
		{// POSで位置

			// 障害物情報読み込み
			ObstacleLoad(pFile);

			continue;
		}
	}
}

//==========================================================================
// 障害物読み込み
//==========================================================================
void CMapBlockInfo::ObstacleLoad(std::ifstream* pFile)
{
	// 読み込み情報
	int type;
	MyLib::Vector3 pos, rot, scale(1.0f);
	CMap_ObstacleManager* pManager = CMap_ObstacleManager::GetInstance();

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;

	while (line.find("END_MODELSET") == std::string::npos)
	{
		std::getline(*pFile, line);

		if (line.find("TYPE") != std::string::npos)
		{// TYPEで配置物の種類

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>	// ＝
				type;	// 配置物の種類
			continue;
		}

		if (line.find("POS") != std::string::npos)
		{// POSで位置

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>						// ＝
				pos.x >> pos.y >> pos.z;	// 位置
			continue;
		}

		if (line.find("ROT") != std::string::npos)
		{// ROTで向き

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>						// ＝
				rot.x >> rot.y >> rot.z;	// 向き
			continue;
		}

		if (line.find("SCALE") != std::string::npos)
		{// SCALEで拡大率

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>						// ＝
				scale.x >> scale.y >> scale.z;	// 拡大率
			continue;
		}
	}

	// 生成
	if (type < static_cast<int>(pManager->GetObstacleInfo().size()))
	{
		SObsacleInfo info;
		info.pos = pos;
		info.rot = rot;
		info.scale = scale;
		info.nType = type;
		m_ObstacleList.push_back(info);
	}
}