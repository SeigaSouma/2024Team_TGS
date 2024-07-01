//=============================================================================
// 
//  荷物処理 [baggage.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "baggage.h"
#include "manager.h"
#include "calculation.h"
#include "game.h"
#include "camera.h"
#include "map_obstacle.h"
#include "renderer.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string MODEL[] =
	{
		"data\\MODEL\\map_object\\rock_02.x",	// 布
	};
	const float LIMIT_HEIGHT = 800.0f;	// 高さ上限
	const float VELOCITY = 1.0f;
	float PITCH_RATIO = 0.00025f;	// ピッチ軸回転倍率
	float PITCH_INER = 0.075f;	// ピッチ軸回転慣性
	float ROLL_FSTSPD = 0.70f;	// ロール軸回転初速
	float ROLL_INER = 0.075f;		// ロール軸回転慣性
	float DEVIATION_WIDTH = 300.0f;	// ぶれ幅
	float DEVIATION_SPEED = 0.02f * D3DX_PI;	// ぶれ速度
	float DEADANGLE_HIT = D3DX_PI * 0.2f;		// ヒット時の死亡判定
}

namespace StateTime
{
	const float DAMAGE = 0.2f;	// ダメージ
	const float DEAD = 0.7f;	// 死亡
}

//==========================================================================
// 関数ポインタ
//==========================================================================
CBaggage::STATE_FUNC CBaggage::m_StateFunc[] =
{
	&CBaggage::StateNone,	// なし
	&CBaggage::StateDamage,	// ダメージ
	&CBaggage::StateDead,	// 死亡
};

//==========================================================================
// ブラー表現(マルチターゲットレンダリング)用定数定義
//==========================================================================
namespace MULTITARGET
{
	// 開始時
	const float START_ALPHA = (0.7f);		// 目標透明度
	const float START_MULTI = (1.0f);		// 目標倍率
	const float START_TIMER = (0.0f);		// 遷移タイマー
}

//==========================================================================
// 静的メンバ変数
//==========================================================================
CListManager<CBaggage> CBaggage::m_List = {};	// リスト

//==========================================================================
// コンストラクタ
//==========================================================================
CBaggage::CBaggage(int nPriority) : CObjectQuaternion(nPriority)
{
	// 値のクリア
	m_type = TYPE::TYPE_CLOTH;		// 種類
	m_state = STATE::STATE_NONE;	// 状態
	m_fStateTimer = 0.0f;			// 状態タイマー
	m_fWeight = 0.0f;	// 重さ
	m_bLand = false;	// 着地判定
	m_bEnd = false;		// 終了処理
	m_velorot = MyLib::Vector3(0.0f, 0.0f, 0.0f);
	m_baggageInfo = {};
	m_fDeviation = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
CBaggage::~CBaggage()
{
	
}

//==========================================================================
// 生成処理
//==========================================================================
CBaggage *CBaggage::Create(TYPE type, SBaggageInfo info)
{
	// メモリの確保
	CBaggage* pObj = DEBUG_NEW CBaggage;

	if (pObj != nullptr)
	{
		// 引数情報
		pObj->m_type = type;
		pObj->m_baggageInfo = info;

		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CBaggage::Init()
{
	// リストに追加
	m_List.Regist(this);

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 初期化処理
	HRESULT hr = CObjectQuaternion::Init(m_baggageInfo.path);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	// パラメータ設定
	m_fWeight = 1.8f;

	CreateCollisionBox();
	SetState(CObjectX::STATE::STATE_EDIT);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CBaggage::Uninit()
{
	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectQuaternion::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CBaggage::Kill()
{
	// リストから削除
	m_List.Delete(this);

	// 終了処理
	CObjectQuaternion::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CBaggage::Update()
{
	if (CGame::GetInstance()->GetGameManager()->GetType() == CGameManager::SceneType::SCENE_WAIT_AIRPUSH) {
		return;
	}

	// 親クラスの更新処理
	CObjectQuaternion::Update();


	// 状態関数
	UpdateState();


	// 吹っ飛んだら終わり
	if (m_state == STATE::STATE_DEAD)
	{
		return;
	}

	// 情報取得
	MyLib::Vector3 posOrigin = GetOriginPosition();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
#ifdef _DEBUG
	ImGui::DragFloat("weight", &m_fWeight, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("PITCH_RATIO", &PITCH_RATIO, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("PITCH_INER", &PITCH_INER, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("ROLL_FSTSPD", &ROLL_FSTSPD, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("ROLL_INER", &ROLL_INER, 0.1f, 0.0f, 0.0f, "%.2f");
#endif

	// 位置更新
	pos += move;
	pos += m_force;

	// 回転(ピッチ軸
	if (move.y > 0.0f) m_velorot.y += fabsf(move.y) * 0.00025f;
	else m_velorot.y += (0.0f - m_velorot.y) * PITCH_INER;

	m_velorot.x += (0.0f - m_velorot.x) * ROLL_INER;

	// クォータニオン割り当て
	BindQuaternion(MyLib::Vector3(0.0f, 1.0f, 0.0f), m_velorot.y);

	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 vec = MyLib::Vector3(sinf(rot.y), 0.0f, cosf(rot.y));
	BindQuaternion(MyLib::Vector3(vec.z, 0.0f, -vec.x), m_velorot.x);

	// 重力加算
	move.y -= mylib_const::GRAVITY * m_fWeight;

	static float limitMoveY = 30.0f;
#ifdef _DEBUG
	ImGui::DragFloat("Limit MoveY", &limitMoveY, 1.0f, 0.0f, 0.0f, "%.2f");
#endif

	if (move.y >= limitMoveY)
	{
		move.y = limitMoveY;
	}

	// 筒の位置判定
	if (pos.y <= GetOriginPosition().y)
	{
		pos.y = GetOriginPosition().y;
		move.y = 0.0f;
		m_bLand = true;
	}
	else
	{
		m_bLand = false;
	}

	// 慣性補正
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;

	// ぶれ設定
	m_fDeviation += m_baggageInfo.cycle;
	UtilFunc::Transformation::RotNormalize(m_fDeviation);
	pos.x += sinf(m_fDeviation) * m_baggageInfo.width * ((pos.y - GetOriginPosition().y) / 1000.0f);

	// 情報設定
	SetPosition(pos);
	SetMove(move);

#ifdef _DEBUG	// デバッグ時変形
	DebugTransform();
#endif
}

//==========================================================================
// 状態更新
//==========================================================================
void CBaggage::UpdateState()
{

	// 状態タイマー
	m_fStateTimer += CManager::GetInstance()->GetDeltaTime();

	// 状態更新
	(this->*(m_StateFunc[m_state]))();

}

//==========================================================================
// なし
//==========================================================================
void CBaggage::StateNone()
{
	// 障害物との衝突判定
	if (Hit())
	{
		m_velorot.x += ROLL_FSTSPD;	// 衝突したらロール軸に回転速度を与える
	}

	m_fStateTimer = 0.0f;
}

//==========================================================================
// ダメージ
//==========================================================================
void CBaggage::StateDamage()
{
	if (StateTime::DAMAGE <= m_fStateTimer)
	{
		m_fStateTimer = 0.0f;
		m_state = STATE::STATE_NONE;
	}
}

//==========================================================================
// 死亡
//==========================================================================
void CBaggage::StateDead()
{
	// カメラ情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	MyLib::Vector3 posV = pCamera->GetPositionVDest();
	MyLib::Vector3 posR = pCamera->GetPositionRDest();
	MyLib::Vector3 ray = (posR - posV).Normal();


	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 posDest = posV + (200.0f * ray);

	pos = UtilFunc::Correction::EasingEaseIn(m_posAwayStart, posDest, 0.0f, StateTime::DEAD, m_fStateTimer);

	if (m_fStateTimer >= StateTime::DEAD)
	{
		pos = posDest;
		if (!m_bEnd)
		{
			//// フィードバックエフェクトON
			//CManager::GetInstance()->GetRenderer()->SetEnableDrawMultiScreen(
			//	MULTITARGET::START_ALPHA,
			//	MULTITARGET::START_MULTI,
			//	MULTITARGET::START_TIMER);

			// SE再生
			CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_CRACK_GRASS);
		}
		m_bEnd = true;
	}

	SetPosition(pos);
}

//==========================================================================
// デバッグ時変形
//==========================================================================
void CBaggage::DebugTransform()
{

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	if (ImGui::TreeNode("Transform"))
	{
		float windowWidth = 100.0f;
		const float  POS_MOVE = 0.5f;
		//=============================
		// 位置設定
		//=============================
		MyLib::Vector3 pos = GetPosition();
		ImGui::Text("pos");
		ImGui::SameLine();
		// X
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("x", &pos.x, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();
		// Y
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("y", &pos.y, POS_MOVE, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine();
		}
		ImGui::PopID();
		// Z
		ImGui::PushID(0); // ウィジェットごとに異なるIDを割り当てる
		{
			ImGui::SetNextItemWidth(windowWidth);
			ImGui::DragFloat("z", &pos.z, POS_MOVE, 0.0f, 0.0f, "%.2f");
		}
		ImGui::PopID();

		// 位置設定
		SetPosition(pos);
		SetMove(0.0f);
		ImGui::TreePop();
	}
}

//==========================================================================
// 力追加
//==========================================================================
void CBaggage::AddForce(const MyLib::Vector3& power, const MyLib::Vector3& ActPos)
{
	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	float angle = pos.AngleXY(ActPos);
	m_force.x = (power.x);
	m_force.x = fabsf(m_force.x);

	move.y += power.y;

	SetMove(move);

}

//==========================================================================
// 描画処理
//==========================================================================
void CBaggage::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ステンシルバッファ有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// 参照値設定
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// バッファへの値に対してのマスク設定
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法設定
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);

	// テスト結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);	// Z+ステンシル成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);		// Z+ステンシル失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);		// Zテストのみ失敗

	// ステンシル描画
	CObjectQuaternion::Draw();

	// ステンシルバッファ無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// 普通の描画
	CObjectQuaternion::Draw();
}

//==========================================================================
// 障害物との判定
//==========================================================================
bool CBaggage::Hit()
{
	// 障害物のリスト取得
	CListManager<CMap_Obstacle> list = CMap_Obstacle::GetListObj();

	// 先頭を保存
	std::list<CMap_Obstacle*>::iterator itr = list.GetEnd();
	CMap_Obstacle* pObj = nullptr;

	MyLib::Vector3 MyPos = GetPosition();
	while (list.ListLoop(itr))
	{
		CMap_Obstacle* pObj = *itr;
		MyLib::Vector3 ObjPos = pObj->GetPosition();

		CMap_ObstacleManager::SObstacleInfo info = pObj->GetObstacleInfo();
		for (const auto& collider : info.boxcolliders)
		{
			if (UtilFunc::Collision::IsAABBCollidingWithBox(GetAABB(), GetWorldMtx(), MyLib::AABB(collider.vtxMin, collider.vtxMax), collider.worldmtx))
			{
				// 向き取得
				float angle = MyPos.AngleXY(ObjPos);

				bool bDead = UtilFunc::Collision::CollisionRangeAngle(angle, DEADANGLE_HIT + (-D3DX_PI * 0.5f), -DEADANGLE_HIT + (-D3DX_PI * 0.5f));
				
				// 移動量取得
				MyLib::Vector3 move = GetMove();

				if (bDead)
				{
					ImGui::Text("dead");

					// 死亡状態
					m_state = STATE::STATE_DEAD;

					MyLib::Vector3 hitmove = (MyPos - ObjPos) * 1.0f;
					move.x += hitmove.x;
					move.z += hitmove.z;

					SetForce(0.0f);

					// 吹っ飛びスタート地点
					m_posAwayStart = MyPos;
				}
				else
				{
					ImGui::Text("no dead");

					move.y *= -1.0f;

					// ダメージ
					m_state = STATE::STATE_DAMAGE;

					/*CMyEffekseer::GetInstance()->SetEffect(
						CMyEffekseer::EFKLABEL::EFKLABEL_IMPACT,
						GetWorldMtx().GetWorldPosition(), MyLib::Vector3(0.0f, 0.0f, 0.0f), 0.0f, 30.0f, true);*/
					m_bHit = true;
				}
				SetMove(move);

				// SE再生
				CSound::GetInstance()->PlaySound(CSound::LABEL::LABEL_SE_HIT);

				return true;
			}
		}
	}

	m_bHit = false;
	return false;
}

//==========================================================================
// 判定リセット
//==========================================================================
void CBaggage::Reset()
{
	m_bHit = false;
	m_bEnd = false;
	m_bLand = false;
	m_velorot = MyLib::Vector3(0.0f, 0.0f, 0.0f);
	m_fDeviation = 0.0f;
	m_fStateTimer = 0.0f;
	m_state = STATE::STATE_NONE;
}