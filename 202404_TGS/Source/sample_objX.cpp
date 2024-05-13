//=============================================================================
// 
//  転移ビーコン処理 [transferBeacon.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "sample_objX.h"
#include "manager.h"
#include "calculation.h"

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
CSample_ObjX::SAMPLE_FUNC CSample_ObjX::m_SampleFuncList[] =
{
	&CSample_ObjX::SampleWho,	// フー
	&CSample_ObjX::SampleWao,	// ワオ
};

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CSample_ObjX> CSample_ObjX::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CSample_ObjX::CSample_ObjX(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;	// 状態カウンター
	m_state = SAMPLE_WAO;	// 状態
}

//==========================================================================
// デストラクタ
//==========================================================================
CSample_ObjX::~CSample_ObjX()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CSample_ObjX *CSample_ObjX::Create()
{
	// メモリの確保
	CSample_ObjX* pObj = DEBUG_NEW CSample_ObjX;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSample_ObjX::Init()
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
void CSample_ObjX::Uninit()
{
	
	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CSample_ObjX::Kill()
{
	
	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSample_ObjX::Update()
{
	// 状態カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態別処理
	(this->*(m_SampleFuncList[m_state]))();

}

//==========================================================================
// フー
//==========================================================================
void CSample_ObjX::SampleWho()
{
	
}

//==========================================================================
// ワオ
//==========================================================================
void CSample_ObjX::SampleWao()
{

}

//==========================================================================
// 描画処理
//==========================================================================
void CSample_ObjX::Draw()
{
	// 描画
	CObjectX::Draw();
}

