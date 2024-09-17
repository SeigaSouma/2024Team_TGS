//==========================================
//
//�t�@�C���ǂݍ��݃v���O����[file.cpp]
//Author:�Ό��D�n
//
//==========================================
#include "file.h"
#include "../chara/characterconfig.h"
#include "../../_Core/manager/manager.h"
#include "../../_Core/texture/texture.h"
#include "../utility/Utility.h"
#include "../_object/object_x/objectX.h"
#include "../_object/object_billboard/objectBillboard.h"
#include "../chara/charaparts/charaparts.h"
#include "../chara/charamotion/motion.h"
#include "../xmodel/xmodel.h"
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <assert.h>

//�R�[�h�֌W
#define STRTOK_NEXT		(strtok(nullptr, " =\n"))

#if 0
//========================
//�}�b�v�ǂݍ��ݏ���
//========================
void LoadModelViewerFile(std::string path)
{
	READSTAT readStat = READSTAT_STARTSCRIPTS;
	bool bRead = false;
	int nCntInit = 0;	//�������J�E���^

	//�ǂݍ��݃f�[�^
	int nType = -1;
	D3DXVECTOR3 pos = IS_Utility::VEC3_ZERO, rot = IS_Utility::VEC3_ZERO;
	int nState = 0;
	bool bCollision = false, bShadow = false;


	std::fstream fs(path);

	if (fs.is_open())
	{
		std::string strCode;
		while (!fs.eof())
		{
			//1�s�ǂݎ��
			std::getline(fs, strCode);

			//�R�����g�A�E�g�`�F�b�N
			int commentCharPos = strCode.find_first_of("#", 0);
			if (commentCharPos != std::string::npos)
			{//�R�����g���������
				std::string::iterator itr = strCode.begin() + commentCharPos;
				strCode.erase(itr, strCode.end());
			}

			//�^�u����
			std::erase_if(strCode, [](char c) {return c == '\t'; });

			//������`�F�b�N
			if (strCode.starts_with(CODE_SCRIPT))
			{//�ǂݎ��J�n
				bRead = true;
			}
			else if (strCode.starts_with(CODE_END_SCRIPT))
			{//�ǂݎ��I��
				bRead = false;
				break;
			}
			else if (bRead == true)
			{//�ǂݎ��
				switch (readStat)
				{
				case READSTAT_STARTSCRIPTS:	//�����擾
					//��������
					if (strCode.starts_with(CODE_CAMERASET))
					{
						readStat = READSTAT_CAMERASET;
					}
					else if (strCode.starts_with(CODE_LIGHTSET))
					{
						readStat = READSTAT_LIGHTSET;
					}
					else if (strCode.starts_with(CODE_SKYSET))
					{
						readStat = READSTAT_SKYSET;
					}
					else if (strCode.starts_with(CODE_MOUNTAINSET))
					{
						readStat = READSTAT_MOUNTAINSET;
					}
					else if (strCode.starts_with(CODE_FIELDSET))
					{
						readStat = READSTAT_FIELDSET;
					}
					else if (strCode.starts_with(CODE_WALLSET))
					{
						readStat = READSTAT_WALLSET;
					}
					else if (strCode.starts_with(CODE_MODELSET))
					{
						readStat = READSTAT_MODELSET;
					}
					else if (strCode.starts_with(CODE_BILLBOARDSET))
					{
						readStat = READSTAT_BILLBOARDSET;
					}
					else if (strCode.starts_with(CODE_PLAYERSET))
					{
						readStat = READSTAT_PLAYERSET;
					}
					else if (strCode.starts_with(CODE_TEXTURE_FILENAME))
					{
						//�������e�̕�������
						strtok(strCode.data(), " =\n");

						//�^�u����
						std::string str = STRTOK_NEXT;
						std::erase_if(str, [](char c) {return c == '\t'; });

						//�e�N�X�`���ǂݍ���
						CManager::GetInstance()->GetInstance()->GetTexture()->Regist(str.data());
					}
					else if (strCode.starts_with(CODE_MODEL_FILENAME))
					{
						//�������e�̕�������
						strtok(strCode.data(), " =\n");

						//�^�u����
						std::string str = STRTOK_NEXT;
						std::erase_if(str, [](char c) {return c == '\t'; });

						//���f���ǂݍ���
						CXModel::Load(str.data());
					}
					break;
				case READSTAT_CAMERASET:	//pvp�J�������擾
					if (strCode.starts_with(CODE_END_CAMERASET))
					{
						//�J�����ݒ�
						InitSetCameraPos(g_readCamera.posV, g_readCamera.posR, g_counterReadCamera);
						readStat = READSTAT_NONE;
						g_counterReadCamera++;		//���Z
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//X���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posV.x = IS_Utility::fatof(pSprit);

						//Y���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posV.y = IS_Utility::fatof(pSprit);

						//Z���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posV.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_REF))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//X���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posR.x = IS_Utility::fatof(pSprit);

						//Y���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posR.y = IS_Utility::fatof(pSprit);

						//Z���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posR.z = IS_Utility::fatof(pSprit);
					}
					break;
				case READSTAT_LIGHTSET:		//���C�g���擾
					if (strCode.starts_with(CODE_END_LIGHTSET))
					{
						//���C�g�ݒ�
						SetLight(g_counterReadLight, g_readLight);
						readStat = READSTAT_NONE;

						g_counterReadLight++;
					}
					else if (strCode.starts_with(CODE_DIRECTION))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//���u���ϐ��錾
						D3DXVECTOR3 vecDir;

						//X���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						vecDir.x = IS_Utility::fatof(pSprit);

						//Y���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						vecDir.y = IS_Utility::fatof(pSprit);

						//Z���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						vecDir.z = IS_Utility::fatof(pSprit);

						//�x�N�g�����K��
						D3DXVec3Normalize(&vecDir, &vecDir);

						//���
						g_readLight.Direction = vecDir;
					}
					else if (strCode.starts_with(CODE_DIFFUSE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//��
						pSprit = strtok(nullptr, " =\n");
						g_readLight.Diffuse.r = IS_Utility::fatof(pSprit);

						//��
						pSprit = strtok(nullptr, " =\n");
						g_readLight.Diffuse.g = IS_Utility::fatof(pSprit);

						//��
						pSprit = strtok(nullptr, " =\n");
						g_readLight.Diffuse.b = IS_Utility::fatof(pSprit);

						//�A���t�@�l��1.0�Œ�
						g_readLight.Diffuse.a = 1.0f;
					}
					break;
				case READSTAT_SKYSET:		//����擾
					if (strCode.starts_with(CODE_END_SKYSET))
					{
						//��ݒ肵��
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�擾
						pSprit = strtok(nullptr, " =\n");
						g_readsky.texType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_MOVE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�擾
						pSprit = strtok(nullptr, " =\n");
						g_readsky.fMove = IS_Utility::fatof(pSprit);
					}
					break;
				case READSTAT_MOUNTAINSET:	//�R���擾
					if (strCode.starts_with(CODE_END_MOUNTAINSET))
					{
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						//�擾
						pSprit = strtok(nullptr, " =\n");
						g_nMountainTexNum = atoi(pSprit);
					}
					break;
				case READSTAT_FIELDSET:		//�����擾
					if (strCode.starts_with(CODE_END_FIELDSET))
					{
						//�n�ʐݒ�
						SetMeshField(g_readmeshfield.texType, g_readmeshfield.mf);
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�擾
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.texType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//X���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.pos.x = IS_Utility::fatof(pSprit);

						//Y���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.pos.y = IS_Utility::fatof(pSprit);

						//Z���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ROT))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//X���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.rot.x = IS_Utility::fatof(pSprit);

						//Y���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.rot.y = IS_Utility::fatof(pSprit);

						//Z���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.rot.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_SIZE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�T�C�Y�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.fRadius = IS_Utility::fatof(pSprit);
					}
					break;
				case READSTAT_WALLSET:		//�Ǐ��擾
					if (strCode.starts_with(CODE_END_WALLSET))
					{
						//�ǐ�������
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�擾
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.texType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�ʒu�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.pos.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.pos.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ROT))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�����ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.rot.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.rot.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.rot.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_BLOCK))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�������ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.blockX = atoi(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.blockZ = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_SIZE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�T�C�Y�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.sizeX = atoi(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.sizeZ = atoi(pSprit);
					}
					break;
				case READSTAT_MODELSET:		//���f�����擾
					if (strCode.starts_with(CODE_END_MODELSET))
					{
						CXModel* pModel = CXModel::GetTop();
						for (int cnt = 0; cnt < nType; cnt++)
						{
							pModel->GetNext();
						}
						CObjectX::Create(pos, rot, pModel);
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//��ޓǂݎ��
						pSprit = strtok(nullptr, " =\n");
						nType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//X���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						pos.x = IS_Utility::fatof(pSprit);

						//Y���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						pos.y = IS_Utility::fatof(pSprit);

						//Z���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ROT))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//X���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						rot.x = (IS_Utility::fatof(pSprit) / 180) * D3DX_PI;

						//Y���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						rot.y = (IS_Utility::fatof(pSprit) / 180) * D3DX_PI;

						//Z���W�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						rot.z = (IS_Utility::fatof(pSprit) / 180) * D3DX_PI;
					}
					else if (strCode.starts_with(CODE_STATE))
					{//��Ԏw��
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//��Ԑݒ�
						pSprit = strtok(nullptr, " =\n");
						nState = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_COLLISION))
					{//0�Ȃ瓖���蔻�薳��
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�����蔻��ݒ�
						pSprit = strtok(nullptr, " =\n");
						bCollision = (atoi(pSprit) >= 1) ? true : false;
					}
					else if (strCode.starts_with(CODE_SHADOW))
					{//0�Ȃ�e���g�p���Ȃ�
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						pSprit = strtok(nullptr, " =\n");
						bShadow = (atoi(pSprit) >= 1) ? true : false;
					}
					break;
				case READSTAT_BILLBOARDSET:	//�r���{�[�h���擾
					if (strCode.starts_with(CODE_END_BILLBOARDSET))
					{
						//�r���{�[�h�ݒ�
						CObjectBillboard::Create(pos, IS_Utility::VEC3_ZERO, fWidth, fHeight, nType);

						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//��ޓǂݎ��
						pSprit = strtok(nullptr, " =\n");
						nType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�ʒu�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						pos.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						pos.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_SIZE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�T�C�Y�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						fWidth = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						fWidth = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ORIGIN))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//���Ɏg���̂��킩��Ȃ����̓ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readbillboard.originX = atoi(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readbillboard.originZ = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_BLEND))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�������[�h�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readbillboard.blend = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_SHADOW))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�e�g�p�L���ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						bShadow = (atoi(pSprit) >= 1) ? true : false;
					}
					break;
				case READSTAT_PLAYERSET:	//�v���C���[���f�����擾
					if (strCode.starts_with(CODE_END_PLAYERSET))
					{
						//�v���C���[���f���擾����
						readStat = READSTAT_NONE;

						//���Z
						g_counterReadAnimal++;
					}
					else if (strCode.starts_with(CODE_MOTION_FILENAME))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�擾
						pSprit = strtok(nullptr, " =\n");
						strcpy(&g_readPlayermodel[g_counterReadAnimal].motionFileName[0], pSprit);

						//�^�u�����������Ă���Ƃ��������
						char* pCharPos = strchr(&g_readPlayermodel[g_counterReadAnimal].motionFileName[0], '/t');
						if (pCharPos != nullptr)
						{//strchr�̕Ԃ�l���ʂ�ۂł͂Ȃ�
							*pCharPos = '/0';
						}
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�ʒu�ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ROT))
					{
						pSprit = strtok(&aCode[0], " =\n");	//�������e�̕�������

						//�����ǂݎ��
						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].rot.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].rot.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].rot.z = IS_Utility::fatof(pSprit);
					}
					break;
				}
			}
		}
		fs.close();
	}
}
#endif

namespace TXTFileLoader
{
	//========================
	//���[�V�����ǂݍ��ݏ���
	//����1:���[�V�����r���[���[�̐ݒ�t�@�C���̃p�X
	//����2:���ꂽ�����f���\���̂̃|�C���^
	//========================
	void LoadMotionViewerFile(std::string path, CCharacterConfig::SCharacterConfig* pConfigData)
	{
		READSTAT readStat = READSTAT_STARTSCRIPTS;
		bool bRead = false;

		//�ǂݎ�������
		int nIdx = -1;								//PARTSSET:�g�p���郂�f��
		int nSetModelNum = 0;						//PARTSSET:�ݒ肵�����f����
		int nParent = -1;							//PARTSSET:�e
		D3DXVECTOR3 posOffset, rotOffset;			//PARTSSET:�I�t�Z�b�g�̈ʒu����

		CCharacterConfig::SParts* pParts = nullptr;
		CCharacterConfig::SMotion* pMotion = nullptr;
		CCharacterConfig::SKeySet* pKeySet = nullptr;
		CCharacterConfig::SKey* pKey = nullptr;

		CMotion::INFO info;
		int nKeyInfo = 0, nKey = 0;		//�L�[���E�L�[

		//�t�@�C���ǂݍ���
		std::fstream fs(path);
		if (fs.is_open())
		{
			std::string strCode;
			while (!fs.eof())
			{
				//1�s�ǂݎ��
				std::getline(fs, strCode);

				//�R�����g�A�E�g�`�F�b�N
				int commentCharPos = strCode.find_first_of("#", 0);
				if (commentCharPos != std::string::npos)
				{//�R�����g���������
					std::string::iterator itr = strCode.begin() + commentCharPos;
					strCode.erase(itr, strCode.end());
				}

				//�^�u����
				std::erase_if(strCode, [](char c) {return c == '\t'; });

				//������`�F�b�N
				if (strCode.starts_with(CODE_SCRIPT))
				{//�ǂݎ��J�n
					bRead = true;
				}
				else if (strCode.starts_with(CODE_END_SCRIPT))
				{//�ǂݎ��I��
					bRead = false;
					break;
				}
				else if (bRead == true)
				{//�ǂݎ��
					switch (readStat)
					{
					case READSTAT_STARTSCRIPTS:	//�����擾
						if (strCode.starts_with(CODE_NUM_MODEL))
						{
							//�ǂݍ��ݍ��ڂɂ�������Ⴀ�邯�ǎg���ĂȂ����疳��
						}
						if (strCode.starts_with(CODE_MODEL_FILENAME))
						{
							//�������e�̕�������
							strtok(strCode.data(), " =\n");

							//�^�u����
							std::string str = STRTOK_NEXT;
							std::erase_if(str, [](char c) {return c == '\t'; });

							//���
							pConfigData->filePath.emplace_back(str);
						}
						if (strCode.starts_with(CODE_CHARACTERSET))
						{
							readStat = READSTAT_CHARACTERSET;
						}
						if (strCode.starts_with(CODE_MOTIONSET))
						{
							pMotion = new CCharacterConfig::SMotion;
							readStat = READSTAT_MOTIONSET;
						}
						break;
					case READSTAT_CHARACTERSET:		//���f�����擾
						if (strCode.starts_with(CODE_END_CHARACTERSET))
						{
							//�ǂݍ��ݒ���Ԃɖ߂�
							readStat = READSTAT_STARTSCRIPTS;
						}
						else if (strCode.starts_with(CODE_PARTSSET))
						{
							pParts = new CCharacterConfig::SParts;
							readStat = READSTAT_PARTSSET;
						}
						else if (strCode.starts_with(CODE_MOVE))
						{
							//�ړ��ʎ擾
							pConfigData->charaHierarchy.move = TXTFileLoader::ReadFloat(strCode);
						}
						else if (strCode.starts_with(CODE_JUMP))
						{
							//�W�����v�ʎ擾
							pConfigData->charaHierarchy.jump = TXTFileLoader::ReadFloat(strCode);

						}
						else if (strCode.starts_with(CODE_RADIUS))
						{
							//���a�擾
							pConfigData->charaHierarchy.radius = TXTFileLoader::ReadFloat(strCode);

						}
						else if (strCode.starts_with(CODE_NUM_PARTS))
						{
							//�p�[�c���擾
							pConfigData->charaHierarchy.num_parts = TXTFileLoader::ReadInt(strCode);
						}
						break;
					case READSTAT_PARTSSET:
						if (strCode.starts_with(CODE_END_PARTSSET))
						{
							//�o�^���Ĕj��
							pConfigData->charaHierarchy.parts.emplace_back(*pParts);
							delete pParts;
							pParts = nullptr;

							//�L�����N�^�[�ݒ��Ԃɖ߂�
							readStat = READSTAT_CHARACTERSET;
						}
						else if (strCode.starts_with(CODE_INDEX))
						{
							//�ԍ��擾
							pParts->index = TXTFileLoader::ReadInt(strCode);
						}
						else if (strCode.starts_with(CODE_PARENT))
						{
							//�e�ԍ��擾
							pParts->parent = TXTFileLoader::ReadInt(strCode);
						}
						else if (strCode.starts_with(CODE_POS))
						{
							//���W�ǂݎ��
							pParts->pos = TXTFileLoader::ReadVec3(strCode);
						}
						else if (strCode.starts_with(CODE_ROT))
						{
							//�����ǂݎ��
							pParts->rot = TXTFileLoader::ReadVec3(strCode);
						}
						break;
					case READSTAT_MOTIONSET:
						if (strCode.starts_with(CODE_KEYSET))
						{
							pKeySet = new CCharacterConfig::SKeySet;
							readStat = READSTAT_KEYSET;
						}
						else if (strCode.starts_with(CODE_END_MOTIONSET))
						{
							//�o�^���Ĕj��
							pConfigData->motions.emplace_back(*pMotion);
							delete pMotion;
							pMotion = nullptr;

							//�ǂݍ��ݒ���Ԃɖ߂�
							readStat = READSTAT_STARTSCRIPTS;
						}
						else if (strCode.starts_with(CODE_LOOP))
						{
							//���[�v�ۓǂݎ��
							pMotion->isLoop = TXTFileLoader::ReadInt(strCode) == 1 ? true : false;
						}
						else if (strCode.starts_with(CODE_NUM_KEY))
						{
							//�L�[���ǂݎ��
							pMotion->num_key = TXTFileLoader::ReadInt(strCode);
						}
						break;
					case READSTAT_KEYSET:
						if (strCode.starts_with(CODE_KEY))
						{
							pKey = new CCharacterConfig::SKey;
							readStat = READSTAT_KEY;
						}
						else if (strCode.starts_with(CODE_END_KEYSET))
						{
							//�o�^���Ĕj��
							pMotion->keysets.emplace_back(*pKeySet);
							delete pKeySet;
							pKeySet = nullptr;

							//���[�V�����ݒ��Ԃɖ߂�
							readStat = READSTAT_MOTIONSET;
						}
						else if (strCode.starts_with(CODE_FRAME))
						{
							//�t���[�����ǂݎ��
							pKeySet->frame = TXTFileLoader::ReadInt(strCode);
						}
						break;
					case READSTAT_KEY:
						if (strCode.starts_with(CODE_END_KEY))
						{
							//�o�^���Ĕj��
							pKeySet->keys.emplace_back(*pKey);
							delete pKey;
							pKey = nullptr;

							//�L�[�Z�b�g��Ԃɖ߂�
							readStat = READSTAT_KEYSET;
						}
						else if (strCode.starts_with(CODE_POS))
						{
							//���W���
							pKey->pos = TXTFileLoader::ReadVec3(strCode);
						}
						else if (strCode.starts_with(CODE_ROT))
						{
							//�������
							pKey->rot = TXTFileLoader::ReadVec3(strCode);
						}
						break;
					}
				}
			}
			fs.close();
		}
	}

	//========================
	// Vec3�^�̓ǂݎ��
	//========================
	D3DXVECTOR3 ReadVec3(std::string str)
	{
		D3DXVECTOR3 value = IS_Utility::VEC3_ZERO;

		strtok(str.data(), " =\n");	//�������e�̕�������

		//�ǂݎ��
		value.x = IS_Utility::fatof(STRTOK_NEXT);
		value.y = IS_Utility::fatof(STRTOK_NEXT);
		value.z = IS_Utility::fatof(STRTOK_NEXT);

		return value;
	}

	//========================
	// int�^�̓ǂݎ��
	//========================
	int ReadInt(std::string str)
	{
		int value = 0;

		strtok(str.data(), " =\n");	//�������e�̕�������

		//�ǂݎ��
		value = atoi(STRTOK_NEXT);

		return value;
	}

	//========================
	// float�^�̓ǂݎ��
	//========================
	float ReadFloat(std::string str)
	{
		float value = 0.0f;

		strtok(str.data(), " =\n");	//�������e�̕�������

		//�ǂݎ��
		value = IS_Utility::fatof(STRTOK_NEXT);

		return value;
	}
}
