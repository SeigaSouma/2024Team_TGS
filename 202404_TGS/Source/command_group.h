//=============================================================================
//
// コマンドグループヘッダー [command_group.h]
// Author : Ibuki Okusada
//
//=============================================================================
#ifndef _COMMAND_GROUP_H_
#define _COMMAND_GROUP_H_	// 二重インクルード防止

#include "command.h"

//==========================================================================
// クラス定義
//==========================================================================
class CCommandGroup
{
public:

	// コンストラクタ
	CCommandGroup();
	~CCommandGroup() {}

public:

	// メンバ関数
	HRESULT Init();
	void Uninit();
	void ListIn(CCommand* command);
	static CCommandGroup* Create(const std::string& string);
	int GetSuccess();

	// 静的メンバ関数

private:

	void Load(const std::string& string);

	std::vector<CCommand*> m_CommandList;	// コマンドリスト
};


#endif
