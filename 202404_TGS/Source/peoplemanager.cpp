//=============================================================================
// 
//  敵のマネージャ処理 [peoplemanager.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "debugproc.h"
#include "peoplemanager.h"
#include "people.h"
#include "calculation.h"
#include "manager.h"
#include "game.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string FILENAME = "data\\TEXT\\people\\peoplemanager.text";
}

//==========================================================================
// コンストラクタ
//==========================================================================
CPeopleManager::CPeopleManager()
{
	// 値のクリア
	m_vecPattern.clear();			// 配置パターン
	m_vecMotionFileName.clear();	// モーションファイル名
}

//==========================================================================
// デストラクタ
//==========================================================================
CPeopleManager::~CPeopleManager()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPeopleManager* CPeopleManager::Create()
{
	// メモリ確保
	CPeopleManager* pModel = DEBUG_NEW CPeopleManager;

	if (pModel != nullptr)
	{
		// 初期化処理
		HRESULT hr = pModel->ReadText(FILENAME);
		if (FAILED(hr))
		{
			return nullptr;
		}

		// 初期化処理
		hr = pModel->Init();
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	return pModel;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPeopleManager::Init()
{

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPeopleManager::Uninit()
{
	
}

//==========================================================================
// 更新処理
//==========================================================================
void CPeopleManager::Update()
{
	
}

//==========================================================================
// 敵配置
//==========================================================================
void CPeopleManager::SetPeople(MyLib::Vector3 pos, MyLib::Vector3 rot, int nPattern)
{
	SPattern NowPattern = m_vecPattern[nPattern];
	int nNumSpawn = NowPattern.nNumEnemy;	// スポーンする数
	CPeople* pEnemy = nullptr;

	for (const auto& data : NowPattern.enemydata)
	{
		// スポーン時の向きを掛け合わせる
		MyLib::Vector3 spawnPos = pos;

		// 拠点の位置分加算
		spawnPos += data.pos;

		// 敵の生成
		pEnemy = CPeople::Create(
			m_vecMotionFileName[data.nType],	// ファイル名
			spawnPos,							// 位置
			(CPeople::TYPE)data.nType);			// 種類

		if (pEnemy == nullptr)
		{
			delete pEnemy;
			break;
		}

		// 向き設定
		pEnemy->SetRotation(rot);
		pEnemy->SetRotDest(rot.y);
	}

	
}

//==========================================================================
// 外部ファイル読み込み処理
//==========================================================================
HRESULT CPeopleManager::ReadText(const std::string& filename)
{
	// ファイルを開く
	std::ifstream File(filename);
	if (!File.is_open()) {
		return E_FAIL;
	}

	// コメント用
	std::string hoge;
	std::string charaFile;

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


		if (line.find("MODEL_FILENAME") != std::string::npos)
		{// MODEL_FILENAMEでモデル名読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			// 情報渡す
			lineStream >>
				hoge >>		// ＝
				hoge >>		// ＝
				charaFile;	// モデルファイル名
			continue;
		}

		if (line.find("COLLIDER_FILENAME") != std::string::npos)
		{// COLLIDER_FILENAMEでコライダー読み込み

			// ストリーム作成
			std::istringstream lineStream(line);

			//// 情報渡す
			//lineStream >>
			//	hoge >>
			//	hoge >>			// ＝
			//	obstacleInfo.colliderFile;	// コライダー名


			
			continue;
		}

		//if (line.find("SETUP") != std::string::npos)
		//{// SETUPで情報読み込み

		//	while (line.find("END_SETUP") == std::string::npos)
		//	{
		//		std::getline(File, line);
		//		if (line.find("IS_AIR") != std::string::npos)
		//		{// IS_AIRで空気貫通読み込み

		//			// ストリーム作成
		//			std::istringstream lineStream(line);

		//			// 情報渡す
		//			lineStream >>
		//				hoge >>
		//				hoge >>	// ＝
		//				obstacleInfo.setup.isAir;	// 空気貫通
		//			continue;
		//		}

		//		if (line.find("IS_MOVE") != std::string::npos)
		//		{// IS_MOVEで動き読み込み

		//			// ストリーム作成
		//			std::istringstream lineStream(line);

		//			// 情報渡す
		//			lineStream >>
		//				hoge >>
		//				hoge >>	// ＝
		//				obstacleInfo.setup.isMove;	// 動き
		//			continue;
		//		}
		//	}
		//}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}


	// ファイルを閉じる
	File.close();
	return S_OK;
}
