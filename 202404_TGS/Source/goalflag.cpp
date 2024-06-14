//=============================================================================
// 
//  ゴールフラグ処理 [goalflag.cpp]
//  Author : 日野澤匠泉のプログラム(お借りしてません)
// 
//=============================================================================
#include "goalflag.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"
#include "stagecleartext.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* MODEL = "data\\MODEL\\box.x";
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CGoalflagX::GOALFLAG_FUNC CGoalflagX::m_GoalflagFuncList[] =
{
	& CGoalflagX::SampleWho,	// フー
	& CGoalflagX::SampleWao,	// ワオ
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CGoalflagX> CGoalflagX::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CGoalflagX::CGoalflagX(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;	// 状態カウンター
	m_state = SAMPLE_WAO;	// 状態
}

//==========================================================================
// デストラクタ
//==========================================================================
CGoalflagX::~CGoalflagX()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CGoalflagX* CGoalflagX::Create(MyLib::Vector3 pos)
{
	// メモリの確保
	CGoalflagX* pObj = DEBUG_NEW CGoalflagX;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();

		// 位置情報せってうい
		pObj->SetPosition(pos);
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CGoalflagX::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	HRESULT hr = CObjectX::Init(MODEL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CGoalflagX::Uninit()
{

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CGoalflagX::Kill()
{

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CGoalflagX::Update()
{
	// 状態カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態別処理
	(this->*(m_GoalflagFuncList[m_state]))();

	// 位置情報取得
	MyLib::Vector3 pos = CObjectX::GetPosition();
	MyLib::Vector3 Playerpos;

	// リストループ
	CListManager<CPlayer> PlayerList = CPlayer::GetListObj();
	CPlayer* pObj = nullptr;
	while (PlayerList.ListLoop(&pObj))
	{
		// プレイヤーの位置情報取得
		Playerpos = pObj->GetPosition();
	}

	if (CGame::GetInstance()->GetGameManager()->GetType() != CGameManager::SceneType::SCENE_MAINCLEAR &&
		Playerpos.x >= pos.x)
	{// ゴールしたっぺ

		// 必要なゲームの状態設定してね
		CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SceneType::SCENE_MAINCLEAR);
		CStageClearText::Create(MyLib::Vector3(640.0f, 400.0f, 0.0f));
	}
}

//==========================================================================
// フー
//==========================================================================
void CGoalflagX::SampleWho()
{

}

//==========================================================================
// ワオ
//==========================================================================
void CGoalflagX::SampleWao()
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CGoalflagX::Draw()
{
	// 描画
	CObjectX::Draw();
}

