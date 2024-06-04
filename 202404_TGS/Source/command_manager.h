//=============================================================================
//
// コマンドマネージャーヘッダー [command_manager.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _COMMAND_MANAGER_H_
#define _COMMAND_MANAGER_H_	// 二重インクルード防止

#include "command.h"

//==========================================================================
// クラス定義
//==========================================================================
class CCommandManager
{
public:
	

private:

	// コンストラクタ
	CCommandManager();
	virtual ~CCommandManager() {}

public:

	// メンバ関数
	void Uninit();

	// 静的メンバ関数
	static CCommandManager* GetInstance() { return m_pInstance; }
	static CCommandManager* Create();

private:

	std::vector<CCommand> m_CommandList;	// コマンドリスト
	static CCommandManager* m_pInstance;	// インスタンス
};


#endif
