//=============================================================================
// 
//  アーチ状の魚障害物処理 [obstacle_fisharch.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "obstacle_fisharch.h"
#include "debugproc.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int NUM_FISH = (10);				// 魚の総数
	const float ROTATE_SPEED = (0.02f);		// 回転速度
	const float FISH_ROT = (1.0f / NUM_FISH);	// 1魚辺りの角度割合
	const float WIDTH = (750.0f);			// 中心からの距離
	const float HEIGHT = (1500.0f);			// 変化する距離
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObstacle_FishArch::CObstacle_FishArch(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// 値のクリア
	m_FishList.clear();
}

//==========================================================================
// デストラクタ
//==========================================================================
CObstacle_FishArch::~CObstacle_FishArch()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObstacle_FishArch* CObstacle_FishArch::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// メモリの確保
	CObstacle_FishArch* pObj = DEBUG_NEW CObstacle_FishArch;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObstacle_FishArch::Init()
{
	CMap_Obstacle::GetListObj().Regist(this);
	MyLib::Vector3 rot;
	
	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 魚を生成する
	for (int i = 0; i < NUM_FISH; i++)
	{
		FishInfo info;
		info.nIdx = i;
		info.pFish = CMap_Obstacle::Create(GetObstacleInfo(), false);
		rot.x = (i * FISH_ROT) * (D3DX_PI * 2);
		UtilFunc::Transformation::RotNormalize(rot.x);
		info.pFish->SetRotation(rot);
		SetFishOffSet(info);
		info.pFish->SetPosition(info.offset);
		m_FishList.push_back(info);
	}

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObstacle_FishArch::Uninit()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish != nullptr)
		{
			it.pFish->Uninit();
		}
	}

	// リストクリア
	m_FishList.clear();

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CObstacle_FishArch::Kill()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish != nullptr)
		{
			it.pFish->Uninit();
		}
	}

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObstacle_FishArch::Update()
{
	// 回転
	MyLib::Vector3 rot = GetRotation();
	rot.x += ROTATE_SPEED;
	UtilFunc::Transformation::RotNormalize(rot.x);
	SetRotation(rot);

	// 魚の更新
	ControllFish();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObstacle_FishArch::Draw()
{
	
}

//==========================================================================
// 魚の操作設定
//==========================================================================
void CObstacle_FishArch::ControllFish()
{
	for (auto& it : m_FishList)
	{
		if (it.pFish == nullptr) continue;	// 存在していない

		MyLib::Vector3 rot = it.pFish->GetRotation();

		// 位置の設定
		{
			MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxFish;	// 計算用マトリックス宣言
			MyLib::Matrix mtxParent = GetWorldMtx();	// 親のマトリックス
			MyLib::Vector3 FishPos = it.offset;

			// 座標用マトリックス初期化
			mtxFish.Identity();

			// 位置を反映する
			mtxTrans.Translation(FishPos);
			mtxFish.Multiply(mtxFish, mtxTrans);

			//// 向きを反映する
			//mtxRot.RotationYawPitchRoll(GetRotation().y, GetRotation().x, GetRotation().z);
			//mtxFish.Multiply(mtxFish, mtxRot);

			mtxParent.Multiply(mtxFish, mtxParent);

			// 座標を設定
			FishPos = (mtxFish._41, mtxFish._42, mtxFish._43);

			it.pFish->SetPosition(mtxParent.GetWorldPosition());
		}

		// 向きとオフセット設定
		{	
			rot.x = GetRotation().x + (D3DX_PI * 2) * (FISH_ROT * it.nIdx);
			UtilFunc::Transformation::RotNormalize(rot.x);
			it.pFish->SetRotation(rot);
			SetFishOffSet(it);

			CManager::GetInstance()->GetDebugProc()->Print("角度 [ %f ]\n", rot.x);
		}
	}
}

//==========================================================================
// 魚のオフセット設定
//==========================================================================
void CObstacle_FishArch::SetFishOffSet(FishInfo& info)
{
	MyLib::Vector3 rot = GetRotation();
	rot.x += (D3DX_PI * 2) * (FISH_ROT * info.nIdx);
	UtilFunc::Transformation::RotNormalize(rot.x);

	// 割合を求める
	float rate = rot.x;

	rate = fabsf(fabsf(rate) / D3DX_PI - 1.0f);

	info.offset.z = sinf(rot.x) * WIDTH;
	info.offset.y = cosf(rot.x) * HEIGHT;
}