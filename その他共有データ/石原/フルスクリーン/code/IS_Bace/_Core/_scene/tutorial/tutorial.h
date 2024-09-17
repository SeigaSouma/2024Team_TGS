//======================================================
//
//チュートリアルシーンのヘッダ[game.h]
//Author:石原颯馬
//
//======================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_
#include "..\..\manager\manager.h"

//前方宣言
class CPlayer;
class CSwitchManager;
class CBlockManager;
class CFade;
class CTutorialObj;

//仮
class CGoal;

//タイトルクラス
class CTutorial : public CScene
{
public:
	//静的const
	static const int TUTORIAL_MOVE;		//移動説明の番号
	static const int TUTORIAL_CHANGE;	//切替説明の番号

	//コンストラクタ・デストラクタ
	CTutorial();
	~CTutorial();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CPlayer* m_pPlayer;					//プレイヤーポインタ
	CSwitchManager* m_pSwitchManager;	//スイッチマネ
	CBlockManager* m_pBlockManager;		//ブロックマネ
	CFade* m_pFade;						//フェード
	CTutorialObj* m_pMoveTutorial;		//移動説明のオブジェクト（テクスチャ変えたい）
	CTutorialObj* m_pChangeTutorial;	//切替説明のオブジェクト（テクスチャ変えたい）
};

#endif // !_TUTORIAL_H_
