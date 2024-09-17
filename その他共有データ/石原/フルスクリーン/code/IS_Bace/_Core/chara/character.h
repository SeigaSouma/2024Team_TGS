//======================================================
//
//キャラクター処理のヘッダ[character.h]
//Author:石原颯馬
//
//======================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//必要なのインクルード
#include "..\_object\object_empty\object.h"
#include "characterconfig.h"

//前方宣言
class CCharaParts;
class CMotion;

//キャラクタークラス
class CCharacter : public CObject
{
public:
	//モーション種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_MOVE,
		MOTIONTYPE_ACTION,	//未実装
		MOTIONTYPE_JUMP,
		MOTIONTYPE_LAND,
	} MOTIONTYPE;

	//コンストラクタ・デストラクタ
	CCharacter();
	~CCharacter();

	//基本処理
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	//取得
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return m_rot; }

	//設定
	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	void SetChara(CCharacterConfig::SCharacterConfig &charaConfig);

	//生成
	static CCharacter* Create(CCharacterConfig::SCharacterConfig &charaConfig, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	static CCharacter* Create(std::string path, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

	//リスト取得
	static CCharacter* GetTop(void) { return m_pTop; }
	CCharacter* GetNext(void) { return m_pNext; }

	//除外（のちに消える）
	void Exclusion() {}

private:
	//リスト
	static CCharacter* m_pTop;		//先頭オブジェクト
	static CCharacter* m_pCur;		//最後尾オブジェクト
	CCharacter* m_pNext;			//次のオブジェクト
	CCharacter* m_pPrev;			//前のオブジェクト
	bool m_bExclusion;				//除外フラグ
	static int m_nNumAll;			//総数

	CCharacterConfig::SCharacterConfig m_charaConfig;
	CCharaParts** m_ppCharaParts;	//キャラパーツ
	int m_nNumParts;				//パーツ数
	CMotion* m_pMotion;				//モーションポインタ
	D3DXMATRIX m_mtxWorld;			//ワールドマトリ

	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_rot;	//向き

};

#endif // !_CHARACTER_H_
