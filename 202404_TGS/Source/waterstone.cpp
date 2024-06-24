//=============================================================================
// 
//  水中石処理 [waterstone.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "waterstone.h"
#include "manager.h"
#include "calculation.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string MODEL[] =
	{
		"data\\MODEL\\map_object\\rock_01_small.x",
		"data\\MODEL\\map_object\\rock_02.x",
		"data\\MODEL\\map_object\\rock_03.x",
		"data\\MODEL\\map_object\\rock_04.x",
		"data\\MODEL\\map_object\\rock_05.x",
	};
	const float DEFAULT_SPLASHTIME = 1.5f;	// 通常のしぶき時間
}

//==========================================================================
// コンストラクタ
//==========================================================================
CWaterStone::CWaterStone(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_fSplashTimer = 0.0f;		// しぶきタイマー
	m_fIntervalSplash = 0.0f;	// しぶきまでのインターバル
}

//==========================================================================
// デストラクタ
//==========================================================================
CWaterStone::~CWaterStone()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CWaterStone *CWaterStone::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CWaterStone* pObj = DEBUG_NEW CWaterStone;

	if (pObj != nullptr)
	{
		pObj->SetPosition(pos);

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CWaterStone::Init()
{
	
	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	int idx = (sizeof(MODEL) / sizeof(*MODEL)) - 1;
	idx = UtilFunc::Transformation::Random(0, idx);

	HRESULT hr = CObjectX::Init(MODEL[idx]);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CWaterStone::Uninit()
{
	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CWaterStone::Kill()
{
	// 終了処理
	CObjectX::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CWaterStone::Update()
{
	// しぶきタイマー加算
	m_fSplashTimer += CManager::GetInstance()->GetDeltaTime();

	// スプラッシュ！
	if (m_fIntervalSplash <= m_fSplashTimer)
	{


		// インターバル更新
		m_fIntervalSplash = DEFAULT_SPLASHTIME + UtilFunc::Transformation::Random(-30, 30) * 0.01f;

		// しぶきタイマー
		m_fSplashTimer = 0.0f;
	}

}

//==========================================================================
// 描画処理
//==========================================================================
void CWaterStone::Draw()
{
	// 描画
	CObjectX::Draw();
}

