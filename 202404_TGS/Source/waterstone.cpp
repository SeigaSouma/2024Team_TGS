//=============================================================================
// 
//  水中石処理 [waterstone.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "waterstone.h"
#include "manager.h"
#include "calculation.h"
#include "3D_effect.h"

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
	const float DEFAULT_SPLASHTIME = 0.8f;	// 通常のしぶき時間
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

		MyLib::Vector3 move;
		MyLib::Vector3 posOrigin = GetPosition();
		MyLib::Vector3 pos;

		for (int i = 0; i < 10; i++)
		{
			float randmoveX = UtilFunc::Transformation::Random(-150, 150) * 0.01f;
			float randmoveY = UtilFunc::Transformation::Random(10, 40) * 0.1f;
			float randRadius = UtilFunc::Transformation::Random(-30, 30) * 0.1f;

			move.x = randmoveX;
			move.z = randmoveX;
			move.y = randmoveY;

			float radius = 40.0f + randRadius;

			int xz = UtilFunc::Transformation::Random(-50, 50);
			pos = posOrigin + MyLib::Vector3(xz, 0.0f, xz);


			float rand = UtilFunc::Transformation::Random(-50, 50) * 0.01f;
			CEffect3D* pEffect = CEffect3D::Create(
				pos,
				move,
				D3DXCOLOR(0.5f + rand, 0.5f + rand, 1.0f, 1.0f),
				radius, 50,
				CEffect3D::MOVEEFFECT::MOVEEFFECT_ADD,
				CEffect3D::TYPE::TYPE_SMOKE);
			pEffect->SetEnableGravity();
			pEffect->SetGravityValue(0.1f);
		}

		// インターバル更新
		m_fIntervalSplash = DEFAULT_SPLASHTIME + UtilFunc::Transformation::Random(-60, 60) * 0.01f;

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

