//=============================================================================
//
// �L�[�R���t�B�O�ݒ菈�� [keyconfig_setting.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "keyconfig_setting.h"
#include "keyconfig.h"
#include "keyconfig_gamepad.h"
#include "keyconfig_keyboard.h"
#include "controlkeydisp.h"
#include "input_gamepad.h"
#include "texture.h"

//==========================================================================
// �萔��`
//==========================================================================
namespace
{
	const MyLib::Vector3 DEFAULT_POS = MyLib::Vector3(640.0f, -50.0f, 0.0f);	// ��{���W
	const float DOWN_POSY = 100.0f;
	const float KEY_SIZE = 50.0f;
	const float FONT_WIDTH = 150.0f;
	const D3DXCOLOR FRONT_COL = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.5f);
	const float ADD_ALPHA = (1.0f / 120.0f);
}

// �t�@�C����
namespace FILENAME
{	
	const std::string CONFIG[INGAME::ACTION::ACT_MAX] =
	{
		"data\\TEXTURE\\ui_setting\\decide.png",
		"data\\TEXTURE\\ui_setting\\back.png",
		"data\\TEXTURE\\ui_setting\\updown.png",
		"data\\TEXTURE\\ui_setting\\air.png",
		"data\\TEXTURE\\ui_setting\\checkpoint.png",
		"data\\TEXTURE\\ui_setting\\retry.png",
		"data\\TEXTURE\\ui_setting\\pause.png",
	};
}

//==========================================================================
// �ÓI�����o�ϐ��錾
//==========================================================================

//==========================================================================
// �R���X�g���N�^
//==========================================================================
CKeyConfigSetting::CKeyConfigSetting()
{
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		m_aKeyConfig[i].s_p2D = nullptr;
		m_aKeyConfig[i].s_pKeyDisp = nullptr;
		m_aKeyConfig[i].s_p2DFront = nullptr;
	}

	m_checkconfig.s_p2D = nullptr;
	m_checkconfig.s_pKeyDispOK = nullptr;
	m_checkconfig.s_pKeyDispNO = nullptr;
	m_checkconfig.s_p2DFront = nullptr;

	m_bNowChange = false;
	m_SelectKey = 0;
	m_AlphaSin = 0.0f;
}

//==========================================================================
// ��������
//==========================================================================
CKeyConfigSetting* CKeyConfigSetting::Create()
{
	CKeyConfigSetting* pKeyConfig = new CKeyConfigSetting;

	if (pKeyConfig != nullptr)
	{
		pKeyConfig->Init();
	}

	return pKeyConfig;
}

//==========================================================================
// ����������
//==========================================================================
HRESULT CKeyConfigSetting::Init()
{
	CKeyConfig* pConfigPad = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);
	CTexture* pTexture = CTexture::GetInstance();

	// �|���S���𐶐�����
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// �L�[�𐶐�
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += i * DOWN_POSY;
		pos.x += FONT_WIDTH;
		m_aKeyConfig[i].s_pKeyDisp = CControlKeyDisp::Create(pos, 0.0f, KEY_SIZE, KEY_SIZE, pConfigPad->GetKey(i));

		// �����𐶐�
		pos.x = DEFAULT_POS.x - FONT_WIDTH;
		m_aKeyConfig[i].s_p2D = CObject2D::Create(11);
		m_aKeyConfig[i].s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_aKeyConfig[i].s_p2D->SetPosition(pos);
		m_aKeyConfig[i].s_p2D->BindTexture(pTexture->Regist(FILENAME::CONFIG[i]));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_aKeyConfig[i].s_p2D->GetIdxTexture());
		m_aKeyConfig[i].s_p2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE));

		// �Â�����p�̃|���S������
		pos.x = DEFAULT_POS.x;
		m_aKeyConfig[i].s_p2DFront = CObject2D::Create(11);
		m_aKeyConfig[i].s_p2DFront->SetPosition(pos);
		m_aKeyConfig[i].s_p2DFront->SetSize(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_aKeyConfig[i].s_p2DFront->SetColor(FRONT_COL);

		if (i <= INGAME::ACT_BACK)
		{
			m_aKeyConfig[i].s_pKeyDisp->SetEnableDisp(false);
			m_aKeyConfig[i].s_p2D->SetEnableDisp(false);
			m_aKeyConfig[i].s_p2DFront->SetEnableDisp(false);
		}

	}

	// ����߂�ύX�p�|���S������
	{
		MyLib::Vector3 pos = DEFAULT_POS;
		pos.y += DOWN_POSY * INGAME::ACTION::ACT_MAX;

		// �����̐���
		m_checkconfig.s_p2D = CObject2D::Create(11);
		m_checkconfig.s_p2D->SetType(CObject::TYPE_OBJECT2D);
		m_checkconfig.s_p2D->SetPosition(pos);
		m_checkconfig.s_p2D->BindTexture(pTexture->Regist("data\\TEXTURE\\ui_setting\\change.png"));
		D3DXVECTOR2 texture = pTexture->GetImageSize(m_checkconfig.s_p2D->GetIdxTexture());
		m_checkconfig.s_p2D->SetSize(UtilFunc::Transformation::AdjustSizeByHeight(texture, KEY_SIZE * 0.8f));

		// �L�[�R���t�B�O�𐶐�
		float size = KEY_SIZE * 0.75f;
		pos.x += (size * 3);
		m_checkconfig.s_pKeyDispOK = CControlKeyDisp::Create(pos, 0.0f, size, size,
			pConfigPad->GetKey(INGAME::ACT_OK));
		pos.x += (size * 3);
		m_checkconfig.s_pKeyDispNO = CControlKeyDisp::Create(pos, 0.0f, size, size,
			pConfigPad->GetKey(INGAME::ACT_BACK));

		// �Â�����p�̃|���S������
		pos = DEFAULT_POS;
		pos.y += DOWN_POSY * INGAME::ACTION::ACT_MAX;
		m_checkconfig.s_p2DFront = CObject2D::Create(11);
		m_checkconfig.s_p2DFront->SetPosition(pos);
		m_checkconfig.s_p2DFront->SetSize(D3DXVECTOR2(KEY_SIZE * 6, KEY_SIZE));
		m_checkconfig.s_p2DFront->SetColor(FRONT_COL);
	}

	m_SelectKey = INGAME::ACT_BACK + 1;

	return S_OK;
}

//==========================================================================
// �I������
//==========================================================================
void CKeyConfigSetting::Uninit()
{
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// 2D�|���S���̏I��
		if (m_aKeyConfig[i].s_p2D != nullptr)
		{
			m_aKeyConfig[i].s_p2D->Uninit();
			m_aKeyConfig[i].s_p2D = nullptr;
		}

		// 2D�|���S���̏I��
		if (m_aKeyConfig[i].s_p2DFront != nullptr)
		{ 
			m_aKeyConfig[i].s_p2DFront->Uninit();
			m_aKeyConfig[i].s_p2DFront = nullptr;
		}

		// �L�[�R���t�B�O�\���̏I��
		if (m_aKeyConfig[i].s_pKeyDisp != nullptr)
		{
			m_aKeyConfig[i].s_pKeyDisp->Uninit();
			m_aKeyConfig[i].s_pKeyDisp = nullptr;
		}
	}

	{
		// 2D�|���S���̏I��
		if (m_checkconfig.s_p2D != nullptr)
		{
			m_checkconfig.s_p2D->Uninit();
			m_checkconfig.s_p2D = nullptr;
		}

		// 2D�|���S���̏I��
		if (m_checkconfig.s_p2DFront != nullptr)
		{
			m_checkconfig.s_p2DFront->Uninit();
			m_checkconfig.s_p2DFront = nullptr;
		}

		// �L�[�R���t�B�O�\���̏I��
		if (m_checkconfig.s_pKeyDispOK != nullptr)
		{
			m_checkconfig.s_pKeyDispOK->Uninit();
			m_checkconfig.s_pKeyDispOK = nullptr;
		}

		if (m_checkconfig.s_pKeyDispNO != nullptr)
		{
			m_checkconfig.s_pKeyDispNO->Uninit();
			m_checkconfig.s_pKeyDispNO = nullptr;
		}
	}

	delete this;
}

//==========================================================================
// �X�V����
//==========================================================================
void CKeyConfigSetting::Update()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	CKeyConfig* pKeyConfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	if (m_bNowChange) { return; }

	// ���͑҂���Ԃɂ���
	if (pKeyConfig->GetTrigger(INGAME::ACT_OK))
	{
		if (m_SelectKey == INGAME::ACTION::ACT_MAX)
		{
			// �L�[���擾
			int oktype = pKeyConfig->GetKey(INGAME::ACT_OK);
			int backtype = pKeyConfig->GetKey(INGAME::ACT_BACK);

			// �L�[�̓���ւ�
			pKeyConfig->Join(INGAME::ACT_OK, backtype);
			pKeyConfig->Join(INGAME::ACT_BACK, oktype);

			// �e�N�X�`����ނ�ݒ�
			m_aKeyConfig[INGAME::ACT_OK].s_pKeyDisp->SetType(backtype);
			m_aKeyConfig[INGAME::ACT_BACK].s_pKeyDisp->SetType(oktype);
			m_checkconfig.s_pKeyDispOK->SetType(backtype);
			m_checkconfig.s_pKeyDispNO->SetType(oktype);

			return;
		}
		// ����L�[�Ɩ߂�L�[�ȊO
		else if (m_SelectKey != INGAME::ACT_OK && m_SelectKey != INGAME::ACT_BACK)
		{
			// �ύX��Ԃ�
			m_bNowChange = true;

			// �}���`�X���b�h�œ��͎�t
			std::thread th(&CKeyConfigSetting::Chenge, this);
			th.detach();

			return;
		}
	}

	// �I���ꏊ�ύX
	if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_UP, 0))
	{
		// ��Ɉړ�
		m_SelectKey = (m_SelectKey + INGAME::ACTION::ACT_MAX - 1) % INGAME::ACTION::ACT_MAX;
		
		if (m_SelectKey < 2)
		{
			m_SelectKey = INGAME::ACTION::ACT_MAX;
		}

		m_AlphaSin = 0.0f;
	}
	else if (pPad->GetTrigger(CInputGamepad::BUTTON::BUTTON_DOWN, 0))
	{
		// ���Ɉړ�
		m_SelectKey = (m_SelectKey + 1) % (INGAME::ACTION::ACT_MAX + 1);
		m_SelectKey = UtilFunc::Transformation::Clamp(m_SelectKey, 2, static_cast<int>(INGAME::ACTION::ACT_MAX));
		m_AlphaSin = 0.0f;
	}

	m_AlphaSin += ADD_ALPHA;

	// �|���S���𐶐�����
	for (int i = 0; i < INGAME::ACTION::ACT_MAX; i++)
	{
		// �g�p����Ă���ꍇ
		if (m_aKeyConfig[i].s_p2DFront == nullptr) { continue; }

		D3DXCOLOR col = FRONT_COL;

		if (i != INGAME::ACT_OK && i != INGAME::ACT_BACK)
		{
			if (m_SelectKey == i)
			{
				col.a = sinf(m_AlphaSin);
				col.a = fabsf(col.a);
				col.a *= FRONT_COL.a;

				m_aKeyConfig[i].s_p2DFront->SetColor(col);
				m_aKeyConfig[i].s_p2DFront->SetVtx();

				continue;
			}
		}

		m_aKeyConfig[i].s_p2DFront->SetColor(col);
		m_aKeyConfig[i].s_p2DFront->SetVtx();
	}

	// �؂�ւ��̐F�ύX
	{
		D3DXCOLOR col = FRONT_COL;

		if (m_SelectKey == INGAME::ACT_MAX)
		{
			col.a = sinf(m_AlphaSin);
			col.a = fabsf(col.a);
			col.a *= FRONT_COL.a;
		}

		m_checkconfig.s_p2DFront->SetColor(col);
		m_checkconfig.s_p2DFront->SetVtx();
	}
}

//==========================================================================
// �ύX
//==========================================================================
void CKeyConfigSetting::Chenge()
{
	CKeyConfig* pKeyConfig = CKeyConfigManager::GetInstance()->GetConfig(CKeyConfigManager::CONTROL_INPAD);

	while (1)
	{
		if (pKeyConfig->GetRelease(INGAME::ACT_OK))
		{
			break;
		}
	}

	// �ύX�O��ۑ�
	int oldkey = pKeyConfig->GetKey(m_SelectKey);

	// �ύX
	pKeyConfig->Setting(m_SelectKey);

	if (m_aKeyConfig[m_SelectKey].s_pKeyDisp != nullptr)
	{
		m_aKeyConfig[m_SelectKey].s_pKeyDisp->SetType(pKeyConfig->GetKey(m_SelectKey));
	}

	// �ύX���ۑ�
	int selectkey = pKeyConfig->GetKey(m_SelectKey);

	// �����m�F����	
	for (int i = 0; i < INGAME::ACT_MAX; i++)
	{
		// �I��
		if (m_SelectKey == i) { continue; }

		// ����߂�̏ꍇ
		if (i <= INGAME::ACT_BACK) { continue; }

		// �L�[�擾
		int key = pKeyConfig->GetKey(i);

		// �����L�[�ɂ��Ă��܂��Ă���ꍇ
		if (selectkey == key)
		{
			// �ύX�O�̂Ɠ���ւ���
			pKeyConfig->Join(i, oldkey);
			m_aKeyConfig[i].s_pKeyDisp->SetType(pKeyConfig->GetKey(i));
		}
	}

	while (1)
	{
		if (!pKeyConfig->GetPress(m_SelectKey))
		{
			break;
		}
	}

	m_bNowChange = false;
}