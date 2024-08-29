//=============================================================================
// 
//  サフォケーション処理 [suffocation.cpp]
//  Author : 相馬靜雅
// 
//=============================================================================
#include "suffocation.h"
#include "manager.h"
#include "calculation.h"
#include "input.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{
	const std::string TEXTURE_SAMPLE = "data\\TEXTURE\\subtitle\\suffocation.png";	// テクスチャのファイル
}

//==========================================================================
// コンストラクタ
//==========================================================================
CSuffocation::CSuffocation(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
}

//==========================================================================
// デストラクタ
//==========================================================================
CSuffocation::~CSuffocation()
{

}

//==========================================================================
// 生成処理
//==========================================================================
CSuffocation* CSuffocation::Create()
{
	// メモリの確保
	CSuffocation* pObj = DEBUG_NEW CSuffocation;

	if (pObj != nullptr)
	{
		// 初期化処理
		pObj->Init();
	}

	return pObj;
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CSuffocation::Init()
{

	// オブジェクト2Dの初期化
	CObject2D::Init();

	MyLib::Vector3 move = GetMove();
	move = UtilFunc::Transformation::Random(-50, 50) * 0.1f;
	SetMove(move);

	// テクスチャ設定
	int texID = CTexture::GetInstance()->Regist(TEXTURE_SAMPLE);
	BindTexture(texID);

	// サイズ設定
	D3DXVECTOR2 size = CTexture::GetInstance()->GetImageSize(texID);

#if 0	// 横幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);

#else	// 縦幅を元にサイズ設定
	size = UtilFunc::Transformation::AdjustSizeByWidth(size, 240.0f);
#endif
	SetSize(size);
	SetSizeOrigin(size);
	SetPosition(MyLib::Vector3(SCREEN_WIDTH * 0.5f,600.0f,0.0f));


	// 位置、向き設定は必要があれば追加

	// 種類の設定
	SetType(CObject::TYPE::TYPE_OBJECT2D);

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CSuffocation::Uninit()
{
	// 終了処理
	CObject2D::Uninit();
}

//==========================================================================
// 更新処理
//==========================================================================
void CSuffocation::Update()
{
	// 更新処理
	CObject2D::Update();

	int nCtr;
	MyLib::Vector3 pos = GetPosition();
	MyLib::Vector3 move = GetMove();

	if (UtilFunc::Transformation::Random(0, 2) == 0)
	{
		move.x *= -1.0f;
	}

	move.y = -1.2f;
	pos += move;

	if (pos.y <= SCREEN_HEIGHT * 0.5f)
	{
		move.y = 0.0f;
		pos.y = SCREEN_HEIGHT * 0.5f;
		SetPosition(pos);
	}
	SetMove(move);
	SetPosition(pos);

}

//==========================================================================
// 描画処理
//==========================================================================
void CSuffocation::Draw()
{
	// 描画処理
	CObject2D::Draw();
}
