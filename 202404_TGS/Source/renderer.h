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
	bool IsDrawMultiScreen() { return m_bDrawMultiScreen; };					// �}���`�^�[�Q�b�g��ʂ̕`�攻��
	void SetEnableDrawMultiScreen(bool bDraw) { m_bDrawMultiScreen = bDraw; }	// �}���`�^�[�Q�b�g��ʂ̕`�攻��

private:

	void ResetRendererState();
	void InitMTRender();	// �}���`�^�[�Q�b�g�����_���[�̏�����

	void DrawMultiTargetScreen(int texIdx, const D3DXCOLOR& col, const D3DXVECTOR2& size);	// �}���`�^�[�Q�b�g��ʂ̕`��

	LPDIRECT3D9 m_pD3D;					// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3D�f�o�C�X�ւ̃|�C���^
	D3DPRESENT_PARAMETERS m_d3dpp;	// �v���[���e�[�V�������[�h

	MultiTarget m_Multitarget;	// �}���`�^�[�Q�b�g�p
	bool m_bDrawMultiScreen;	// �}���`�^�[�Q�b�g��ʂ̕`�攻��
};



#endif