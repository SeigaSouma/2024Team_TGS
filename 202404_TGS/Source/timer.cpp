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

// 派生先
#include "timer_result.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const char* TEXTURE = "data\\TEXTURE\\number\\number_oradano.png";	// テクスチャのファイル

	const MyLib::Vector3 DEFAULT_POSITION = MyLib::Vector3(1100.0f, 100.0f, 0.0f);	// 初期位置
	const D3DXVECTOR2 SIZE_NUMBER = D3DXVECTOR2(30.0f, 30.0f);
	const float DSTANCE_TIMER = SIZE_NUMBER.x * 2.25f;
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
	&CTimer::StateWait,		// 待機
	&CTimer::StateGoal,		// ゴール
};

//==========================================================================
// コンストラクタ
//==========================================================================
CTimer::CTimer(int nPriority)
{
	// 値のクリア
	m_nPriority = nPriority;
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
CTimer* CTimer::Create(Type type, int nPriority)
{
	if (m_pTimer != nullptr) return m_pTimer;

	// メモリの確保

	switch (type)
	{
	case TYPE_NORMAL:
		m_pTimer = DEBUG_NEW CTimer;
		break;

	case Type::TYPE_RESULT:
		m_pTimer = DEBUG_NEW CTimer_Result(nPriority);
		break;
	}

	if (m_pTimer != nullptr)
	{
		// 初期化処理
		m_pTimer->Init();
	}

	return m_pTimer;
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
	m_bAddTime = false;			// タイマー加算のフラグ


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
			TEXTURE, false, m_nPriority);
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
		pNumber->SetType(CObject::TYPE::TYPE_NUMBER);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CTimer::Uninit()
{
	delete m_pTimer;
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
	ApplyTimer();
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
// ゴール状態
//==========================================================================
void CTimer::StateGoal()
{
	m_bAddTime = false;
}

//==========================================================================
// タイマー反映
//==========================================================================
void CTimer::ApplyTimer()
{
	// タイマーを分、秒、ミリ秒に変換
	int time[3];
	time[2] = static_cast<int>(m_fTime / 60);
	time[1] = static_cast<int>(m_fTime) % 60;
	time[0] = static_cast<int>((m_fTime - static_cast<int>(m_fTime)) * 1000);
	time[0] /= 10;

	for (int i = 0; i < 3; i++)
	{
		if (m_pClearTime[i] == nullptr) {
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
// 描画処理
//==========================================================================
void CTimer::Draw()
{

}

//==========================================================================
// タイマー設定と反映
//==========================================================================
void CTimer::SetTime(const float time)
{
	m_fTime = time;
	ApplyTimer();
}
