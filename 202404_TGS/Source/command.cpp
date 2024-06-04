//=============================================================================
//
// コマンド処理 [command.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "command.h"
#include "input_gamepad.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CCommand::CCommand()
{
	// 値のクリア
	m_nNowFlame = 0;
	m_nMaxFlame = 0;
	m_Info.clear();
}

//=============================================================================
// 終了処理
//=============================================================================
void CCommand::Uninit()
{
	m_Info.clear();
}

//=============================================================================
// コマンド設定
//=============================================================================
void CCommand::SetCommand(int nKey)
{
	m_Info[nKey] = false;	// 登録
}

//=============================================================================
// 
// パッド設定
// 
//=============================================================================
// コマンド入力確認
//=============================================================================
bool CCommandPad::GetCommand()
{
	bool bValue = false;
	auto it = m_Info.begin();

	if (it->second) SetNowFlame(GetNowFlame() + 1);	// 一つ目が入力されていたらカウント増加

	for (it; it != m_Info.end(); it++) {

	}

	return bValue;
}

//=============================================================================
// 入力方向取得
//=============================================================================
CCommandPad::DIRECTION CCommandPad::GetDirection()
{
	CInputGamepad* pPad = CInputGamepad::GetInstance();
	float rot = pPad->GetStickRotR(0);
	float size = D3DX_PI * 0.125f;

	// 向きから割り出す
	if		(rot <= 0.0f + size				&& rot >= 0.0f - size)				return DIRECTION::STICK_UP;
	else if (rot <= -D3DX_PI + size			&& rot >= D3DX_PI - size)			return DIRECTION::STICK_DOWN;
	else if (rot <= -D3DX_PI * 0.5f + size	&& rot >= -D3DX_PI * 0.5f - size)	return DIRECTION::STICK_LEFT;
	else if (rot <= D3DX_PI * 0.5f + size	&& rot >= D3DX_PI * 0.5f - size)	return DIRECTION::STICK_RIGHT;
	else if (rot <= -D3DX_PI * 0.25f + size && rot >= -D3DX_PI * 0.25f - size)	return DIRECTION::STICK_LEFTUP;
	else if (rot <= D3DX_PI * 0.25f + size	&& rot >= D3DX_PI * 0.25f - size)	return DIRECTION::STICK_RIGHTUP;
	else if (rot <= -D3DX_PI * 0.75f + size && rot >= -D3DX_PI * 0.75f - size)	return DIRECTION::STICK_LEFTDOWN;
	else if (rot <= D3DX_PI * 0.75f + size	&& rot >= D3DX_PI * 0.75f - size)	return DIRECTION::STICK_RIGHTDOWN;

	return DIRECTION::STICK_NONE;	// 入力されていない場合
}