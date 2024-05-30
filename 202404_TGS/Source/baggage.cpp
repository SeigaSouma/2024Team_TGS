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
#include "map_obstacle.h"

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
	m_type = TYPE::TYPE_CLOTH;	// 種類
	m_fWeight = 0.0f;	// 重さ
	m_bDrop = false;	// 落下判定
	m_velorot = MyLib::Vector3(0.0f, 0.0f, 0.0f);
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
CBaggage *CBaggage::Create(TYPE type)
{
	// メモリの確保
	CBaggage* pObj = DEBUG_NEW CBaggage;

	if (pObj != nullptr)
	{
		// 引数情報
		pObj->m_type = type;

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
	int typeID = static_cast<int>(m_type);
	HRESULT hr = CObjectQuaternion::Init(MODEL[typeID]);
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

	CObjectQuaternion::Update();

	// 障害物との衝突判定
	if(Hit()) m_velorot.x += ROLL_FSTSPD;	// 衝突したらロール軸に回転速度を与える

	// 情報取得
	MyLib::Vector3 posOrigin = GetOriginPosition();
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 move = GetMove();
	ImGui::DragFloat("weight", &m_fWeight, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("PITCH_RATIO", &PITCH_RATIO, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("PITCH_INER", &PITCH_INER, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("ROLL_FSTSPD", &ROLL_FSTSPD, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::DragFloat("ROLL_INER", &ROLL_INER, 0.1f, 0.0f, 0.0f, "%.2f");

	// 位置更新
	pos += move;
	pos += m_force;

	// 回転(ピッチ軸
	if (move.y > 0.0f) m_velorot.y += fabsf(move.y) * 0.00025f;
	else m_velorot.y += (0.0f - m_velorot.y) * PITCH_INER;

	m_velorot.x += (0.0f - m_velorot.x) * ROLL_INER;
	rot.z -= m_velorot.x;
	rot.y += m_velorot.y;
	UtilFunc::Transformation::RotNormalize(rot);

	// 重力加算
	move.y -= mylib_const::GRAVITY * m_fWeight;

	static float limitMoveY = 30.0f;
	ImGui::DragFloat("Limit MoveY", &limitMoveY, 1.0f, 0.0f, 0.0f, "%.2f");

	if (move.y >= limitMoveY)
	{
		move.y = limitMoveY;
	}

	if (pos.y <= GetOriginPosition().y) pos.y = GetOriginPosition().y, move.y = 0.0f;

	// 落下判定
	m_bDrop = pos.y <= 0.0f;

	// 慣性補正
	move.x += (0.0f - move.x) * 0.25f;
	move.z += (0.0f - move.z) * 0.25f;
	/*m_force.x += (0.0f - m_force.x) * 0.01f;
	m_force.z += (0.0f - m_force.z) * 0.01f;*/

	// 情報設定
	SetPosition(pos);
	SetRotation(rot);
	SetMove(move);

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
	// 描画
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
				MyLib::Vector3 move = GetMove();
				move.y *= -1.0f;
				SetMove(move);
				return true;
			}
		}
	}

	return false;
}