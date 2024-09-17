//======================================================
//
//�L�����N�^�[����[character.cpp]
//Author:�Ό��D�n
//
//======================================================
#include "character.h"
#include "..\manager\manager.h"
#include "..\renderer\renderer.h"
#include "..\file(legacy)\file.h"
#include "..\xmodel\xmodel.h"
#include "characterconfig.h"
#include "charaparts/charaparts.h"
#include "charamotion/motion.h"
#include "../utility/Utility.h"

//�ÓI�����o�ϐ�
CCharacter* CCharacter::m_pTop = nullptr;
CCharacter* CCharacter::m_pCur = nullptr;
int CCharacter::m_nNumAll = 0;

//=================================
//�R���X�g���N�^
//=================================
CCharacter::CCharacter()
{
	m_ppCharaParts = nullptr;
	m_nNumParts = 0;
	m_pMotion = nullptr;
	m_pos = IS_Utility::VEC3_ZERO;
	m_rot = IS_Utility::VEC3_ZERO;

	if (m_pCur == nullptr)
	{//�Ō�������Ȃ��i���Ȃ킿�擪�����Ȃ��j
		m_pTop = this;		//�����擪
		m_pPrev = nullptr;		//�O��N�����Ȃ�
		m_pNext = nullptr;
	}
	else
	{//�Ō��������
		m_pPrev = m_pCur;		//�Ō���������̑O�̃I�u�W�F
		m_pCur->m_pNext = this;	//�Ō���̎��̃I�u�W�F������
		m_pNext = nullptr;			//�����̎��̃I�u�W�F�͂��Ȃ�
	}
	m_pCur = this;				//�����Ō��
	m_bExclusion = false;		//�����Ă�
	m_nNumAll++;
}

//=================================
//�f�X�g���N�^
//=================================
CCharacter::~CCharacter()
{
}

//========================
//����������
//========================
HRESULT CCharacter::Init(void)
{
	return S_OK;
}

//========================
//�I������
//========================
void CCharacter::Uninit(void)
{
	//���[�V�����j��
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	if (m_ppCharaParts != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumParts; cnt++)
		{//�������
			if (m_ppCharaParts[cnt] != nullptr)
			{
				m_ppCharaParts[cnt]->Uninit();
				m_ppCharaParts[cnt] = nullptr;
			}
		}
		delete[] m_ppCharaParts;	//�z�񂻂̂��̂�j��
	}

	m_bExclusion = true;		//���O�\��

	//�������g�j��
	Release();
}

//========================
//�X�V����
//========================
void CCharacter::Update(void)
{
	//���f���X�V
	if (m_ppCharaParts != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumParts; cnt++)
		{
			if (m_ppCharaParts[cnt] != nullptr)
			{
				m_ppCharaParts[cnt]->Update();
			}
		}
	}
}

//========================
//�`�揈��
//========================
void CCharacter::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//�f�o�C�X�擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p

	//���݂̃}�e���A���擾
	pDevice->GetMaterial(&matDef);

	//���f���擾
	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu���f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���`��
	if (m_ppCharaParts != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumParts; cnt++)
		{
			if (m_ppCharaParts[cnt] != nullptr)
			{
				m_ppCharaParts[cnt]->Draw();
			}
		}
	}

	//�}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=================================
//�L�����ݒ�
//=================================
void CCharacter::SetChara(CCharacterConfig::SCharacterConfig &charaConfig)
{
	//�L�����p�[�c���f���̓ǂݍ���
	CXModel** ppLoadedModel = new CXModel*[charaConfig.filePath.size()];
	for (int cnt = 0; cnt < charaConfig.filePath.size(); cnt++)
	{
		ppLoadedModel[cnt] = CXModel::Load(charaConfig.filePath[cnt].data());
	}

	//�L�����p�[�c�̐���
	m_nNumParts = charaConfig.charaHierarchy.num_parts;
	m_ppCharaParts = new CCharaParts*[m_nNumParts];
	for (int cnt = 0; cnt < m_nNumParts; cnt++)
	{
		//�g�p����f�[�^
		int partsIdx = charaConfig.charaHierarchy.parts[cnt].index;
		D3DXVECTOR3 posOffset = charaConfig.charaHierarchy.parts[cnt].pos;
		D3DXVECTOR3 rotOffset = charaConfig.charaHierarchy.parts[cnt].rot;

		//����
		m_ppCharaParts[cnt] = CCharaParts::Create(posOffset, rotOffset, ppLoadedModel[partsIdx]);
	}

	//�e�q�ݒ�
	for (int cnt = 0; cnt < m_nNumParts; cnt++)
	{
		int parent = charaConfig.charaHierarchy.parts[cnt].parent;
		if (parent != -1)
		{//�e������
			m_ppCharaParts[cnt]->SetParent(m_ppCharaParts[parent]);
		}
		else
		{//�e�����Ȃ�
			m_ppCharaParts[cnt]->SetParent(nullptr);
		}
	}

	//���[�V�����Ƀ��f����ݒ�
	m_pMotion = new CMotion;
	m_pMotion->Init();
	m_pMotion->SetModel(m_ppCharaParts, m_nNumParts);

	//���[�V�����̐ݒ�
	for (int cnt = 0; cnt < charaConfig.motions.size(); cnt++)
	{
		//�o�^���[�V�����\����
		CMotion::INFO info;

		//�f�[�^���
		info.m_bLoop = charaConfig.motions[cnt].isLoop;
		info.m_nNumKey = charaConfig.motions[cnt].num_key;
		info.m_pKeyInfo = new CMotion::KEY_INFO[info.m_nNumKey];
		for (int cntKeySet = 0; cntKeySet < info.m_nNumKey; cntKeySet++)
		{
			info.m_pKeyInfo[cntKeySet].m_nFrame = charaConfig.motions[cnt].keysets[cntKeySet].frame;
			info.m_pKeyInfo[cntKeySet].m_pKey = new CMotion::KEY[m_nNumParts];
			for (int cntKey = 0; cntKey < charaConfig.motions[cnt].keysets[cntKeySet].keys.size(); cntKey++)
			{
				info.m_pKeyInfo[cntKeySet].m_pKey[cntKey].pos = charaConfig.motions[cnt].keysets[cntKeySet].keys[cntKey].pos;
				info.m_pKeyInfo[cntKeySet].m_pKey[cntKey].rot = charaConfig.motions[cnt].keysets[cntKeySet].keys[cntKey].rot;
			}
		}

		//���[�V�����o�^
		m_pMotion->SetInfo(info);
	}

	//�ǂݍ��݂̎��ɓ��I�m�ۂ������̂�j��
	delete[] ppLoadedModel;
}

//=================================
//����
//=================================
CCharacter* CCharacter::Create(CCharacterConfig::SCharacterConfig &charaConfig, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CCharacter* pObj = nullptr;

	if (pObj == nullptr)
	{
		//�I�u�W�F�N�g�̐���
		pObj = new CCharacter;

		//������
		pObj->Init();

		//�ݒ�
		pObj->SetPos(pos);
		pObj->SetRot(rot);
		pObj->SetChara(charaConfig);

		return pObj;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//�����i�p�X�w��j
//=================================
CCharacter* CCharacter::Create(std::string path, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CCharacterConfig::SCharacterConfig charaConfig = CCharacterConfig::Load(path)->GetConfig();
	return CCharacter::Create(charaConfig, pos, rot);
}
