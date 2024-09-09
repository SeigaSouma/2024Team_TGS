//=============================================================================
// 
// ランキング2D処理 [rankingItem.cpp]
//  Author : 澗口将太郎
// 
//=============================================================================
#include "rankingItem.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"
#include "gamemanager.h"
#include "game.h"
#include "game_pressenter.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	//ロゴ情報
	//const std::string TEXTURE_RESULT = "data\\TEXTURE\\result\\ninja_resultlogo.png";
	//const MyLib::Vector3 LOGO_POS = MyLib::Vector3(220.0f, 60.0f, 0.0f);
	//const D3DXVECTOR2 LOGO_SIZE = D3DXVECTOR2(200.0f, 50.0f);
	
	const MyLib::Vector3 ITEM_POS = MyLib::Vector3(250.0f,250.0f, 0.0f);
	const D3DXVECTOR2 ITEM_SIZE = D3DXVECTOR2(100.0f, 50.0f);
	const std::string TEXTURE_ITEM[CRankingItem::ITEM_MAX] = { "data\\TEXTURE\\result\\ranking_num00.png",
										"data\\TEXTURE\\result\\toatalrank.png",
										"data\\TEXTURE\\result\\num00.png",
										"data\\TEXTURE\\result\\rank00.png"};

	const D3DXVECTOR2 SPRITE_SIZE = D3DXVECTOR2(50.0f, 50.0f);
	const int MAX_SCORE_DEGIT = 8;

	const float TIME_SET = 0.2f;
}
//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
int CRankingItem::m_nObjNum = NULL;	//当オブジェクトの生成数

//==========================================================================
// コンストラクタ
//==========================================================================
CRankingItem::CRankingItem(int nPriority) : CObject(nPriority)
{
	m_fStateTime = 0.0f;
	
	for (int nCnt = 0; nCnt < ITEM_MAX; nCnt++)
	{
		m_pItem[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < MAX_SCORE_DEGIT; nCnt++)
	{
		m_pScoreItem[nCnt] = nullptr;
	}

	//当オブジェクトの生成数の初期化と加算処理
	if (m_nObjNum == NULL)
	{
		m_nObjNum = 1;
	}
	else
	{
		m_nObjNum++;
	}
	
}
//==========================================================================
// デストラクタ
//==========================================================================
CRankingItem::~CRankingItem()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CRankingItem* CRankingItem::Create(int nNumRank,int nScore, int nAllRank)
{
	// メモリの確保
	CRankingItem *pMarker =  new CRankingItem;

	if (pMarker != nullptr)
	{
		//順位を取得
		pMarker->m_nNumRank = nNumRank;

		//スコアを取得
		pMarker->m_nScore = nScore;

		//総評を取得
		pMarker->m_nAllRank = nAllRank;

		// 初期化処理
		pMarker->Init();
	}

	return pMarker;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CRankingItem::Init()
{
	//項目の初期化処理
	MyLib::Vector3 difItemPos = MyLib::Vector3(-610.0f,15.0f,0.0f);
	MyLib::Vector3 difRankPos = MyLib::Vector3(0.0f, 100.0f * (m_nObjNum - 1), 0.0f);
	D3DXVECTOR2 persize = D3DXVECTOR2(0.5f, 0.5f);

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (nCntItem < ITEM_SCORE)
		{//横長文字OBJの初期化

			//Obj2D生成
			m_pItem[nCntItem] = CObject2D::Create(GetPriority());

			//各項目の距離加算
			difItemPos += MyLib::Vector3(610.0f, -15.0f, 0.0f);

			m_pItem[nCntItem]->SetPosition(ITEM_POS + difItemPos + difRankPos);

			//テクスチャ割り当て
			int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
			m_pItem[nCntItem]->BindTexture(nIdxTex);

			//サイズ設定
			D3DXVECTOR2 size = ITEM_SIZE;

			if (nCntItem == 1)
			{//縮小処理
				size.x *= persize.x;
				size.y *= persize.y;
			}
			m_pItem[nCntItem]->SetSize(size);
			m_pItem[nCntItem]->SetSizeOrigin(size);

			//タイプ設定
			CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

			//スプライト調整
			if (nCntItem == 0)
			{//順位の調整
				D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(0.0f,m_nNumRank * 0.1f - 0.1f), D3DXVECTOR2(1.0f,m_nNumRank * 0.1f - 0.1f),  D3DXVECTOR2(0.0f,m_nNumRank * 0.1f),  D3DXVECTOR2(1.0f,m_nNumRank * 0.1f) };
				m_pItem[nCntItem]->SetTex(uvpos);
			}
		}
		else if (nCntItem == ITEM_SCORE)
		{//スコアの初期化
			difItemPos = MyLib::Vector3(150.0f,0.0f, 0.0f);
			int nDegitNum[MAX_SCORE_DEGIT+1];
			int nScore = m_nScore;

			//スコアのみ別管理のためnullptrを代入
			m_pItem[nCntItem] = nullptr;

			//スコアを桁ごとに分解
			for (int nDegitCnt = MAX_SCORE_DEGIT; nDegitCnt > 0; nDegitCnt--)
			{
				nDegitNum[nDegitCnt] = nScore /pow(10,nDegitCnt-1);
				nScore = nScore - pow(10, nDegitCnt - 1) * nDegitNum[nDegitCnt];
			}
			
			//各桁の初期化
			for (int nCnt=0, nDegitCnt = MAX_SCORE_DEGIT; nDegitCnt > 0; nDegitCnt--,nCnt++)
			{
				MyLib::Vector3 difDegitPos = MyLib::Vector3(50.0f * nCnt, 0.0f, 0.0f);
				m_pScoreItem[nDegitCnt] = CObject2D::Create(GetPriority());

				m_pScoreItem[nDegitCnt]->SetPosition(ITEM_POS + difItemPos + difDegitPos+ difRankPos);

				int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
				m_pScoreItem[nDegitCnt]->BindTexture(nIdxTex);

				D3DXVECTOR2 size = SPRITE_SIZE;

				m_pScoreItem[nDegitCnt]->SetSize(size);
				m_pScoreItem[nDegitCnt]->SetSizeOrigin(size);

				CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);

				
				D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f,0.0f), D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f + 0.1f,0.0f),  D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f ,1.0f),  D3DXVECTOR2(nDegitNum[nDegitCnt] * 0.1f + 0.1f,1.0f) };
				m_pScoreItem[nDegitCnt]->SetTex(uvpos);
				
			}
		}

		else if (nCntItem == ITEM_ALLRANK_DATA)
		{//総評ランクアイコンの初期化
			m_pItem[nCntItem] = CObject2D::Create(GetPriority());

			difItemPos += MyLib::Vector3(575.0f, 0.0f, 0.0f);

			m_pItem[nCntItem]->SetPosition(ITEM_POS + difItemPos + difRankPos);

			int nIdxTex = CTexture::GetInstance()->Regist(TEXTURE_ITEM[nCntItem]);
			m_pItem[nCntItem]->BindTexture(nIdxTex);

			D3DXVECTOR2 size = SPRITE_SIZE;

			m_pItem[nCntItem]->SetSize(size);
			m_pItem[nCntItem]->SetSizeOrigin(size);

			CObject::SetType(CObject::TYPE::TYPE_OBJECT2D);
			
			D3DXVECTOR2 uvpos[4] = { D3DXVECTOR2(0.0f,m_nAllRank * 0.25f), D3DXVECTOR2(1.0f,m_nAllRank * 0.25f),  D3DXVECTOR2(0.0f,m_nAllRank * 0.25f+0.25f),  D3DXVECTOR2(1.0f,m_nAllRank * 0.25f+0.25f) };
			m_pItem[nCntItem]->SetTex(uvpos);
		}
	}
	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CRankingItem::Uninit()
{

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem]!=nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}
	Release();
}

//==========================================================================
// 削除
//==========================================================================
void CRankingItem::Kill()
{

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Uninit();
			m_pItem[nCntItem] = nullptr;
		}
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CRankingItem::Update()
{
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//項目の更新
	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			D3DXVECTOR2 size = m_pItem[nCntItem]->GetSize();
			size.x = UtilFunc::Correction::EasingEaseInOut(ITEM_SIZE.x * 10.0f, ITEM_SIZE.x, 0.0f, TIME_SET, m_fStateTime);
			size.y = UtilFunc::Correction::EasingEaseInOut(ITEM_SIZE.y * 10.0f, ITEM_SIZE.y, 0.0f, TIME_SET, m_fStateTime);
			m_pItem[nCntItem]->SetSize(size);

			D3DXVECTOR2 sizetext = m_pItem[nCntItem]->GetSize();
			D3DXVECTOR2 sizetextOR = m_pItem[nCntItem]->GetSizeOrigin();
			sizetext.x = UtilFunc::Correction::EasingEaseInOut(sizetextOR.x * 10.0f, sizetextOR.x, 0.0f, TIME_SET, m_fStateTime);
			sizetext.y = UtilFunc::Correction::EasingEaseInOut(sizetextOR.y * 10.0f, sizetextOR.y, 0.0f, TIME_SET, m_fStateTime);
			m_pItem[nCntItem]->SetSize(sizetext);

			m_pItem[nCntItem]->Update();
		}
	}
}
//==========================================================================
// 描画処理
//==========================================================================
void CRankingItem::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// オブジェクト2Dの描画

	for (int nCntItem = 0; nCntItem < ITEM_MAX; nCntItem++)
	{
		if (m_pItem[nCntItem] != nullptr)
		{
			m_pItem[nCntItem]->Draw();
		}
	}

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}
