//=============================================================================
//
// コマンドグループ処理 [command_group.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "command_group.h"

//==========================================================================
// 定数定義
//==========================================================================
namespace
{

}

//==========================================================================
// コンストラクタ
//==========================================================================
CCommandGroup::CCommandGroup()
{
	m_CommandList.clear();
}

//==========================================================================
// 初期化処理
//==========================================================================
HRESULT CCommandGroup::Init()
{
	m_CommandList.clear();

	return S_OK;
}

//==========================================================================
// 終了処理
//==========================================================================
void CCommandGroup::Uninit()
{
	for (auto it = m_CommandList.begin(); it != m_CommandList.end(); it++) {
		if ((*it) == nullptr) continue;
		(*it)->Uninit();
		delete (*it);
		(*it) = nullptr;
	}

	m_CommandList.clear();
}

//==========================================================================
// 内部に登録
//==========================================================================
void CCommandGroup::ListIn(CCommand* command)
{
	m_CommandList.push_back(command);
}

//==========================================================================
// コンフィグ生成
//==========================================================================
CCommandGroup* CCommandGroup::Create(const std::string& string)
{
	CCommandGroup* pGroup = DEBUG_NEW CCommandGroup;

	if (pGroup != nullptr) {
		pGroup->Load(string);
	}

	return pGroup;
}

//==========================================================================
// コマンド成功番号を取得
//==========================================================================
int CCommandGroup::GetSuccess()
{
	bool value = false;
	for (auto it = m_CommandList.begin(); it != m_CommandList.end(); it++) {

		if ((*it)->GetCommand()) return (*it)->GetId();
	}

	return -1;
}

//==========================================================================
// 読み込み
//==========================================================================
void CCommandGroup::Load(const std::string& string)
{
	// ファイルを開く
	std::ifstream File(string);
	if (!File.is_open()) {
		return;
	}

	int id = 0;

	// コメント用
	std::string hoge;

	// データ読み込み
	std::string line;
	while (std::getline(File, line))
	{
		// コメントはスキップ
		if (line.empty() ||
			line[0] == '#')
		{
			continue;
		}

		// ストリーム作成
		std::istringstream lineStream(line);

		if (line.find("COMMAND_FILENAME") != std::string::npos)
		{// COMMAND_FILENAMEでコマンド情報読み込み
			std::string commandfile;
			commandfile.clear();

			// 情報渡す
			lineStream >>
				hoge >>
				hoge >>
				commandfile;	// 配置物の種類

			CCommandPad* pPad = DEBUG_NEW CCommandPad;
			pPad->Load(commandfile);
			pPad->SetId(id);
			ListIn(pPad);
			id++;
		}

		if (line.find("END_SCRIPT") != std::string::npos)
		{
			break;
		}
	}

	// ファイルを閉じる
	File.close();
}
