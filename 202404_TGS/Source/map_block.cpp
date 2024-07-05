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
	const int NUM_CHUNK = 5;	// チャンクの数

}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CListManager<CMapBlock> CMapBlock::m_List = {};

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
CListManager<CMapBlock>* CMapBlock::Create()
{
	if (m_List.GetNumAll() > 0) {
		return &m_List;
	}

	// ファイルを開く
	std::ifstream File(FILENAME);
	if (!File.is_open()) {
		return &m_List;
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
			CMapBlock* pBlock = new CMapBlock;
			pBlock->Load(&File);
			m_List.Regist(pBlock);
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{	// 終了文字
			break;
		}
	}

	// ファイルを閉じる
	File.close();

	return &m_List;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CMapBlock::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CMapBlock::Uninit()
{
	// チェックポイントの終了
	for (int i = 0; i < m_CheckpointList.GetNumAll(); i++)
	{
		m_CheckpointList.GetData(i)->Uninit();
	}

	m_CheckpointList.Uninit();


	// 障害物の終了
	for (int i = 0; i < m_ObstacleList.GetNumAll(); i++)
	{
		m_ObstacleList.GetData(i)->Uninit();
	}

	m_ObstacleList.Uninit();
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
}

//==========================================================================
// 更新処理
//==========================================================================
void CMapBlock::Update()
{


}

//==========================================================================
// セーブ
//==========================================================================
void CMapBlock::Save()
{
	
}

//==========================================================================
// ロード
//==========================================================================
void CMapBlock::Load(std::ifstream* pFile)
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
			CCheckpoint* pCheck = CCheckpoint::Create(length);
			m_CheckpointList.Regist(pCheck);

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
void CMapBlock::ObstacleLoad(std::ifstream* pFile)
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
		CMap_Obstacle* pObj = CMap_Obstacle::Create(pManager->GetObstacleInfo(type));
		pObj->SetPosition(pos);
		pObj->SetRotation(rot);
		pObj->SetScale(scale);
		m_ObstacleList.Regist(pObj);
	}
}