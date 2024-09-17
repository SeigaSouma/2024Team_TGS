//==========================================
//
//ファイル読み込みプログラム[file.cpp]
//Author:石原颯馬
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

//コード関係
#define STRTOK_NEXT		(strtok(nullptr, " =\n"))

#if 0
//========================
//マップ読み込み処理
//========================
void LoadModelViewerFile(std::string path)
{
	READSTAT readStat = READSTAT_STARTSCRIPTS;
	bool bRead = false;
	int nCntInit = 0;	//初期化カウンタ

	//読み込みデータ
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
			//1行読み取る
			std::getline(fs, strCode);

			//コメントアウトチェック
			int commentCharPos = strCode.find_first_of("#", 0);
			if (commentCharPos != std::string::npos)
			{//コメント文字あるよ
				std::string::iterator itr = strCode.begin() + commentCharPos;
				strCode.erase(itr, strCode.end());
			}

			//タブ消去
			std::erase_if(strCode, [](char c) {return c == '\t'; });

			//文字列チェック
			if (strCode.starts_with(CODE_SCRIPT))
			{//読み取り開始
				bRead = true;
			}
			else if (strCode.starts_with(CODE_END_SCRIPT))
			{//読み取り終了
				bRead = false;
				break;
			}
			else if (bRead == true)
			{//読み取り
				switch (readStat)
				{
				case READSTAT_STARTSCRIPTS:	//処理取得
					//処理部分
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
						//処理内容の部分消す
						strtok(strCode.data(), " =\n");

						//タブ消去
						std::string str = STRTOK_NEXT;
						std::erase_if(str, [](char c) {return c == '\t'; });

						//テクスチャ読み込み
						CManager::GetInstance()->GetInstance()->GetTexture()->Regist(str.data());
					}
					else if (strCode.starts_with(CODE_MODEL_FILENAME))
					{
						//処理内容の部分消す
						strtok(strCode.data(), " =\n");

						//タブ消去
						std::string str = STRTOK_NEXT;
						std::erase_if(str, [](char c) {return c == '\t'; });

						//モデル読み込み
						CXModel::Load(str.data());
					}
					break;
				case READSTAT_CAMERASET:	//pvpカメラ情報取得
					if (strCode.starts_with(CODE_END_CAMERASET))
					{
						//カメラ設定
						InitSetCameraPos(g_readCamera.posV, g_readCamera.posR, g_counterReadCamera);
						readStat = READSTAT_NONE;
						g_counterReadCamera++;		//加算
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posV.x = IS_Utility::fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posV.y = IS_Utility::fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posV.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_REF))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posR.x = IS_Utility::fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posR.y = IS_Utility::fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readCamera.posR.z = IS_Utility::fatof(pSprit);
					}
					break;
				case READSTAT_LIGHTSET:		//ライト情報取得
					if (strCode.starts_with(CODE_END_LIGHTSET))
					{
						//ライト設定
						SetLight(g_counterReadLight, g_readLight);
						readStat = READSTAT_NONE;

						g_counterReadLight++;
					}
					else if (strCode.starts_with(CODE_DIRECTION))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//仮置き変数宣言
						D3DXVECTOR3 vecDir;

						//X座標読み取り
						pSprit = strtok(nullptr, " =\n");
						vecDir.x = IS_Utility::fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(nullptr, " =\n");
						vecDir.y = IS_Utility::fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(nullptr, " =\n");
						vecDir.z = IS_Utility::fatof(pSprit);

						//ベクトル正規化
						D3DXVec3Normalize(&vecDir, &vecDir);

						//代入
						g_readLight.Direction = vecDir;
					}
					else if (strCode.starts_with(CODE_DIFFUSE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//赤
						pSprit = strtok(nullptr, " =\n");
						g_readLight.Diffuse.r = IS_Utility::fatof(pSprit);

						//緑
						pSprit = strtok(nullptr, " =\n");
						g_readLight.Diffuse.g = IS_Utility::fatof(pSprit);

						//青
						pSprit = strtok(nullptr, " =\n");
						g_readLight.Diffuse.b = IS_Utility::fatof(pSprit);

						//アルファ値は1.0固定
						g_readLight.Diffuse.a = 1.0f;
					}
					break;
				case READSTAT_SKYSET:		//空情報取得
					if (strCode.starts_with(CODE_END_SKYSET))
					{
						//空設定して
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(nullptr, " =\n");
						g_readsky.texType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_MOVE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(nullptr, " =\n");
						g_readsky.fMove = IS_Utility::fatof(pSprit);
					}
					break;
				case READSTAT_MOUNTAINSET:	//山情報取得
					if (strCode.starts_with(CODE_END_MOUNTAINSET))
					{
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						//取得
						pSprit = strtok(nullptr, " =\n");
						g_nMountainTexNum = atoi(pSprit);
					}
					break;
				case READSTAT_FIELDSET:		//床情報取得
					if (strCode.starts_with(CODE_END_FIELDSET))
					{
						//地面設定
						SetMeshField(g_readmeshfield.texType, g_readmeshfield.mf);
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.texType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.pos.x = IS_Utility::fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.pos.y = IS_Utility::fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ROT))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.rot.x = IS_Utility::fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.rot.y = IS_Utility::fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.rot.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_SIZE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//サイズ読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readmeshfield.mf.fRadius = IS_Utility::fatof(pSprit);
					}
					break;
				case READSTAT_WALLSET:		//壁情報取得
					if (strCode.starts_with(CODE_END_WALLSET))
					{
						//壁生成して
						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.texType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//位置読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.pos.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.pos.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ROT))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//向き読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.rot.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.rot.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.rot.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_BLOCK))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//分割数読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.blockX = atoi(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.blockZ = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_SIZE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//サイズ読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.sizeX = atoi(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readMeshWall.sizeZ = atoi(pSprit);
					}
					break;
				case READSTAT_MODELSET:		//モデル情報取得
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
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//種類読み取り
						pSprit = strtok(nullptr, " =\n");
						nType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(nullptr, " =\n");
						pos.x = IS_Utility::fatof(pSprit);

						//Y座標読み取り
						pSprit = strtok(nullptr, " =\n");
						pos.y = IS_Utility::fatof(pSprit);

						//Z座標読み取り
						pSprit = strtok(nullptr, " =\n");
						pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ROT))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//X座標読み取り
						pSprit = strtok(nullptr, " =\n");
						rot.x = (IS_Utility::fatof(pSprit) / 180) * D3DX_PI;

						//Y座標読み取り
						pSprit = strtok(nullptr, " =\n");
						rot.y = (IS_Utility::fatof(pSprit) / 180) * D3DX_PI;

						//Z座標読み取り
						pSprit = strtok(nullptr, " =\n");
						rot.z = (IS_Utility::fatof(pSprit) / 180) * D3DX_PI;
					}
					else if (strCode.starts_with(CODE_STATE))
					{//状態指定
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//状態設定
						pSprit = strtok(nullptr, " =\n");
						nState = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_COLLISION))
					{//0なら当たり判定無効
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//当たり判定設定
						pSprit = strtok(nullptr, " =\n");
						bCollision = (atoi(pSprit) >= 1) ? true : false;
					}
					else if (strCode.starts_with(CODE_SHADOW))
					{//0なら影を使用しない
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						pSprit = strtok(nullptr, " =\n");
						bShadow = (atoi(pSprit) >= 1) ? true : false;
					}
					break;
				case READSTAT_BILLBOARDSET:	//ビルボード情報取得
					if (strCode.starts_with(CODE_END_BILLBOARDSET))
					{
						//ビルボード設定
						CObjectBillboard::Create(pos, IS_Utility::VEC3_ZERO, fWidth, fHeight, nType);

						readStat = READSTAT_NONE;
					}
					else if (strCode.starts_with(CODE_TEXTYPE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//種類読み取り
						pSprit = strtok(nullptr, " =\n");
						nType = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_POS, sizeof CODE_POS / sizeof(char) - 1) == 0)
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//位置読み取り
						pSprit = strtok(nullptr, " =\n");
						pos.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						pos.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_SIZE))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//サイズ読み取り
						pSprit = strtok(nullptr, " =\n");
						fWidth = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						fWidth = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ORIGIN))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//何に使うのかわからないもの読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readbillboard.originX = atoi(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readbillboard.originZ = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_BLEND))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//合成モード読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readbillboard.blend = atoi(pSprit);
					}
					else if (strCode.starts_with(CODE_SHADOW))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//影使用有無読み取り
						pSprit = strtok(nullptr, " =\n");
						bShadow = (atoi(pSprit) >= 1) ? true : false;
					}
					break;
				case READSTAT_PLAYERSET:	//プレイヤーモデル情報取得
					if (strCode.starts_with(CODE_END_PLAYERSET))
					{
						//プレイヤーモデル取得完了
						readStat = READSTAT_NONE;

						//加算
						g_counterReadAnimal++;
					}
					else if (strCode.starts_with(CODE_MOTION_FILENAME))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//取得
						pSprit = strtok(nullptr, " =\n");
						strcpy(&g_readPlayermodel[g_counterReadAnimal].motionFileName[0], pSprit);

						//タブ文字が入っているところを消す
						char* pCharPos = strchr(&g_readPlayermodel[g_counterReadAnimal].motionFileName[0], '/t');
						if (pCharPos != nullptr)
						{//strchrの返り値がぬるぽではない
							*pCharPos = '/0';
						}
					}
					else if (strCode.starts_with(CODE_POS))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//位置読み取り
						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.x = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.y = IS_Utility::fatof(pSprit);

						pSprit = strtok(nullptr, " =\n");
						g_readPlayermodel[g_counterReadAnimal].pos.z = IS_Utility::fatof(pSprit);
					}
					else if (strCode.starts_with(CODE_ROT))
					{
						pSprit = strtok(&aCode[0], " =\n");	//処理内容の部分消す

						//向き読み取り
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
	//モーション読み込み処理
	//引数1:モーションビューワーの設定ファイルのパス
	//引数2:入れたいモデル構造体のポインタ
	//========================
	void LoadMotionViewerFile(std::string path, CCharacterConfig::SCharacterConfig* pConfigData)
	{
		READSTAT readStat = READSTAT_STARTSCRIPTS;
		bool bRead = false;

		//読み取った情報
		int nIdx = -1;								//PARTSSET:使用するモデル
		int nSetModelNum = 0;						//PARTSSET:設定したモデル数
		int nParent = -1;							//PARTSSET:親
		D3DXVECTOR3 posOffset, rotOffset;			//PARTSSET:オフセットの位置向き

		CCharacterConfig::SParts* pParts = nullptr;
		CCharacterConfig::SMotion* pMotion = nullptr;
		CCharacterConfig::SKeySet* pKeySet = nullptr;
		CCharacterConfig::SKey* pKey = nullptr;

		CMotion::INFO info;
		int nKeyInfo = 0, nKey = 0;		//キー情報・キー

		//ファイル読み込み
		std::fstream fs(path);
		if (fs.is_open())
		{
			std::string strCode;
			while (!fs.eof())
			{
				//1行読み取る
				std::getline(fs, strCode);

				//コメントアウトチェック
				int commentCharPos = strCode.find_first_of("#", 0);
				if (commentCharPos != std::string::npos)
				{//コメント文字あるよ
					std::string::iterator itr = strCode.begin() + commentCharPos;
					strCode.erase(itr, strCode.end());
				}

				//タブ消去
				std::erase_if(strCode, [](char c) {return c == '\t'; });

				//文字列チェック
				if (strCode.starts_with(CODE_SCRIPT))
				{//読み取り開始
					bRead = true;
				}
				else if (strCode.starts_with(CODE_END_SCRIPT))
				{//読み取り終了
					bRead = false;
					break;
				}
				else if (bRead == true)
				{//読み取り
					switch (readStat)
					{
					case READSTAT_STARTSCRIPTS:	//処理取得
						if (strCode.starts_with(CODE_NUM_MODEL))
						{
							//読み込み項目にあるっちゃあるけど使ってないから無視
						}
						if (strCode.starts_with(CODE_MODEL_FILENAME))
						{
							//処理内容の部分消す
							strtok(strCode.data(), " =\n");

							//タブ消去
							std::string str = STRTOK_NEXT;
							std::erase_if(str, [](char c) {return c == '\t'; });

							//代入
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
					case READSTAT_CHARACTERSET:		//モデル情報取得
						if (strCode.starts_with(CODE_END_CHARACTERSET))
						{
							//読み込み中状態に戻る
							readStat = READSTAT_STARTSCRIPTS;
						}
						else if (strCode.starts_with(CODE_PARTSSET))
						{
							pParts = new CCharacterConfig::SParts;
							readStat = READSTAT_PARTSSET;
						}
						else if (strCode.starts_with(CODE_MOVE))
						{
							//移動量取得
							pConfigData->charaHierarchy.move = TXTFileLoader::ReadFloat(strCode);
						}
						else if (strCode.starts_with(CODE_JUMP))
						{
							//ジャンプ量取得
							pConfigData->charaHierarchy.jump = TXTFileLoader::ReadFloat(strCode);

						}
						else if (strCode.starts_with(CODE_RADIUS))
						{
							//半径取得
							pConfigData->charaHierarchy.radius = TXTFileLoader::ReadFloat(strCode);

						}
						else if (strCode.starts_with(CODE_NUM_PARTS))
						{
							//パーツ数取得
							pConfigData->charaHierarchy.num_parts = TXTFileLoader::ReadInt(strCode);
						}
						break;
					case READSTAT_PARTSSET:
						if (strCode.starts_with(CODE_END_PARTSSET))
						{
							//登録して破棄
							pConfigData->charaHierarchy.parts.emplace_back(*pParts);
							delete pParts;
							pParts = nullptr;

							//キャラクター設定状態に戻る
							readStat = READSTAT_CHARACTERSET;
						}
						else if (strCode.starts_with(CODE_INDEX))
						{
							//番号取得
							pParts->index = TXTFileLoader::ReadInt(strCode);
						}
						else if (strCode.starts_with(CODE_PARENT))
						{
							//親番号取得
							pParts->parent = TXTFileLoader::ReadInt(strCode);
						}
						else if (strCode.starts_with(CODE_POS))
						{
							//座標読み取り
							pParts->pos = TXTFileLoader::ReadVec3(strCode);
						}
						else if (strCode.starts_with(CODE_ROT))
						{
							//向き読み取り
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
							//登録して破棄
							pConfigData->motions.emplace_back(*pMotion);
							delete pMotion;
							pMotion = nullptr;

							//読み込み中状態に戻る
							readStat = READSTAT_STARTSCRIPTS;
						}
						else if (strCode.starts_with(CODE_LOOP))
						{
							//ループ可否読み取り
							pMotion->isLoop = TXTFileLoader::ReadInt(strCode) == 1 ? true : false;
						}
						else if (strCode.starts_with(CODE_NUM_KEY))
						{
							//キー数読み取り
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
							//登録して破棄
							pMotion->keysets.emplace_back(*pKeySet);
							delete pKeySet;
							pKeySet = nullptr;

							//モーション設定状態に戻る
							readStat = READSTAT_MOTIONSET;
						}
						else if (strCode.starts_with(CODE_FRAME))
						{
							//フレーム数読み取り
							pKeySet->frame = TXTFileLoader::ReadInt(strCode);
						}
						break;
					case READSTAT_KEY:
						if (strCode.starts_with(CODE_END_KEY))
						{
							//登録して破棄
							pKeySet->keys.emplace_back(*pKey);
							delete pKey;
							pKey = nullptr;

							//キーセット状態に戻る
							readStat = READSTAT_KEYSET;
						}
						else if (strCode.starts_with(CODE_POS))
						{
							//座標代入
							pKey->pos = TXTFileLoader::ReadVec3(strCode);
						}
						else if (strCode.starts_with(CODE_ROT))
						{
							//向き代入
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
	// Vec3型の読み取り
	//========================
	D3DXVECTOR3 ReadVec3(std::string str)
	{
		D3DXVECTOR3 value = IS_Utility::VEC3_ZERO;

		strtok(str.data(), " =\n");	//処理内容の部分消す

		//読み取り
		value.x = IS_Utility::fatof(STRTOK_NEXT);
		value.y = IS_Utility::fatof(STRTOK_NEXT);
		value.z = IS_Utility::fatof(STRTOK_NEXT);

		return value;
	}

	//========================
	// int型の読み取り
	//========================
	int ReadInt(std::string str)
	{
		int value = 0;

		strtok(str.data(), " =\n");	//処理内容の部分消す

		//読み取り
		value = atoi(STRTOK_NEXT);

		return value;
	}

	//========================
	// float型の読み取り
	//========================
	float ReadFloat(std::string str)
	{
		float value = 0.0f;

		strtok(str.data(), " =\n");	//処理内容の部分消す

		//読み取り
		value = IS_Utility::fatof(STRTOK_NEXT);

		return value;
	}
}
