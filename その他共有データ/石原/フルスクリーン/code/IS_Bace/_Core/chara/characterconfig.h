//======================================================
//
//キャラクター構成データのヘッダ[characterconfig.h]
//Author:石原颯馬
//
//======================================================
#ifndef _CHARACTER_CONFIG_H_
#define _CHARACTER_CONFIG_H_

//必要なのインクルード
#include <list>
#include <vector>
#include <string>

//キャラ構成データクラス
class CCharacterConfig
{
public:
	//構成データ
	struct SParts	//キャラパーツ
	{
		int index;
		int parent;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	struct SCharacterHierarchy	//キャラ階層構造
	{
		std::vector<SParts> parts;
		int num_parts;
		float move;
		float jump;
		float radius;
	};

	struct SKey	//キャラモーションキー
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	struct SKeySet	//キャラモーションキーセット
	{
		std::vector<SKey> keys;
		int frame;
	};

	struct SMotion	//キャラモーション
	{
		std::vector<SKeySet> keysets;
		int num_key;
		bool isLoop;
	};

	struct SCharacterConfig	//キャラ構成データ
	{
		SCharacterHierarchy charaHierarchy;
		std::vector<SMotion> motions;
		std::vector<std::string> filePath;
	};

	//コンストラクタ・デストラクタ
	CCharacterConfig();
	~CCharacterConfig();

	//読み込み関係
	static CCharacterConfig* Load(std::string pPath);
	void Unload(void);
	static void UnloadAll(void);

	//取得
	SCharacterConfig &GetConfig() { return m_charaConfig; }
private:
	static std::list<CCharacterConfig*> m_charaConfigList;
	SCharacterConfig m_charaConfig;	//キャラ構成データ
	std::string m_filePath;			//キャラ構成データ（モーション）のファイルパス
};

#endif // !_CHARACTER_H_
