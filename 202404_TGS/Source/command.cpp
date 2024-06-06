//=============================================================================
//
// コマンド処理 [command.cpp]
// Author : Ibuki Okusada
//
//=============================================================================
#include "command.h"
#include "input_gamepad.h"
#include "debugproc.h"
#include "manager.h"

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
	Info info;
	info.nButton = nKey;
	info.bActive = false;
	m_Info.push_back(info);
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
		it->bActive = false;
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
	auto old = it;

	if (it->bActive) SetNowFlame(GetNowFlame() + 1);	// 一つ目が入力されていたらカウント増加

	for (it = m_Info.begin(); it != m_Info.end(); it++) 
	{
		if (it->bActive) 
		{ // 既に入力済み
			CManager::GetInstance()->GetDebugProc()->Print("入力されたよ^^\n"); 
			old = it; 
			continue; 
		}
		else if (it->nButton == direction)
		{ // 入力方向が一致
			it->bActive = true;
			break;
		}
		else { // 入力キーが一致しない
			if (it != m_Info.begin()) {	// 最初以外
				if (old->nButton == direction) { // 前回と入力情報が一致
					old = it;
					return bValue;
				}
			}

			Reset();
			break;
		}

		old = it;
	}

	if (it == m_Info.end()) bValue = true;	// 最後のコマンドまで入力できた

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
	else if ((rot <= -D3DX_PI + size && rot >= -D3DX_PI) || 
		(rot >= D3DX_PI - size && rot <= D3DX_PI))			return DIRECTION::STICK_DOWN;
	else if (rot <= -D3DX_PI * 0.5f + size	&& rot >= -D3DX_PI * 0.5f - size)	return DIRECTION::STICK_LEFT;
	else if (rot <= D3DX_PI * 0.5f + size	&& rot >= D3DX_PI * 0.5f - size)	return DIRECTION::STICK_RIGHT;
	else if (rot <= -D3DX_PI * 0.25f + size && rot >= -D3DX_PI * 0.25f - size)	return DIRECTION::STICK_LEFTUP;
	else if (rot <= D3DX_PI * 0.25f + size	&& rot >= D3DX_PI * 0.25f - size)	return DIRECTION::STICK_RIGHTUP;
	else if (rot <= -D3DX_PI * 0.75f + size && rot >= -D3DX_PI * 0.75f - size)	return DIRECTION::STICK_LEFTDOWN;
	else if (rot <= D3DX_PI * 0.75f + size	&& rot >= D3DX_PI * 0.75f - size)	return DIRECTION::STICK_RIGHTDOWN;

	return DIRECTION::STICK_NONE;	// 入力されていない場合
}