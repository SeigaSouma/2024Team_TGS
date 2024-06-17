//=============================================================================
// 
//  �����_���[�w�b�_�[ [renderer.h]
//  Author : ���n�Ή�
// 
//=============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_	// ��d�C���N���[�h�h�~

#include "main.h"

//==========================================================================
// �N���X��`
//==========================================================================
// �����_���[�N���X��`
class CRenderer
{
private:

	// �}���`�^�[�Q�b�g�����_�����O�p���
	struct MultiTargetInfo
	{
		float fStartColAlpha;	// �J�n�����x
		float fStartMulti;		// �J�n�T�C�Y�{��
		float fAddTimer;		// �^�C�}�[������
		float fColAlpha;		// �����x
		float fTimer;			// �J�ڃ^�C�}�[
		float fMulti;			// �T�C�Y�{��
		bool bActive;			// �ғ���

		MultiTargetInfo()	// �R���X�g���N�^
		{
			// �l�̃N���A
			fStartColAlpha = 0.0f;
			fStartMulti = 0.0f;
			fAddTimer = 0.0f;
			fColAlpha = 0.0f;
			fMulti = 0.0f;
			fTimer = 0.0f;
			bActive = false;
		}
	};

public:

	struct MultiTarget
	{
		LPDIRECT3DTEXTURE9 pTextureMT[2];	// �����_�����O�^�[�Q�b�g�p�e�N�X�`��
		LPDIRECT3DSURFACE9 pRenderMT[2];	// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X
		LPDIRECT3DSURFACE9 pZBuffMT;		// �e�N�X�`�������_�����O�pZ�o�b�t�@
		D3DVIEWPORT9 viewportMT;			// �e�N�X�`�������_�����O�p�r���[�|�[�g
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff;	// ���_�o�b�t�@
	};

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice() const;
	LPDIRECT3D9 GetD3D() const { return m_pD3D; }

	HRESULT SetFullScreen();

	// �}���`�^�[�Q�b�g�֘A
	void GetDefaultRenderTarget(LPDIRECT3DSURFACE9* pRender, LPDIRECT3DSURFACE9* pZBuff, D3DXMATRIX* viewport, D3DXMATRIX* projection);	// �f�t�H���g�̃����_�[�^�[�Q�b�g�擾
	void ChangeRendertarget(LPDIRECT3DSURFACE9 pRender, LPDIRECT3DSURFACE9 pZBuff, D3DXMATRIX viewport, D3DXMATRIX projection);	// �^�[�Q�b�g�ؖ�
	void ChangeTarget(MyLib::Vector3 posV, MyLib::Vector3 posR, MyLib::Vector3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(int idx) { return m_Multitarget.pTextureMT[idx]; }	// �����_�����O�^�[�Q�b�g�p�e�N�X�`���擾

	/**
	@brief	�}���`�^�[�Q�b�g��ʂ̕`�攻��	
	@param	fGoalAlpha	[in]	�ڕW�����F
	@param	fGoalMulti	[in]	�ڕW��ʔ{��
	@param	fTimer		[in]	�ڕW�܂ł̎���
	@return	void
	*/
	void SetEnableDrawMultiScreen(float fGoalAlpha, float fGoalMulti, float fTimer);

private:

	void ResetRendererState();
	void InitMTRender();	// �}���`�^�[�Q�b�g�����_���[�̏�����
	void SetMultiTarget();

	void DrawMultiTargetScreen(int texIdx, const D3DXCOLOR& col, const D3DXVECTOR2& size);	// �}���`�^�[�Q�b�g��ʂ̕`��

	LPDIRECT3D9 m_pD3D;					// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3D�f�o�C�X�ւ̃|�C���^
	D3DPRESENT_PARAMETERS m_d3dpp;	// �v���[���e�[�V�������[�h
	MultiTargetInfo m_MultitargetInfo;
	MultiTarget m_Multitarget;	// �}���`�^�[�Q�b�g�p
};



#endif