//=============================================================================
// 
//  プレイヤー処理 [people.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "people.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "player.h"
#include "camera.h"
#include "3D_Effect.h"
#include "shadow.h"
#include "sound.h"
#include "stage.h"
#include "objectX.h"
#include "limitarea.h"
#include "debugproc.h"


//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const float PLAYER_SERCH = 800.0f;	// プレイヤー探索範囲
	const float CHACE_DISTABCE = 50.0f;	// 追い掛ける時の間隔
}

namespace STATE_TIME
{
	const float DMG = static_cast<float>(10) / static_cast<float>(mylib_const::DEFAULT_FPS);	// ダメージ時間
	const float DEAD = static_cast<float>(40) / static_cast<float>(mylib_const::DEFAULT_FPS);	// 死亡時間
	const float DOWN = static_cast<float>(150) / static_cast<float>(mylib_const::DEFAULT_FPS);	// ダウン時間
	const float FADEOUT = 0.6f;	// 死亡時間
}

//==========================================================================
// 関数ポインタ
//==========================================================================
// 状態関数
CPeople::STATE_FUNC CPeople::m_StateFunc[] =
{
	&CPeople::StateNone,		// なし
	&CPeople::StateFadeOut,	// フェードアウト
};

//==========================================================================
// 静的メンバ変数宣言
//==========================================================================
CListManager<CPeople> CPeople::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CPeople::CPeople(int nPriority) : CObjectChara(nPriority)
{
	// 値のクリア
	m_type = TYPE::TYPE_NORMAL;	// 種類
	m_state = STATE::STATE_NONE;	// 状態
	m_Oldstate = m_state;	// 前回の状態
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// マテリアルの色
	m_TargetPosition = mylib_const::DEFAULT_VECTOR3;	// 目標の位置

	
	m_fStateTime = 0.0f;		// 状態遷移カウンター
	m_sMotionFrag = SMotionFrag();		// モーションのフラグ
	m_pShadow = nullptr;
}

//==========================================================================
// デストラクタ
//==========================================================================
CPeople::~CPeople()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CPeople* CPeople::Create(const char* pFileName, MyLib::Vector3 pos, TYPE type)
{
	// 生成用のオブジェクト
	CPeople* pPeople = nullptr;

	// メモリの確保
	switch (type)
	{
	case TYPE_NORMAL:
		//pPeople = DEBUG_NEW CPeople;
		break;

	case TYPE_HORSE:
		//pPeople = DEBUG_NEW CPeople;
		break;

	default:
		return nullptr;
		break;
	}

	if (pPeople != nullptr)
	{// メモリの確保が出来ていたら

		// 種類
		pPeople->m_type = type;

		// 位置設定
		pPeople->SetPosition(pos);
		pPeople->CObject::SetOriginPosition(pos);

		// テキスト読み込み
		HRESULT hr = pPeople->RoadText(pFileName);
		if (FAILED(hr))
		{// 失敗していたら
			return nullptr;
		}

		// 初期化処理
		pPeople->Init();
	}

	return pPeople;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CPeople::Init()
{
	// 各種変数の初期化
	m_state = STATE_NONE;	// 状態
	m_Oldstate = STATE_NONE;
	m_fStateTime = 0.0f;			// 状態遷移カウンター

	// 種類の設定
	SetType(CObject::TYPE::TYPE_ENEMY);

	// 向き設定
	SetRotation(MyLib::Vector3(0.0f, 0.0f, 0.0f));

	// リストに追加
	m_List.Regist(this);

	// デフォルト設定
	CMotion* pMotion = GetMotion();
	if (pMotion != nullptr)
	{
		pMotion->Set(MOTION_DEF);
	}

	return S_OK;
}

//==========================================================================
// テキスト読み込み
//==========================================================================
HRESULT CPeople::RoadText(const char *pFileName)
{
	// キャラ作成
	HRESULT hr = SetCharacter(pFileName);
	if (FAILED(hr))
	{// 失敗していたら
		return E_FAIL;
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CPeople::Uninit()
{
	
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow = nullptr;
	}

	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectChara::Uninit();
}

//==========================================================================
// 死亡処理
//==========================================================================
void CPeople::Kill()
{
	
	// 影を消す
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		m_pShadow = nullptr;
	}
}

//==========================================================================
// 更新処理
//==========================================================================
void CPeople::Update()
{
	// 死亡の判定
	if (IsDeath() == true)
	{// 死亡フラグが立っていたら
		return;
	}

	// 過去の位置設定
	SetOldPosition(GetPosition());

	// 親の処理
	CObjectChara::Update();

	// 当たり判定
	Collision();

	// 死亡の判定
	if (IsDeath())
	{// 死亡フラグが立っていたら
		return;
	}

	// モーションの設定処理
	MotionSet();

	// 状態更新
	UpdateState();

	// 死亡の判定
	if (IsDeath())
	{// 死亡フラグが立っていたら
		return;
	}

	
	// 影の位置更新
	if (m_pShadow != nullptr)
	{
		// 位置取得
		MyLib::Vector3 pos = GetPosition();
		m_pShadow->SetPosition(MyLib::Vector3(pos.x, m_pShadow->GetPosition().y, pos.z));
	}

	if (GetPosition().y <= mylib_const::KILL_Y)
	{
		// 敵の終了処理
		Kill();
		Uninit();
		return;
	}

	// 大人の壁
	LimitArea();
}

//==========================================================================
// 当たり判定
//==========================================================================
void CPeople::Collision()
{
	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// 向き取得
	MyLib::Vector3 rot = GetRotation();

	// 重力処理
	move.y -= mylib_const::GRAVITY;

	// 位置更新
	pos += move;

	// 慣性補正
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	if (move.x >= 0.1f || move.x <= -0.1f ||
		move.z >= 0.1f || move.z <= -0.1f)
	{// 移動中
		m_sMotionFrag.bMove = true;
	}
	else
	{
		m_sMotionFrag.bMove = false;
	}

	// 高さ取得
	bool bLand = false;
	float fHeight = CGame::GetInstance()->GetElevation()->GetHeight(pos, &bLand);

	if (fHeight > pos.y)
	{// 地面の方が自分より高かったら

		// 地面の高さに補正
		pos.y = fHeight;

		if (bLand == true)
		{// 着地してたら

			// 着地時処理
			ProcessLanding();

			// ジャンプ使用可能にする
			move.y = 0.0f;
			m_sMotionFrag.bJump = false;
		}
	}


	if (pos.y <= -800.0f)
	{
		pos.y = -800.0f;
	}

	// 位置設定
	SetPosition(pos);

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// 着地時の処理
//==========================================================================
void CPeople::ProcessLanding()
{
	// 移動量取得
	MyLib::Vector3 move = GetMove();

	// ジャンプ使用可能にする
	move.y = 0.0f;
	m_sMotionFrag.bJump = false;

	// 移動量設定
	SetMove(move);
}

//==========================================================================
// 状態更新処理
//==========================================================================
void CPeople::UpdateState()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_mMatcol.a);

	// 状態遷移カウンター加算
	m_fStateTime += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();
}

//==========================================================================
// 何もない状態
//==========================================================================
void CPeople::StateNone()
{
	// 色設定
	m_mMatcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fStateTime = 0.0f;
}

//==========================================================================
// フェードアウト
//==========================================================================
void CPeople::StateFadeOut()
{
	// 色設定
	m_mMatcol.a = 1.0f - m_fStateTime / STATE_TIME::FADEOUT;

	if (m_fStateTime >= STATE_TIME::FADEOUT)
	{
		// 終了処理
		Kill();
		Uninit();
		return;
	}



}


//==========================================================================
// 大人の壁
//==========================================================================
void CPeople::LimitArea()
{
	// 自身の値を取得
	MyLib::Vector3 pos = GetPosition();

	// 大人の壁取得
	CListManager<CLimitArea> limitareaList = CLimitArea::GetListObj();
	CLimitArea* pLimitArea = nullptr;

	while (limitareaList.ListLoop(&pLimitArea))
	{
		CLimitArea::sLimitEreaInfo info = pLimitArea->GetLimitEreaInfo();

		// 大人の壁を適用
		if (pos.x + GetRadius() >= info.fMaxX) { pos.x = info.fMaxX - GetRadius(); }
		if (pos.x - GetRadius() <= info.fMinX) { pos.x = info.fMinX + GetRadius(); }
		if (pos.z + GetRadius() >= info.fMaxZ) { pos.z = info.fMaxZ - GetRadius(); }
		if (pos.z - GetRadius() <= info.fMinZ) { pos.z = info.fMinZ + GetRadius(); }
	}

	// 値を適用
	SetPosition(pos);

}

//==========================================================================
// モーションの設定
//==========================================================================
void CPeople::SetMotion(int motionIdx)
{
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}
	pMotion->Set(motionIdx);
}

//==========================================================================
// 攻撃時処理
//==========================================================================
void CPeople::AttackAction(CMotion::AttackInfo ATKInfo, int nCntATK)
{
	return;
}

//==========================================================================
// 攻撃判定中処理
//==========================================================================
void CPeople::AttackInDicision(CMotion::AttackInfo* pATKInfo, int nCntATK)
{
	//return;
	// モーション取得
	CMotion* pMotion = GetMotion();
	if (pMotion == nullptr)
	{
		return;
	}

	// 位置取得
	MyLib::Vector3 pos = GetPosition();

	// 武器の位置
	MyLib::Vector3 weponpos = pMotion->GetAttackPosition(GetModel(), *pATKInfo);

	if (pATKInfo->fRangeSize == 0.0f)
	{
		return;
	}

	if (pATKInfo->bEndAtk)
	{
		return;
	}

#if _DEBUG
	CEffect3D::Create(weponpos, MyLib::Vector3(0.0f, 0.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), pATKInfo->fRangeSize, 10, CEffect3D::MOVEEFFECT_NONE, CEffect3D::TYPE_NORMAL);
#endif

	

}

//==========================================================================
// 描画処理
//==========================================================================
void CPeople::Draw()
{
	if (m_state == STATE_FADEOUT)
	{
		CObjectChara::Draw(m_mMatcol.a);
	}
	else if (m_mMatcol != D3DXCOLOR(1.0f, 1.0f, 1.0f, m_mMatcol.a))
	{
		// オブジェクトキャラの描画
		CObjectChara::Draw(m_mMatcol);
	}
	else
	{
		// オブジェクトキャラの描画
		CObjectChara::Draw();
	}
}

//==========================================================================
// 状態設定
//==========================================================================
void CPeople::SetState(STATE state)
{
	m_state = state;
}

//==========================================================================
// 敵の情報取得
//==========================================================================
CPeople *CPeople::GetPeople()
{
	// 自分自身のポインタを取得
	return this;
}

