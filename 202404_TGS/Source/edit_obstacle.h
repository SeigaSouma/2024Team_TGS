//=============================================================================
// 
//  障害物エディタ―ヘッダー [edit_obstacle.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _EDIT_OBSTACLE_H_
#define _EDIT_OBSTACLE_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectX.h"
#include "edithandle.h"
#include "edit.h"

//==========================================================================
// クラス定義
//==========================================================================
// 障害物エディタ―クラス
class CEdit_Obstacle : public CEdit
{
public:

	CEdit_Obstacle();
	~CEdit_Obstacle();
	
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;



private:

	//=============================
	// 構造体定義
	//=============================
	struct SMapModelInfo
	{
		int nIdx;
		std::string fileName;
	};

	//=============================
	// メンバ関数
	//=============================
	void Resize();
	void MenuBar();
	void CreateBoxLine();	// 当たり判定ボックス生成
	void DeleteBoxLine();	// 当たり判定ボックス削除

	//=============================
	// メンバ変数
	//=============================
	int m_nEditIdx;						// 調整するインデックス
	int m_nColliderIdx;					// 調整するコライダーのインデックス
	std::vector<CObjectX*> m_pObjX;		// オブジェクトXのポインタ
	MyLib::AABB m_AABB;	// AABB情報
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// 当たり判定ボックス
};

#endif