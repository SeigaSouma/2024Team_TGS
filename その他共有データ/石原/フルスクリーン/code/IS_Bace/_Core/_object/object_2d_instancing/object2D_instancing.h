//======================================================
//
//ポリゴン（2D,インスタンシングあり）処理のヘッダ[object2D_instancing.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECT2D_INSTANCING_H_	//多重インクルード対策
#define _OBJECT2D_INSTANCING_H_
#include "..\object_empty\object.h"
#include <list>

//オブジェクトクラス
class CObject2D_Instancing : public CObject
{
public:
	//インスタンシング用バッファ構造体
	struct SinglePolyVtx
	{
		float x, y;
		float u, v;
	};
	struct InstancePosVtx
	{
		float x, y;
	};
	struct InstanceUVVtx
	{
		float u, v;
	};

	//変数管理用
	struct Instance
	{
		InstancePosVtx pos;
		InstanceUVVtx uv;
		D3DXCOLOR col;
	};

	//コンストラクタ・デストラクタ
	CObject2D_Instancing(int nPriority = PRIORITY_04);	//デフォルト
	virtual ~CObject2D_Instancing();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObject2D_Instancing* Create(const float fWidth, const float fHeight, const int nInstanceNum, const int nPriority = PRIORITY_04);

	//単ポリ取得・設定
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	HRESULT SetSize(const float fWidth, const float fHeight);		//サイズ
	int GetIdxTexture(void) { return m_nIdxTexture; }
	virtual void BindTexture(int nIdx);
	void SetInstanceNum(const int num);

	//インスタンス設定
	void AddInstance(float x, float y, float u, float v, D3DXCOLOR col);	//全部直打ち
	void AddInstance(D3DXVECTOR3 pos, const int nPatternU, const int nPatternV, D3DXCOLOR col);	//直打ちめんどくせぇ！って人用
	void ResetList() { m_InstanceList.clear(); }

	//除外
	virtual void Exclusion(void){}

private:
	void AddList(Instance ins);
	void SetInstanceBuff();
	void ReleaseInstanceBuff();

	HRESULT SetVtx(void);

	static D3DVERTEXELEMENT9 m_aVtxElem[];
	LPDIRECT3DVERTEXDECLARATION9 m_pDecl;
	LPDIRECT3DVERTEXBUFFER9 m_pSinglePolyVtxBuff;
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pInstancePosVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pInstanceUVVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pInstanceColVtxBuff;
	LPD3DXEFFECT m_pEffect;
	float m_fWidth;		//幅
	float m_fHeight;	//高さ
	int m_nIdxTexture;	//テクスチャ番号
	
	std::list<Instance> m_InstanceList;
	int m_nInstanceNum;
};

#endif	//多重