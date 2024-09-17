//======================================================
//
//ポーズ画面のヘッダ[game.h]
//Author:石原颯馬
//
//======================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//タイトルクラス
class CPause
{
public:
	//選択列挙
	enum MODE
	{
		MODE_RESUME = 0,
		MODE_RESTART,
		MODE_EXIT,
		MODE_SCREENSHOT,
		MODE_MAX
	};

	//コンストラクタ・デストラクタ
	CPause();
	~CPause();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Select(void);

	MODE m_mode;			//選択中モード
	bool m_bScreenShot;		//スクショ中か
};

#endif // !_GAME_H_
