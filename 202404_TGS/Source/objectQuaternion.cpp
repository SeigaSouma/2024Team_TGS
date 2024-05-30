//=============================================================================
// 
//  クォータニオン処理 [objectQuaternion.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "objectQuaternion.h"
#include "calculation.h"
#include "manager.h"
#include "debugproc.h"
#include "input.h"

//==========================================================================
// コンストラクタ
//==========================================================================
CObjectQuaternion::CObjectQuaternion(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	D3DXMatrixIdentity(&m_mtxWorld);			// ワールドマトリックス
	D3DXQuaternionIdentity(&m_quaternion);
	m_vecAxis = 0.0f;			// 回転軸
	m_fValueRot = 0.0f;			// 回転角
	D3DXMatrixIdentity(&m_RotationMtx);	// 計算用マトリックス宣言
	m_fRotDest = 0.0f;
}

//==========================================================================
// デストラクタ
//==========================================================================
CObjectQuaternion::~CObjectQuaternion()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CObjectQuaternion* CObjectQuaternion::Create()
{
	// メモリの確保
	CObjectQuaternion* pObjChara = DEBUG_NEW CObjectQuaternion;

	if (pObjChara != nullptr)
	{
		// 初期化処理
		HRESULT hr = pObjChara->Init();
		if (FAILED(hr)){
			return nullptr;
		}
	}

	return pObjChara;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectQuaternion::Init()
{
	CObjectX::Init("data\\MODEL\\box.x");
	CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CObjectQuaternion::Init(const std::string& file)
{
	CObjectX::Init(file);
	CObject::SetType(CObject::TYPE::TYPE_OBJECTX);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CObjectQuaternion::Uninit()
{
	// 終了処理
	CObjectX::Uninit();

	// オブジェクトの破棄
	Release();
}

//==========================================================================
// 更新処理
//==========================================================================
void CObjectQuaternion::Update()
{
	// オブジェクトXの更新処理
	CObjectX::Update();
#if 0
	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();
	MyLib::Vector3 rot = GetRotation();


	MyLib::Vector3 vec = MyLib::Vector3(sinf(D3DX_PI + rot.y), 0.0f, cosf(D3DX_PI + rot.y));

	m_vecAxis.x = vec.z;
	m_vecAxis.z = -vec.x;



	// 位置更新
	pos += move;

	// 回転角設定
	m_fValueRot = move.x * 0.005f;

	move.x += (0.0f - move.x) * 0.05f;


	// 設定
	SetPosition(pos);
	SetMove(move);
	SetRotation(rot);
#endif

}

//==========================================================================
// ワールドマトリックスの計算処理
//==========================================================================
void CObjectQuaternion::CalWorldMtx()
{
#if 0
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス宣言

	// 情報取得
	MyLib::Vector3 pos = GetPosition();

	// 初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXQuaternionIdentity(&m_quaternion);


	// 差分
	MyLib::Vector3 rotDiff = GetRotation() - GetOldRotation();

	// 任意の回転軸における回転角からクォータニオン計算
	D3DXQuaternionRotationAxis(&m_quaternion, &m_vecAxis, m_fValueRot);

	// 回転マトリックスを作成
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);

	// クォータニオンを正規化
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);


	// 向きを反映する
	D3DXMatrixMultiply(&m_RotationMtx, &m_RotationMtx, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_RotationMtx);

	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
#else


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス宣言

	// 情報取得
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 rot = GetRotation();
	MyLib::Vector3 rotOld = GetOldRotation();

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	D3DXQuaternionIdentity(&m_quaternion);

	MyLib::Vector3 vec;

	D3DXQUATERNION quatX;
	vec = { 1.0f, 0.0f, 0.0f };
	D3DXQuaternionRotationAxis(&quatX, &vec, rotOld.x);

	D3DXQUATERNION quatY;
	vec = { 0.0f, 1.0f, 0.0f };
	D3DXQuaternionRotationAxis(&quatY, &vec, rotOld.y);

	D3DXQUATERNION quatZ;
	vec = { 0.0f, 0.0f, 1.0f };
	D3DXQuaternionRotationAxis(&quatZ, &vec, rotOld.z);

	// x軸、y軸、z軸の順で回転を適用
	m_quaternion = quatY * m_quaternion;
	m_quaternion = quatX * m_quaternion;
	m_quaternion = quatZ * m_quaternion;

	// クォータニオンを正規化
	D3DXQuaternionNormalize(&m_quaternion, &m_quaternion);

	// クォータニオンを組み合わせて回転マトリックスを作成
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixRotationQuaternion(&mtxRot, &m_quaternion);


	// 向きを反映する
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);


	// 位置を反映する
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


#endif
}

//==========================================================================
// 描画処理
//==========================================================================
void CObjectQuaternion::Draw()
{
	// ワールドマトリックスの計算処理
	CalWorldMtx();

	// 描画のみ
	CObjectX::DrawOnly();
}
