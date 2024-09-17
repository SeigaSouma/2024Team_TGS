//======================================================
//
//オブジェクト処理のヘッダ[object.h]
//Author:石原颯馬
//
//======================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#define MAX_OBJ				(256)	//オブジェクト最大数
#define DEATH_LIFE			(0)		//死亡体力

#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				//2D
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//3D

//頂点情報[2D]
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換係数（1.0fで固定）
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
} VERTEX_2D;

//頂点情報[3D]
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//カラー
	D3DXVECTOR2 tex;	//テクスチャ
} VERTEX_3D;

//オブジェクトクラス
class CObject
{
public:
	//種類列挙
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_BLOCK,
		TYPE_CHARACTER,
		TYPE_SWITCH,
		TYPE_GOAL,
		TYPE_ITEM,
		TYPE_TELEPORT,
		TYPE_TUTORIALOBJ,
		TYPE_MAX
	} TYPE;

	//優先順位参考表
	typedef enum
	{
		PRIORITY_BG = 0,		//BG
		PRIORITY_01,		//未定義
		PRIORITY_02,		//未定義
		PRIORITY_DEFAULT,		//デフォルト
		PRIORITY_04,		//未定義
		PRIORITY_05,			//UI向け
		PRIORITY_PAUSE,			//ポーズ
		PRIORITY_FADE,			//フェード
		PRIORITY_MAX			//最大優先順位（これで足りるらしい）
	} PRIORITY;

	//コンストラクタ・デストラクタ
	CObject(int nPriority = PRIORITY_DEFAULT);
	virtual ~CObject();

	//基本処理
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//全オブジェクト系
	static void ReleaseAll(void);					//すべて
	static void ReleaseAll(const int nPriority);	//優先度別
	static void UpdateAll(void);
	static void DrawAll(void);

	//取得・設定
	void SetType(TYPE type) { m_Type = type; }
	TYPE GetType(void) { return m_Type; }
	static CObject* GetTop(const int nPriority) { return m_apTop[nPriority]; }
	CObject* GetNext(void) { return m_pNext; }

	//除外（リスト実装ありの場合）
	virtual void Exclusion(void) = 0;

	//死亡フラグが立っているオブジェを殺す
	static void Death(void);

protected:
	void Release(void);

private:
	static CObject* m_apTop[PRIORITY_MAX];	//先頭オブジェクト
	static CObject* m_apCur[PRIORITY_MAX];	//最後尾オブジェクト
	CObject* m_pNext;		//次のオブジェクト
	CObject* m_pPrev;		//前のオブジェクト
	static int m_nNumAll;	//総数
	int m_nPriority;		//優先順位
	TYPE m_Type;			//種類
	bool m_bDeath;			//死亡フラグ
};

#endif // !_OBJECT_H_
