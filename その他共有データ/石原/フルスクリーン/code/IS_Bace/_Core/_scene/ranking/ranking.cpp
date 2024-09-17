//======================================================
//
//ランキングシーン[ranking.cpp]
//Author:石原颯馬
//
//======================================================
#include "ranking.h"
#include "..\..\input\input.h"
#include "..\..\..\_Expansion\fade\fade.h"

//静的メンバ変数
const int CRanking::MAX_RANK = 5;

//=================================
//コンストラクタ
//=================================
CRanking::CRanking()
{
	m_pFade = nullptr;
}

//=================================
//デストラクタ
//=================================
CRanking::~CRanking()
{
}

//=================================
//初期化
//=================================
HRESULT CRanking::Init(void)
{
	//スコア値
	int* pRankScore = new int[MAX_RANK];	//動的確保
	Load(pRankScore);

	delete[] pRankScore;	//スコア破棄

	return S_OK;
}

//=================================
//終了
//=================================
void CRanking::Uninit(void)
{
	//オブジェ全破棄
	for (int cnt = 0; cnt < CObject::PRIORITY_FADE; cnt++)
	{
		CObject::ReleaseAll(cnt);
	}
}

//=================================
//更新
//=================================
void CRanking::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputGamePad* pGamepad = CManager::GetInstance()->GetInputGamePad();
	bool bPush = false;

	if (pGamepad != nullptr && pGamepad->IsConnect() == true)
	{//ゲームパッド接続
		if (pGamepad->GetTrigger(XINPUT_GAMEPAD_A) == true)
		{
			bPush = true;
		}
	}
	else
	{//未接続
		if (pKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			bPush = true;
		}
	}

	if(bPush == true && m_pFade == nullptr)
	{
		m_pFade = CFade::Create(CScene::MODE_TITLE);
	}
}

//=================================
//描画
//=================================
void CRanking::Draw(void)
{
}

//=================================
//ランキング設定
//=================================
void CRanking::Set(const int nScore)
{
	int* pRanking = new int[MAX_RANK];	//一応静的constも変数なので
													//ランキング読み込み
	Load(pRanking);

	//ソート
	if (pRanking[MAX_RANK - 1] < nScore)
	{//表示できる中で一番低い順位に入るか
		pRanking[MAX_RANK - 1] = nScore;	//とりあえず入れる

		for (int cnt = MAX_RANK - 1; cnt > 0; cnt--)
		{
			if (pRanking[cnt] > pRanking[cnt - 1])
			{//入れ替え
				int nTemp = pRanking[cnt - 1];
				pRanking[cnt - 1] = pRanking[cnt];
				pRanking[cnt] = nTemp;
			}
		}
	}

	//ランキング書き込み
	Save(pRanking);

	delete[] pRanking;	//破棄
}

//=================================
//ランキング読み込み
//=================================
void CRanking::Load(int* pRanking)
{
	FILE* pFile = nullptr;
	pFile = fopen("data\\ranking.bin", "rb");

	if (pFile != nullptr)
	{//ランキングファイルがある（読み込み）
		fread(&pRanking[0], sizeof(int), MAX_RANK, pFile);
		fclose(pFile);	//閉じる
	}
	else
	{//ランキングファイルがない（新規作成）
		for (int cnt = 0; cnt < MAX_RANK; cnt++)
		{
			pRanking[cnt] = 0;	//とりあえず0で埋める
		}
		Save(pRanking);
	}
}

//=================================
//ランキング書き込み
//=================================
void CRanking::Save(int* pRanking)
{
	FILE* pFile = nullptr;
	pFile = fopen("data\\ranking.bin", "wb");

	assert(("書き込みモードで開けなかったよ！奇跡だね！", pFile != nullptr));

	//開けた体で進める
	fwrite(pRanking, sizeof(int), MAX_RANK, pFile);	//書き込み

	fclose(pFile);	//閉じる
}
