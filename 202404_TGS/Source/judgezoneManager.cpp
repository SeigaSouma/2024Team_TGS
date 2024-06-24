//=============================================================================
// 
//  判定ゾーンマネージャ [judgezoneManager.cpp]
//  Author : 石原颯馬
// 
//============================================================================
#include "judgezoneManager.h"
#include "judgezone.h"
#include "judge.h"
#include "judgeobj.h"
#include "game.h"
#include "course.h"
#include "texture.h"
#include "object2D.h"
#include "spline.h"
#include "objectBillboard.h"
#include <map>

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	std::map<CJudge::JUDGE, std::string> TEXTURE =
	{
		{CJudge::JUDGE::JUDGE_AAA,"data\\TEXTURE\\judge_test_01.png"},
		{CJudge::JUDGE::JUDGE_BBB,"data\\TEXTURE\\judge_test_02.png"},
		{CJudge::JUDGE::JUDGE_CCC,"data\\TEXTURE\\judge_test_03.png"},
		{CJudge::JUDGE::JUDGE_DDD,"data\\TEXTURE\\judge_test_04.png"},
	};
}
CJudgeZoneManager* CJudgeZoneManager::m_ThisPtr = nullptr;

//==========================================================================
// コンストラクタ
//==========================================================================
CJudgeZoneManager::CJudgeZoneManager()
{
	
}

//==========================================================================
// デストラクタ
//==========================================================================
CJudgeZoneManager::~CJudgeZoneManager()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CJudgeZoneManager *CJudgeZoneManager::Create()
{
	if (m_ThisPtr == nullptr)
	{
		// メモリの確保
		m_ThisPtr = DEBUG_NEW CJudgeZoneManager;

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
HRESULT CJudgeZoneManager::Init()
{
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CJudgeZoneManager::Uninit()
{
	delete m_ThisPtr;
	m_ThisPtr = nullptr;
}

//==========================================================================
// 判定確認処理
//==========================================================================
void CJudgeZoneManager::Check(float progress)
{
	CListManager<CJudgeZone>::Iterator itr = m_zoneList.GetEnd();
	while (m_zoneList.ListLoop(itr))
	{
		if ((*itr)->IsEnable())
		{
			CJudgeZone::SZone zone = (*itr)->GetZone();
			if (progress >= zone.start && progress <= zone.end)
			{//範囲内
				(*itr)->Check();
			}
			else if (progress > zone.end)
			{//終了（判定）
				CJudge::JUDGE judge = (*itr)->Judge();	//ここに判定が入ってる
				CJudgeObj::Create(MyLib::Vector3(400.0f, 100.0f, 0.0f), judge);

				(*itr)->Uninit();
			}

#ifdef _DEBUG
			CCourse* pCource = CGame::GetInstance()->GetCourse();
			MyLib::Vector3 pos;
			float length = pCource->GetCourceLength();

			//スタート
			pos = MySpline::GetSplinePosition_NonLoop(pCource->GetVecPosition(), length * (*itr)->GetZone().start);
			pos.y = (*itr)->GetBorder();
			CEffect3D::Create(
				pos,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);

			//終了
			pos = MySpline::GetSplinePosition_NonLoop(pCource->GetVecPosition(), length * (*itr)->GetZone().end);
			pos.y = (*itr)->GetBorder();
			CEffect3D::Create(
				pos,
				MyLib::Vector3(0.0f, 0.0f, 0.0f),
				D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f),
				40.0f, 2, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif // _DEBUG
		}
	}
	
	// 無効化したゾーン破棄
	Release();
}

//==========================================================================
// 無効ゾーン破棄処理
//==========================================================================
void CJudgeZoneManager::Release()
{
	std::list<CJudgeZone*> removeList;
	std::list<CJudgeZone*>::iterator itr;
	itr = m_zoneList.GetEnd();
	CJudgeZone* pObj = nullptr;
	while (m_zoneList.ListLoop(itr))
	{
		if (!(*itr)->IsEnable())
		{
			removeList.push_back((*itr));
		}
	}
	
	for (itr = removeList.begin(); itr != removeList.end(); itr++)
	{
		m_zoneList.Delete((*itr));
	}
}

//==========================================================================
// 全ゾーン破棄処理
//==========================================================================
void CJudgeZoneManager::ReleaseAll()
{
	CJudgeZone* pZone = nullptr;
	while (m_zoneList.ListLoop(&pZone))
	{
		m_zoneList.Delete(pZone);	// リスト除外
		pZone->Uninit();			// 終了
		delete pZone;				// 破棄
	}
}

//==========================================================================
// 判定ゾーン類読み込み処理
//==========================================================================
void CJudgeZoneManager::Load(std::string path)
{
	CJudgeZone* pJudgeZone = nullptr;
	std::string aPath[2];

	// ファイルを開く
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("USEZONE") != std::string::npos)
			{// TYPEで配置物の種類

				// ストリーム作成
				std::istringstream lineStream(line);

				// 情報渡す
				std::string path;
				lineStream >>
					hoge >>
					hoge >>		// ＝
					path;		// 数値
				path = UtilFunc::Transformation::ReplaceBackslash(path);

				//判定ゾーン読み込み
				LoadZone(path);
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// ファイルを閉じる
		File.close();
	}
}

//==========================================================================
// 判定ゾーン読み込み処理
//==========================================================================
void CJudgeZoneManager::LoadZone(std::string path)
{
	CJudgeZone* pJudgeZone = nullptr;
	std::string aPath[2];

	// ファイルを開く
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("ZONESET") != std::string::npos)
			{// MODELSETで配置情報読み込み

				// 読み込み情報
				CJudgeZone::SZone zone = { 0.0f,1.0f };
				float border = 0.0f;

				while (line.find("END_ZONESET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("START") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							zone.start;	// 数値
					}
					if (line.find("END") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							zone.end;	// 数値
					}
					if (line.find("BORDER") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							border;		// 数値
					}
					else if (line.find("CONDITION_TOP") != std::string::npos)
					{// POSで位置

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							aPath[0];	// パス
						aPath[0] = UtilFunc::Transformation::ReplaceBackslash(aPath[0]);
					}
					else if (line.find("CONDITION_UNDER") != std::string::npos)
					{// ROTで向き

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							aPath[1];	// パス
						aPath[1] = UtilFunc::Transformation::ReplaceBackslash(aPath[1]);
					}
				}

				// 取り込み
				if (pJudgeZone == nullptr)
				{
					pJudgeZone = CJudgeZone::Create(zone.start, zone.end, border);
				}
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// ファイルを閉じる
		File.close();
	}

	// 条件読み込み
	if (pJudgeZone != nullptr)
	{
		pJudgeZone->SetInfo(CJudge::BORDER::UP, LoadCondition(aPath[CJudge::BORDER::UP]));
		pJudgeZone->SetInfo(CJudge::BORDER::DOWN, LoadCondition(aPath[CJudge::BORDER::DOWN]));
		m_zoneList.Regist(pJudgeZone);
	}
}

//==========================================================================
// 条件読み込み処理
//==========================================================================
CJudge::SJudgeInfo CJudgeZoneManager::LoadCondition(std::string path)
{
	CJudge::SJudgeInfo info;
	info.type = CJudge::JUDGETYPE::TYPE_NONE;
	info.judgeParam = 
	{
		{CJudge::JUDGE::JUDGE_AAA,-1},
		{CJudge::JUDGE::JUDGE_BBB,-1},
		{CJudge::JUDGE::JUDGE_CCC,-1},
		{CJudge::JUDGE::JUDGE_DDD,-1},
	};

	// ファイルを開く
	std::ifstream File(path);
	if (File.is_open()) {
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

			if (line.find("CONDITIONSET") != std::string::npos)
			{// MODELSETで配置情報読み込み
				while (line.find("END_CONDITIONSET") == std::string::npos)
				{
					std::getline(File, line);
					if (line.find("TYPE") != std::string::npos)
					{// TYPEで配置物の種類
						int num = 0;
						
						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							num;		// 数値

						info.type = static_cast<CJudge::JUDGETYPE>(num);
					}
					if (line.find("JUDGE_AAA") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.judgeParam[CJudge::JUDGE::JUDGE_AAA];	// 数値
					}
					if (line.find("JUDGE_BBB") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.judgeParam[CJudge::JUDGE::JUDGE_BBB];	// 数値
					}
					if (line.find("JUDGE_CCC") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.judgeParam[CJudge::JUDGE::JUDGE_CCC];	// 数値
					}
					if (line.find("JUDGE_DDD") != std::string::npos)
					{// TYPEで配置物の種類

						// ストリーム作成
						std::istringstream lineStream(line);

						// 情報渡す
						lineStream >>
							hoge >>
							hoge >>		// ＝
							info.judgeParam[CJudge::JUDGE::JUDGE_DDD];	// 数値
					}
				}
			}

			if (line.find("END_SCRIPT") != std::string::npos)
			{
				break;
			}
		}

		// ファイルを閉じる
		File.close();
	}
	return info;
}
