//======================================================
//
//オブジェクト（3D）処理のヘッダ[object3D.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECT3D_H_	//多重インクルード対策
#define _OBJECT3D_H_
#include "..\object_empty\object.h"

//オブジェクトクラス
class CObject3D : public CObject
{
public:
	//コンストラクタ・デストラクタ
	CObject3D(int nPriority = PRIORITY_DEFAULT);	//デフォルト
	virtual ~CObject3D();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//生成
	static CObject3D* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority = PRIORITY_DEFAULT);

	//取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return m_fWidth; }
	float GetDepth(void) { return m_fDepth; }
	int GetIdxTexture(void) { return m_nIdxTexture; }

	//テクスチャ設定
	void BindTexture(int nIdx) { m_nIdxTexture = nIdx; }

	//設定
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }	//位置
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }	//向き

	//除外
	virtual void Exclusion(void){}
protected:
	HRESULT SetCol(const D3DXCOLOR col);	//色
	void SetSize(const float fWidth, const float fDepth);
	HRESULT SetTex(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex3);	//テクスチャ座標
	HRESULT SetTex(const int nPatternU, const int nPatternV);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() { return m_pVtxbuff; }
	D3DXMATRIX* GetMtxWorld() { return &m_mtxWorld; }

private:
	void CulcNor(VERTEX_3D* pVtx);

	int m_nIdxTexture;					//テクスチャ番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxbuff;	//頂点バッファ
	D3DXMATRIX m_mtxWorld;				//ワールドマトリ

	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き
	float m_fWidth;		//幅(X)
	float m_fDepth;		//奥行(Z)
};

#endif	//多重