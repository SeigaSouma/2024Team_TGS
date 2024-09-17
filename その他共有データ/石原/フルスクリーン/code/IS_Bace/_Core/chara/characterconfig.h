//======================================================
//
//�L�����N�^�[�\���f�[�^�̃w�b�_[characterconfig.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _CHARACTER_CONFIG_H_
#define _CHARACTER_CONFIG_H_

//�K�v�Ȃ̃C���N���[�h
#include <list>
#include <vector>
#include <string>

//�L�����\���f�[�^�N���X
class CCharacterConfig
{
public:
	//�\���f�[�^
	struct SParts	//�L�����p�[�c
	{
		int index;
		int parent;
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	struct SCharacterHierarchy	//�L�����K�w�\��
	{
		std::vector<SParts> parts;
		int num_parts;
		float move;
		float jump;
		float radius;
	};

	struct SKey	//�L�������[�V�����L�[
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	struct SKeySet	//�L�������[�V�����L�[�Z�b�g
	{
		std::vector<SKey> keys;
		int frame;
	};

	struct SMotion	//�L�������[�V����
	{
		std::vector<SKeySet> keysets;
		int num_key;
		bool isLoop;
	};

	struct SCharacterConfig	//�L�����\���f�[�^
	{
		SCharacterHierarchy charaHierarchy;
		std::vector<SMotion> motions;
		std::vector<std::string> filePath;
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CCharacterConfig();
	~CCharacterConfig();

	//�ǂݍ��݊֌W
	static CCharacterConfig* Load(std::string pPath);
	void Unload(void);
	static void UnloadAll(void);

	//�擾
	SCharacterConfig &GetConfig() { return m_charaConfig; }
private:
	static std::list<CCharacterConfig*> m_charaConfigList;
	SCharacterConfig m_charaConfig;	//�L�����\���f�[�^
	std::string m_filePath;			//�L�����\���f�[�^�i���[�V�����j�̃t�@�C���p�X
};

#endif // !_CHARACTER_H_
