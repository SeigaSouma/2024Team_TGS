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

namespace StateTime
{
	const float WAIT = 0.5f;	// 待機
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CTimer_Result::STATE_FUNC CTimer_Result::m_StateFunc[] =
{
	&CTimer_Result::StateScrollText,	// 文字送り
	&CTimer_Result::StateSrollVoid,		// 空間送り
	&CTimer_Result::StateScrollTime,	// タイム送り
	&CTimer_Result::StateFinish,		// 終了
	&CTimer_Result::StateNone,			// なにもなし

};

//==========================================================================
// コンストラクタ
//==========================================================================
CTimer_Result::CTimer_Result(int nPriority) : CTimer(nPriority)
{
	// 値のクリア
	m_fStateTime = 0.0f;		// 状態カウンター
	m_state = State::STATE_SCROLL_TEXT;			// 状態
	m_fMoveTextLen = 0.0f;	// テキストの移動距離
	m_fMoveTimeLen = 0.0f;	// タイムの移動距離
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
	m_pos = m_pText->GetPosition() + MyLib::Vector3(m_pText->GetSizeOrigin().x * 2.0f + 150.0f, 0.0f, 0.0f);

	// アンカーポイントを左にする
	for (int i = 0; i < 3; i++)
	{
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			CObject2D* pObj2D = pNumber[j]->GetObject2D();

			pObj2D->SetAnchorType(CObject2D::AnchorPoint::LEFT);
		}

	}

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
// 状態更新
//==========================================================================
void CTimer_Result::UpdateState()
{
	// 状態タイマー
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// 文字送り
//==========================================================================
void CTimer_Result::StateScrollText()
{
	// サイズ取得
	D3DXVECTOR2 size = m_pText->GetSize(), sizeOrigin = m_pText->GetSizeOrigin();

	// テキスト移動距離加算
	m_fMoveTextLen += MOVEVALUE_TEXT;
	m_fMoveTextLen = UtilFunc::Transformation::Clamp(m_fMoveTextLen, 0.0f, sizeOrigin.x);

	if (m_fMoveTextLen >= sizeOrigin.x)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_VOID);
	}

	// サイズ設定
	size.x = m_fMoveTextLen;
	m_pText->SetSize(size);

	// テクスチャ座標設定
	D3DXVECTOR2* pTex = m_pText->GetTex();
	pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);
}

//==========================================================================
// 空間送り
//==========================================================================
void CTimer_Result::StateSrollVoid()
{
	if (m_fStateTime >= StateTime::WAIT)
	{
		// 状態遷移
		SetState(State::STATE_SCROLL_TIME);
	}
}

//==========================================================================
// タイム送り
//==========================================================================
void CTimer_Result::StateScrollTime()
{

	// アンカーポイントを左にする
	D3DXVECTOR2 size, sizeOrigin;

	for (int i = 0; i < 3; i++)
	{
		// ナンバー取得
		CMultiNumber* pMultiNumber = m_pClearTime[i];
		CNumber** pNumber = pMultiNumber->GetNumber();

		for (int j = 0; j < pMultiNumber->GetDigit(); j++)
		{
			// 数字の2Dオブジェクト取得
			CObject2D* pObj2D = pNumber[j]->GetObject2D();
			size = pObj2D->GetSize();


		}

	}


	//// サイズ取得
	//D3DXVECTOR2 size = GetSize(), sizeOrigin = GetSizeOrigin();

	//// テキスト移動距離加算
	//m_fMoveTimeLen += MOVEVALUE_TEXT;
	//m_fMoveTimeLen = UtilFunc::Transformation::Clamp(m_fMoveTimeLen, 0.0f, sizeOrigin.x);

	//if (m_fMoveTimeLen >= sizeOrigin.x)
	//{
	//	// 状態遷移
	//	SetState(State::STATE_FINISH);
	//}

	//// サイズ設定
	//size.x = m_fMoveTimeLen;
	//SetSize(size);

	//// テクスチャ座標設定
	//D3DXVECTOR2* pTex = GetTex();
	//pTex[1].x = pTex[3].x = (size.x / sizeOrigin.x);
}

//==========================================================================
// 終了
//==========================================================================
void CTimer_Result::StateFinish()
{
	//// サイズ設定
	//SetSize(GetSizeOrigin());
	//m_pText->SetSize(m_pText->GetSizeOrigin());

	//// テクスチャ座標設定
	//D3DXVECTOR2* pTex = GetTex();
	//D3DXVECTOR2* pTexText = m_pText->GetTex();
	//pTex[1].x = pTex[3].x = pTexText[1].x = pTexText[3].x = 1.0f;

	//// 状態遷移
	//SetState(State::STATE_NONE);
}

//==========================================================================
// 状態設定
//==========================================================================
void CTimer_Result::SetState(State state)
{
	m_state = state;
	m_fStateTime = 0.0f;
}

//==========================================================================
// 描画処理
//==========================================================================
void CTimer_Result::Draw()
{

}
