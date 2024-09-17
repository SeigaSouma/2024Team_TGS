//==========================================
//
//デバッグ表示プログラムのヘッダ[debugproc.h]
//Author:石原颯馬
//
//==========================================
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_
#include "..\lostrssmanager\lostrssmanager.h"

//マクロ
#define DEBPRO_MAX_STR	(2048)			//確保する文字数

class CDebugProc : public ILostResource
{
public:
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	void Draw(void);
	void Print(const char* fmt, ...);

	void Backup();
	void Restore();
private:
	LPD3DXFONT m_pFont = nullptr;				//フォントへのポインタ
	char m_aStrPrint[DEBPRO_MAX_STR];		//表示文字バッファ
};

#endif // !_DEBUGPROC_H_
