//=============================================================================
// 
//  ゴールフラグ処理 [goalflag.cpp]
//  Author : 日野澤匠泉のプログラム(お借りしてません)
// 
//=============================================================================
#include "checkpoint.h"
#include "manager.h"
#include "calculation.h"
#include "player.h"
#include "game.h"

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
CCheckpoint::CHECKPOINT_FUNC CCheckpoint::m_CheckpointFuncList [] =
{
	&CCheckpoint::SampleWho,	// フー
	&CCheckpoint::SampleWao,	// ワオ
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CCheckpoint> CCheckpoint::m_List = {};	// リスト
int CCheckpoint::m_nAll = 0;
int CCheckpoint::m_nSaveID = 0;

//==========================================================================
// コンストラクタ
//==========================================================================
CCheckpoint::CCheckpoint(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;	// 状態カウンター
	m_state = SAMPLE_WAO;	// 状態
}

//==========================================================================
// デストラクタ
//==========================================================================
CCheckpoint::~CCheckpoint()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CCheckpoint* CCheckpoint::Create(MyLib::Vector3 pos)
{
	// メモリの確保
	CCheckpoint* pObj = DEBUG_NEW CCheckpoint;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();

		// 位置情報せってうい
		pObj->SetPosition(pos);

		// 総数加算
		m_nAll++;
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCheckpoint::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// ID設定
	m_MyIndex = m_nAll;

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
void CCheckpoint::Uninit()
{
	// 総数減らす
	m_nAll--;

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CCheckpoint::Kill()
{

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CCheckpoint::Update()
{
	// 状態カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態別処理
	(this->*(m_CheckpointFuncList[m_state]))();

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

	if (Playerpos.x >= pos.x)
	{// チェックポイント通過したら

		m_nSaveID = m_MyIndex;
	}
}

//==========================================================================
// フー
//==========================================================================
void CCheckpoint::SampleWho()
{

}

//==========================================================================
// ワオ
//==========================================================================
void CCheckpoint::SampleWao()
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CCheckpoint::Draw()
{
	// 描画
	CObjectX::Draw();
}

