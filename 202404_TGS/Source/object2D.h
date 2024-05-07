//=============================================================================
// 
//  オブジェクト2Dヘッダー [object2D.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_	// 二重インクルード防止

#include "object.h"

//==========================================================================
// クラス定義
//==========================================================================
// オブジェクト2Dクラス定義
class CObject2D : public CObject
{
public:

	CObject2D(int nPriority = mylib_const::PRIORITY_DEF2D, const LAYER layer = LAYER::LAYER_2D);
	~CObject2D();

	// オーバーライドされた関数
	HRESULT Init();
	HRESULT Init(int nNumVertex);
	void Uninit();
	void Update();
	void Draw();
	void Draw(int nNumVertex);
	void Draw(LPDIRECT3DTEXTURE9 mutitex);
	virtual void SetVtx();
	virtual void SetVtx(int nNumVertex);
	void BindTexture(int nIdx);
	int GetIdxTexture() { return m_nTexIdx; }

	virtual void SetColor(const D3DXCOLOR col);			// 色設定
	virtual D3DXCOLOR GetColor() const;					// 色取得
	virtual void SetAlpha(const float col);				// 不透明度設定
	virtual float GetAlpha() const;						// 不透明度取得
	virtual void SetSize(const D3DXVECTOR2 size);		// サイズの設定
	virtual D3DXVECTOR2 GetSize() const;				// サイズの取得
	virtual void SetSizeOrigin(const D3DXVECTOR2 size);	// 元のサイズの設定
	virtual D3DXVECTOR2 GetSizeOrigin() const;			// 元のサイズの取得
	virtual void SetTex(D3DXVECTOR2 *tex);				// テクスチャ座標の設定
	virtual D3DXVECTOR2 *GetTex();						// テクスチャ座標の取得
	virtual MyLib::Vector3 *GetVtxPos();				// 頂点座標取得
	virtual void SetVtxPos(MyLib::Vector3 *pos);		// 頂点座標取得

	static CObject2D *Create();
	static CObject2D *Create(int nPriority);
	static CObject2D *Create(int nPriority, int nNumVtx);
	CObject2D *GetObject2D();
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() const;
protected:

private:

	void SetLength(const float fLength);	// 対角線の長さ設定
	float GetLength() const;				// 対角線の長さ取得
	void SetAngle(const float fAngle);		// 対角線の向き設定
	float GetAngle() const;					// 対角線の向き取得

	D3DXCOLOR m_col;			// 色
	D3DXVECTOR2 m_size;			// サイズ
	D3DXVECTOR2 m_sizeOrigin;	// 元のサイズ
	D3DXVECTOR2 m_fTex[32];		// テクスチャ座標
	float m_fLength;			// 対角線の長さ
	float m_fAngle;				// 対角線の向き
	MyLib::Vector3 m_VtxPos[32];		// 頂点座標
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファ
	int m_nTexIdx;					// テクスチャのインデックス番号
};

#endif