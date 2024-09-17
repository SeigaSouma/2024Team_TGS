//======================================================
//
//�����_�����O�����̃w�b�_[renderer.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

class CRenderer
{
public:
	//�u�����f�B���O��ޗ�
	typedef enum
	{
		BLENDTYPE_NORMAL = 0,	//�ʏ�
		BLENDTYPE_ADD,			//���Z	
		BLENDTYPE_SUB,			//���Z
		BLENDTYPE_MAX
	} BLENDTYPE;
	enum class DISPLAYMODE
	{
		MODE_WINDOW,
		MODE_FULLSCREEN,
	};

	//�R���X�g���N�^�E�f�X�g���N�^
	CRenderer();
	~CRenderer();

	//��{����
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ݒ�
	void SetViewport(float x, float y, float width, float height);
	void SetBlendType(BLENDTYPE type);
	void SetZEnable(const bool bEnable);
	void SetEnableZTest(bool bEnable);
	void SetEnableAlplaTest(bool bEnable);

	//�擾
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	void SwitchDisplayMode(DISPLAYMODE mode);

private:
	void ResetDevice();

	LPDIRECT3D9 m_pD3D;						//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Direct3D�f�o�C�X�ւ̃|�C���^
	DISPLAYMODE m_dispMode;					//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS m_d3dpp;			//�f�o�C�X���Z�b�g���Ɏg�p����v���[���e�[�V�����p�����[�^
	D3DPRESENT_PARAMETERS m_d3dppWindow;	//�f�o�C�X���Z�b�g���Ɏg�p����v���[���e�[�V�����p�����[�^
	D3DPRESENT_PARAMETERS m_d3dppFull;		//�f�o�C�X���Z�b�g���Ɏg�p����v���[���e�[�V�����p�����[�^
	bool m_bResetWait;						//�f�o�C�X���Z�b�g�҂����
	HWND m_hWnd;
};

#endif // !_RENDERER_H_