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
// 現在のフレーム数を設定
//=============================================================================
void CCommand::SetNowFlame(int nFlame)
{
	m_nNowFlame = nFlame;

	if (m_nNowFlame < 0 || m_nNowFlame >= m_nMaxFlame) Reset();	// 最大フレームを超えた
}

//=============================================================================
// 入力状況初期化
//=============================================================================
void CCommand::Reset()
{
	// フラグをリセット
	for (auto it = m_Info.begin(); it != m_Info.end(); it++) {
		it->second = false;
	}

	m_nNowFlame = 0;
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
	DIRECTION direction = GetDirection();
	auto it = m_Info.begin();

	if (it->second) SetNowFlame(GetNowFlame() + 1);	// 一つ目が入力されていたらカウント増加

	for (it; it != m_Info.end(); it++) {
		if (it->second) continue;	// 既に入力済み
		else if (it->first == direction) it->second = true;	// 入力方向が一致
		else	Reset();	// 入力キーが一致しない
	}

	if (it == m_Info.end() && it->second) bValue = true;	// 最後のコマンドまで入力できた

	return bValue;	// コマンド成功失敗を返す
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