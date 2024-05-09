//=============================================================================
// 
//  タイマー処理 [timer.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "timer.h"
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

	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(880.0f, 237.0f, 0.0f);	// 初期位置
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(50.0f, 50.0f);
	const float DSTANCE_TIMER = SIZE_NUMBER.x * 2;
}

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CTimer *CTimer::m_pTimer = nullptr;	// 自身のポインタ

//==========================================================================
// 関数リスト
//==========================================================================
CTimer::STATE_FUNC CTimer::m_StateFuncList[] =
{
	&CTimer::StateWait,			// 待機
	&CTimer::StatAppearance,	// 出現
	&CTimer::StatAddLittle,		// 少し加算
	&CTimer::StateAdjustment,	// 調整
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTimer::CTimer(int nPriority)
{
	// 値のクリア
	m_state = STATE_WAIT;		// 状態
	m_fStateTime = 0.0f;		// 状態時間
	m_fTime = 0.0f;				// 時間
	m_pos = mylib_const::DEFAULT_VECTOR3;	// 位置
	m_posOrigin = mylib_const::DEFAULT_VECTOR3;	// 元の位置
	m_bAddTime = false;			// タイマー加算のフラグ
}

//==========================================================================
// デストラクタ
//==========================================================================
CTimer::~CTimer()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CTimer *CTimer::Create()
{
	// 生成用のオブジェクト
	CTimer *pScore = nullptr;

	if (pScore == nullptr)
	{// nullptrだったら

		// メモリの確保
		pScore = DEBUG_NEW CTimer;

		if (pScore != nullptr)
		{// メモリの確保が出来ていたら

			// 初期化処理
			pScore->Init();
		}

		return pScore;
	}

	return nullptr;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CTimer::Init()
{
	// 各種変数初期化
	m_pos = DEFAULT_POSITION;
	m_posOrigin = m_pos;	// 元の位置
	m_fTime = 0.0f;	// 時間
	m_state = STATE_WAIT;			// 状態
	m_bAddTime = true;			// タイマー加算のフラグ


	// 分、秒、ミリ秒の計算
	int time[3];
	time[0] = static_cast<int>(m_fTime) / (60);
	time[1] = static_cast<int>(m_fTime) % (60);
	time[2] = static_cast<int>(m_fTime) % 1000;

	for (int i = 0; i < 3; i++)
	{
		// 生成
		m_pClearTime[i] = CMultiNumber::Create(
			m_pos,
			SIZE_NUMBER,
			2,
			CNumber::EObjectType::OBJECTTYPE_2D,
			TEXTURE, false, 3);
		if (m_pClearTime[i] == nullptr){
			continue;
		}
		CMultiNumber* pNumber = m_pClearTime[i];

		// 位置設定
		MyLib::Vector3 pos = m_pos;
		pos.x -= DSTANCE_TIMER * i;
		pNumber->SetPosition(pos);

		// 右寄せに設定
		pNumber->SetAlignmentType(CMultiNumber::AlignmentType::ALIGNMENT_RIGHT);

		// 値の設定
		pNumber->SetValue(time[i]);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTimer::Uninit()
{
	m_pTimer = nullptr;
}

//==========================================================================
// 更新処理
//==========================================================================
void CTimer::Update()
{
	// 状態別処理
	(this->*(m_StateFuncList[m_state]))();

	if (!m_bAddTime)
	{
		return;
	}

	// タイマー加算
	m_fTime += CManager::GetInstance()->GetDeltaTime();

	// タイマーを分、秒、ミリ秒に変換
	int time[3];
	time[2] = static_cast<int>(m_fTime / 60);
	time[1] = static_cast<int>(m_fTime) % 60;
	time[0] = static_cast<int>((m_fTime - static_cast<int>(m_fTime)) * 1000);
	time[0] /= 10;

	for (int i = 0; i < 3; i++)
	{
		if (m_pClearTime[i] == nullptr){
			continue;
		}

		// 値の設定
		m_pClearTime[i]->SetValue(time[i]);

		// 位置設定
		MyLib::Vector3 pos = m_pos;
		pos.x -= DSTANCE_TIMER * i;
		m_pClearTime[i]->SetPosition(pos);
	}

}

//==========================================================================
// 待機状態
//==========================================================================
void CTimer::StateWait()
{
	// 時間リセット
	m_fStateTime = 0.0f;
}

//==========================================================================
// 出現状態
//==========================================================================
void CTimer::StatAppearance()
{
	// 時間加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//if (m_fStateTime >= TIME_APPEARANCE)
	//{
	//	// 状態遷移
	//	m_state = STATE_ADD_LITTLE;
	//	m_fStateTime = 0.0f;

	//	// タイマーを進める
	//	m_bAddTime = true;
	//}
}

//==========================================================================
// 少し加算状態
//==========================================================================
void CTimer::StatAddLittle()
{
	// 時間加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	//if (m_fStateTime >= TIME_ADDLITTLE)
	//{
	//	// 状態遷移
	//	m_state = STATE_ADJ;
	//	m_fStateTime = 0.0f;
	//	CGame::GetInstance()->GetGameManager()->SetType(CGameManager::SCENE_MAIN);
	//}
}

//==========================================================================
// 調整状態
//==========================================================================
void CTimer::StateAdjustment()
{
	// 時間加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	/*m_pos.x = UtilFunc::Correction::EasingLinear(m_posOrigin.x, DEST_POSITION.x, m_fStateTime / TIME_ADJUSTMENT);
	m_pos.y = UtilFunc::Correction::EasingLinear(m_posOrigin.y, DEST_POSITION.y, m_fStateTime / TIME_ADJUSTMENT);*/

	MyLib::Vector3 setpos = m_pos;

	//if (m_fStateTime >= TIME_ADJUSTMENT)
	//{
	//	// 状態遷移
	//	m_state = STATE_WAIT;
	//	m_fStateTime = 0.0f;
	//}
}

//==========================================================================
// 描画処理
//==========================================================================
void CTimer::Draw()
{

}
