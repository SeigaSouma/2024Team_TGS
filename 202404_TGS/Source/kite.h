//=============================================================================
// 
//  凧ヘッダー [kite.h]
//  Author : IbukiOkusada
// 
//=============================================================================

#ifndef _KITE_H_
#define _KITE_H_	// 二重インクルード防止

#include "listmanager.h"
#include "objectChara.h"
#include "motion.h"
#include "people.h"

// 前方宣言
class CShadow;

//==========================================================================
// クラス定義
//==========================================================================
// 人クラス
class CKite : public CPeople
{
public:


	CKite(int nPriority = mylib_const::ENEMY_PRIORITY);
	virtual ~CKite();

	// オーバーライドされた関数
	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw() override;

	virtual void Kill();	// 削除

	static CKite* Create(const std::string& pFileName, MyLib::Vector3 pos);

protected:

private:

	MyLib::Vector3 m_StartRot;	// 開始向き
};



#endif