//=============================================================================
// 
//  タイマー処理(真ん中) [timer_result.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "timer_result.h"
#include "manager.h"
#include "multinumber.h"
#include "debugproc.h"
#include "calculation.h"
#include "game.h"
#include "gamemanager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano.png";	// テクスチャのファイル
	const std::string TEXT_TEXTURE = "data\\TEXTURE\\result\\cleartime.png";

	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(1100.0f, 100.0f, 0.0f);	// 初期位置
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(30.0f, 30.0f);
	const float DSTANCE_TIMER = SIZE_NUMBER.x * 2.25f;

	const float SIZE_HEIGHT = 50.0f;	// 縦幅のサイズ
	const float MOVEVALUE_TEXT = 3.0f;	//テキストの移動量
}

//==========================================================================
// コンストラクタ
//==========================================================================
CTimer_Result::CTimer_Result(int nPriority) : CTimer()
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CTimer_Result::~CTimer_Result()
{

}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTimer_Result::Init()
{
	//=============================
	// 文字生成
	//=============================
	CreateText();

	// 初期化
	CTimer::Init();
	m_pos = m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSizeOrigin().x * 2.0f + 200.0f, 0.0f, 0.0f);

	// タイマー
	ApplyTimer();

	return S_OK;
}


//==========================================================================
// 文字生成
//==========================================================================
void CTimer_Result::CreateText()
{
	// 生成
	m_pText = CObject2D::Create(6);
	m_pText->SetType(CObject::TYPE::TYPE_OBJECT2D);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXT_TEXTURE);
	m_pText->BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByHeight(size, SIZE_HEIGHT);
	m_pText->SetSize(size);
	m_pText->SetSizeOrigin(size);

	// 位置設定
	m_pText->SetPosition(MyLib::Vector3(200.0f, 350.0f, 0.0f));
	
	// アンカーポイントの設定
	m_pText->SetAnchorType(CObject2D::AnchorPoint::LEFT);
}

//==========================================================================
// 終了処理
//==========================================================================
void CTimer_Result::Uninit()
{
	CTimer::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CTimer_Result::Update()
{
	// 更新処理
	CTimer::Update();

}


//==========================================================================
// 描画処理
//==========================================================================
void CTimer_Result::Draw()
{

}
