//======================================================
//
//キャラのパーツ処理のヘッダ[charaparts.h]
//Author:石原颯馬
//
//======================================================
#ifndef _CHARA_PARTS_H_
#define _CHARA_PARTS_H_
#include "../../_object/object_x/objectX.h"

//前方宣言
class CXModel;

//キャラのパーツクラス
class CCharaParts
{
public:
	//コンストラクタ・デストラクタ
	CCharaParts();
	~CCharaParts();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//生成
	static CCharaParts* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel);

	//取得
	D3DXMATRIX GetMtx() { return m_mtxWorld; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }

	//設定
	void SetParent(CCharaParts* pModel) { m_pParent = pModel; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetPosMotioned(D3DXVECTOR3 pos) { m_posMotioned = pos; }
	void SetRotMotioned(D3DXVECTOR3 rot) { m_rotMotioned = rot; }
private:
	CXModel* m_pModel;			//使用するXファイルモデル
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_posMotioned;	//モーション後の位置
	D3DXVECTOR3 m_rotMotioned;	//モーション後の向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリ
	CCharaParts* m_pParent;		//親のパーツ
};

#endif // !_MODEL_H_
