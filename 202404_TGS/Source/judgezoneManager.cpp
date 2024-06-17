//=============================================================================
// 
//  判定ゾーンマネージャ [judgezoneManager.cpp]
//  Author : 石原颯馬
// 
//============================================================================
#include "judgezoneManager.h"
#include "judgezone.h"
#include "judge.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	
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
				(*itr)->Uninit();
			}
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
	CJudgeZone* pZone = nullptr;
	while (m_zoneList.ListLoop(&pZone))
	{
		if (!pZone->IsEnable())
		{// 無効のゾーンのみ破棄
			m_zoneList.Delete(pZone);	// リスト除外
			pZone->Uninit();			// 終了
			delete pZone;				// 破棄
		}
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
