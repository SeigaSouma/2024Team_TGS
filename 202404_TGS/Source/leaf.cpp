//=============================================================================
// 
//  葉っぱ処理 [leaf.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "leaf.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\key\\A.png";	// テクスチャのファイル
}

namespace StateTime	// 状態別時間
{
	const float FADEOUT = 0.3f;	// フェードアウト
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CLeaf::STATE_FUNC CLeaf::m_StateFunc[] =
{
	&CLeaf::StateFall,		// 落下
	&CLeaf::StateFadeOut,	// フェードアウト
};

//==========================================================================
// コンストラクタ
//==========================================================================
CLeaf::CLeaf(int nPriority, const LAYER layer) : CObject3D(nPriority, layer)
{
	// 値のクリア
	m_fStateTimer = 0.0f;			// 状態カウンター
	m_state = State::STATE_FALL;	// 状態
	m_rotDest = MyLib::Vector3();	// 目標の向き
	m_fRotateTimer = 0.0f;			// 回転までの時間
	m_fRotateInterval = 0.0f;		// 回転までの間隔
}

//==========================================================================
// デストラクタ
//==========================================================================
CLeaf::~CLeaf()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CLeaf* CLeaf::Create(const MyLib::Vector3& pos)
{
	// メモリの確保
	CLeaf* pObj = DEBUG_NEW CLeaf;

	if (pObj == nullptr)
	{// 失敗
		return nullptr;
	}

	// 引数情報設定
	pObj->SetPosition(pos);
	pObj->SetOriginPosition(pos);

	// 初期化処理
	pObj->Init();

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CLeaf::Init()
{

	// オブジェクト3Dの初期化
	CObject3D::Init();

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 縦幅を元にサイズ設定
	float randSize = UtilFunc::Transformation::Random(40, 60);
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, randSize);

	MyLib::Vector3 setsize = MyLib::Vector3(size.x, size.y, 0.0f);
	SetSize(setsize);
	SetSizeOrigin(setsize);

	// 向き
	MyLib::Vector3 rot;
	rot.y = UtilFunc::Transformation::Random(-31, 31) * 0.01f;
	rot.x = UtilFunc::Transformation::Random(-31, 31) * 0.01f;
	SetRotation(rot);
	SetOriginRotation(rot);

	// 目標の向き
	CalDestRotation();

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT3D);


	// 状態
	m_fRotateInterval = UtilFunc::Transformation::Random(100, 150) * 0.01f;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CLeaf::Uninit()
{
	// 終了処理
	CObject3D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CLeaf::Update()
{
	// 状態更新
	UpdateState();

	// 更新処理
	CObject3D::Update();
}

//==========================================================================
// 目標の向き計算
//==========================================================================
void CLeaf::CalDestRotation()
{
	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// 目標の向き
	m_rotDest.y = rot.y + UtilFunc::Transformation::Random(10, 31) * 0.01f;
	m_rotDest.x = rot.x + UtilFunc::Transformation::Random(10, 31) * 0.01f;
	UtilFunc::Transformation::RotNormalize(m_rotDest);
}

//==========================================================================
// 状態更新
//==========================================================================
void CLeaf::UpdateState()
{
	// 状態タイマー
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// 落下
//==========================================================================
void CLeaf::StateFall()
{
	MyLib::Vector3 pos = GetPosition(), posOrigin = GetOriginPosition();
	MyLib::Vector3 rot = GetRotation(), rotOrigin = GetOriginRotation();

	// 回転までの時間加算
	m_fRotateTimer += CManager::GetInstance()->GetDeltaTime();

	// 回転線形補間
	rot = UtilFunc::Correction::EasingEaseOut(rotOrigin, m_rotDest, 0.0f, m_fRotateInterval, m_fRotateTimer);
	UtilFunc::Transformation::RotNormalize(rot);
	SetRotation(rot);

	// 落下
	pos = UtilFunc::Correction::EasingEaseOut(posOrigin, posOrigin + MyLib::Vector3(0.0f, -50.0f, 0.0f), 0.0f, m_fRotateInterval, m_fRotateTimer);

	// 時間経過、新しい目標向き算出
	if (m_fRotateTimer >= m_fRotateInterval)
	{
		// 回転タイマーリセット
		m_fRotateTimer = 0.0f;
		m_fRotateInterval = UtilFunc::Transformation::Random(100, 150) * 0.01f;

		// 目標の向き
		CalDestRotation();
		SetOriginRotation(rot);

		// 元の位置
		SetOriginPosition(pos);
	}

	// 情報設定
	SetPosition(pos);

}

//==========================================================================
// フェードアウト
//==========================================================================
void CLeaf::StateFadeOut()
{
	if (m_fStateTimer >= StateTime::FADEOUT)
	{// 時間経過


	}
}

//==========================================================================
// 描画処理
//==========================================================================
void CLeaf::Draw()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 背面のカリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// 描画処理
	CObject3D::Draw();

	// カリングデフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}
