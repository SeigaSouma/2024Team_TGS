//=============================================================================
// 
//  円形で回転する鳥障害物処理 [obstacle_birdcircle.cpp]
//  Author : Ibuki Okusada
// 
//=============================================================================
#include "obstacle_birdcircle.h"
#include "debugproc.h"
#include "manager.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const int	NUM_BIRD = (10);				// 鳥の総数
	const float BIRD_ROT = (1.0f / NUM_BIRD);	// 1鳥辺りの角度割合
}

// デフォルト情報
namespace BIRDDEFAULT
{
	const float ROTATE_SPEED = (-0.02f);	// 回転速度
	const float LENGTH = (750.0f);			// 中心からの距離
	const float PLUSLENGTH = (250.0f);		// 変化する距離
}

//==========================================================================
// コンストラクタ
//==========================================================================
CObstacle_BirdCircle::CObstacle_BirdCircle(int nPriority,
	CObject::LAYER layer) : CMap_Obstacle(nPriority, layer)
{
	// 値のクリア
	m_BirdList.clear();
}

//==========================================================================
// デストラクタ
//==========================================================================
CObstacle_BirdCircle::~CObstacle_BirdCircle()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObstacle_BirdCircle* CObstacle_BirdCircle::Create(const CMap_ObstacleManager::SObstacleInfo& info)
{
	// メモリの確保
	CObstacle_BirdCircle* pObj = DEBUG_NEW CObstacle_BirdCircle;

	if (pObj != nullptr)
	{
		pObj->SetObstacleInfo(info);
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObstacle_BirdCircle::Init()
{
	CMap_Obstacle::GetListObj().Regist(this);
	MyLib::Vector3 rot;

	// 種類の設定
	CObject::SetType(TYPE_OBJECTX);

	// 鳥を生成する
	for (int i = 0; i < NUM_BIRD; i++)
	{
		BirdInfo info;
		info.nIdx = i;
		info.pBird = CMap_Obstacle::Create(GetObstacleInfo(), false);
		rot.y = (i * BIRD_ROT) * (D3DX_PI * 2);
		UtilFunc::Transformation::RotNormalize(rot.y);
		info.pBird->SetRotation(rot);
		SetBirdOffSet(info);
		info.pBird->SetPosition(info.offset);
		m_BirdList.push_back(info);
	}

	// 初期値設定
	m_Info.fDefLength = BIRDDEFAULT::LENGTH;
	m_Info.fPlusLength = BIRDDEFAULT::PLUSLENGTH;
	m_Info.fRotSpeed = BIRDDEFAULT::ROTATE_SPEED;
	m_Info.fNowLength = BIRDDEFAULT::LENGTH;

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObstacle_BirdCircle::Uninit()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird != nullptr)
		{
			it.pBird->Uninit();
		}
	}

	// リストクリア
	m_BirdList.clear();

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 削除
//==========================================================================
void CObstacle_BirdCircle::Kill()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird != nullptr)
		{
			it.pBird->Uninit();
		}
	}

	// 終了処理
	CMap_Obstacle::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObstacle_BirdCircle::Update()
{
	// 回転(yは向き、xは移動に使用)
	MyLib::Vector3 rot = GetRotation();
	rot.y += m_Info.fRotSpeed;
	rot.x -= m_Info.fRotSpeed;
	UtilFunc::Transformation::RotNormalize(rot.y);
	UtilFunc::Transformation::RotNormalize(rot.x);
	SetRotation(rot);

	// 距離設定
	SetNowLength();

	// 鳥の更新
	ControllBird();
}

//==========================================================================
// 描画処理
//==========================================================================
void CObstacle_BirdCircle::Draw()
{

}

//==========================================================================
// 鳥の操作設定
//==========================================================================
void CObstacle_BirdCircle::ControllBird()
{
	for (auto& it : m_BirdList)
	{
		if (it.pBird == nullptr) continue;	// 存在していない

		MyLib::Vector3 rot = it.pBird->GetRotation();

		// 位置の設定
		{
			MyLib::Matrix mtxRot, mtxTrans, mtxScale, mtxBird;	// 計算用マトリックス宣言
			MyLib::Matrix mtxParent = GetWorldMtx();	// 親のマトリックス
			MyLib::Vector3 BirdPos = it.offset;

			// 座標用マトリックス初期化
			mtxBird.Identity();

			// 位置を反映する
			mtxTrans.Translation(BirdPos);
			mtxBird.Multiply(mtxBird, mtxTrans);

			mtxParent.Multiply(mtxBird, mtxParent);

			// 座標を設定
			BirdPos = (mtxBird._41, mtxBird._42, mtxBird._43);

			it.pBird->SetPosition(mtxParent.GetWorldPosition());
		}

		// 向きとオフセット設定
		{
			rot.y = GetRotation().y + (D3DX_PI * 2) * (BIRD_ROT * it.nIdx);
			UtilFunc::Transformation::RotNormalize(rot.y);
			it.pBird->SetRotation(rot);
			SetBirdOffSet(it);

			CManager::GetInstance()->GetDebugProc()->Print("角度 [ %f ]\n", rot.y);
		}
	}
}

//==========================================================================
// 鳥のオフセット設定
//==========================================================================
void CObstacle_BirdCircle::SetBirdOffSet(BirdInfo& info)
{
	// 自身の存在する向きを取得
	MyLib::Vector3 rot = GetRotation();
	rot.x = GetRotation().x + (info.nIdx * BIRD_ROT) * (-D3DX_PI * 2);
	UtilFunc::Transformation::RotNormalize(rot.y);

	// 割合を求める
	float rate = rot.x;
	rate = fabsf(fabsf(rate) / D3DX_PI - 1.0f);

	// オフセットを設定
	info.offset.z = sinf(rot.x) * m_Info.fNowLength;
	info.offset.x = cosf(rot.x) * m_Info.fNowLength;
}

//==========================================================================
// 現在の距離設定
//==========================================================================
void CObstacle_BirdCircle::SetNowLength()
{
	// 本体の向きをから加算距離を設定
	MyLib::Vector3 rot = GetRotation();
	float rate = sinf(rot.x);
	m_Info.fNowLength = m_Info.fDefLength + (m_Info.fPlusLength * rate);
}

//==========================================================================
// 可変情報設定
//==========================================================================
void CObstacle_BirdCircle::SetInfo(const float fDefLen, const float fPlusLen, const float fRotSpd)
{
	// 値の設定
	m_Info.fDefLength = fDefLen;
	m_Info.fPlusLength = fPlusLen;
	m_Info.fRotSpeed = fRotSpd;
}