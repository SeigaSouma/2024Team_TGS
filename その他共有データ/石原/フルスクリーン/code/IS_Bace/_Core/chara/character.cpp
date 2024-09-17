//======================================================
//
//キャラクター処理[character.cpp]
//Author:石原颯馬
//
//======================================================
#include "character.h"
#include "..\manager\manager.h"
#include "..\renderer\renderer.h"
#include "..\file(legacy)\file.h"
#include "..\xmodel\xmodel.h"
#include "characterconfig.h"
#include "charaparts/charaparts.h"
#include "charamotion/motion.h"
#include "../utility/Utility.h"

//静的メンバ変数
CCharacter* CCharacter::m_pTop = nullptr;
CCharacter* CCharacter::m_pCur = nullptr;
int CCharacter::m_nNumAll = 0;

//=================================
//コンストラクタ
//=================================
CCharacter::CCharacter()
{
	m_ppCharaParts = nullptr;
	m_nNumParts = 0;
	m_pMotion = nullptr;
	m_pos = IS_Utility::VEC3_ZERO;
	m_rot = IS_Utility::VEC3_ZERO;

	if (m_pCur == nullptr)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;		//俺が先頭
		m_pPrev = nullptr;		//前後誰もいない
		m_pNext = nullptr;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = nullptr;			//自分の次のオブジェはいない
	}
	m_pCur = this;				//俺が最後尾
	m_bExclusion = false;		//生きてる
	m_nNumAll++;
}

//=================================
//デストラクタ
//=================================
CCharacter::~CCharacter()
{
}

//========================
//初期化処理
//========================
HRESULT CCharacter::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CCharacter::Uninit(void)
{
	//モーション破棄
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_ppCharaParts != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumParts; cnt++)
		{//一つずつ消す
			if (m_ppCharaParts[cnt] != nullptr)
			{
				m_ppCharaParts[cnt]->Uninit();
				m_ppCharaParts[cnt] = nullptr;
			}
		}
		delete[] m_ppCharaParts;	//配列そのものを破棄
	}

	m_bExclusion = true;		//除外予定

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CCharacter::Update(void)
{
	//モデル更新
	if (m_ppCharaParts != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumParts; cnt++)
		{
			if (m_ppCharaParts[cnt] != nullptr)
			{
				m_ppCharaParts[cnt]->Update();
			}
		}
	}
}

//========================
//描画処理
//========================
void CCharacter::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイス取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//モデル取得
	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデル描画
	if (m_ppCharaParts != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumParts; cnt++)
		{
			if (m_ppCharaParts[cnt] != nullptr)
			{
				m_ppCharaParts[cnt]->Draw();
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=================================
//キャラ設定
//=================================
void CCharacter::SetChara(CCharacterConfig::SCharacterConfig &charaConfig)
{
	//キャラパーツモデルの読み込み
	CXModel** ppLoadedModel = new CXModel*[charaConfig.filePath.size()];
	for (int cnt = 0; cnt < charaConfig.filePath.size(); cnt++)
	{
		ppLoadedModel[cnt] = CXModel::Load(charaConfig.filePath[cnt].data());
	}

	//キャラパーツの生成
	m_nNumParts = charaConfig.charaHierarchy.num_parts;
	m_ppCharaParts = new CCharaParts*[m_nNumParts];
	for (int cnt = 0; cnt < m_nNumParts; cnt++)
	{
		//使用するデータ
		int partsIdx = charaConfig.charaHierarchy.parts[cnt].index;
		D3DXVECTOR3 posOffset = charaConfig.charaHierarchy.parts[cnt].pos;
		D3DXVECTOR3 rotOffset = charaConfig.charaHierarchy.parts[cnt].rot;

		//生成
		m_ppCharaParts[cnt] = CCharaParts::Create(posOffset, rotOffset, ppLoadedModel[partsIdx]);
	}

	//親子設定
	for (int cnt = 0; cnt < m_nNumParts; cnt++)
	{
		int parent = charaConfig.charaHierarchy.parts[cnt].parent;
		if (parent != -1)
		{//親がいる
			m_ppCharaParts[cnt]->SetParent(m_ppCharaParts[parent]);
		}
		else
		{//親がいない
			m_ppCharaParts[cnt]->SetParent(nullptr);
		}
	}

	//モーションにモデルを設定
	m_pMotion = new CMotion;
	m_pMotion->Init();
	m_pMotion->SetModel(m_ppCharaParts, m_nNumParts);

	//モーションの設定
	for (int cnt = 0; cnt < charaConfig.motions.size(); cnt++)
	{
		//登録モーション構造体
		CMotion::INFO info;

		//データ代入
		info.m_bLoop = charaConfig.motions[cnt].isLoop;
		info.m_nNumKey = charaConfig.motions[cnt].num_key;
		info.m_pKeyInfo = new CMotion::KEY_INFO[info.m_nNumKey];
		for (int cntKeySet = 0; cntKeySet < info.m_nNumKey; cntKeySet++)
		{
			info.m_pKeyInfo[cntKeySet].m_nFrame = charaConfig.motions[cnt].keysets[cntKeySet].frame;
			info.m_pKeyInfo[cntKeySet].m_pKey = new CMotion::KEY[m_nNumParts];
			for (int cntKey = 0; cntKey < charaConfig.motions[cnt].keysets[cntKeySet].keys.size(); cntKey++)
			{
				info.m_pKeyInfo[cntKeySet].m_pKey[cntKey].pos = charaConfig.motions[cnt].keysets[cntKeySet].keys[cntKey].pos;
				info.m_pKeyInfo[cntKeySet].m_pKey[cntKey].rot = charaConfig.motions[cnt].keysets[cntKeySet].keys[cntKey].rot;
			}
		}

		//モーション登録
		m_pMotion->SetInfo(info);
	}

	//読み込みの時に動的確保したものを破棄
	delete[] ppLoadedModel;
}

//=================================
//生成
//=================================
CCharacter* CCharacter::Create(CCharacterConfig::SCharacterConfig &charaConfig, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CCharacter* pObj = nullptr;

	if (pObj == nullptr)
	{
		//オブジェクトの生成
		pObj = new CCharacter;

		//初期化
		pObj->Init();

		//設定
		pObj->SetPos(pos);
		pObj->SetRot(rot);
		pObj->SetChara(charaConfig);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//生成（パス指定）
//=================================
CCharacter* CCharacter::Create(std::string path, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CCharacterConfig::SCharacterConfig charaConfig = CCharacterConfig::Load(path)->GetConfig();
	return CCharacter::Create(charaConfig, pos, rot);
}
