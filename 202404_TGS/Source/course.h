//=============================================================================
// 
//  コースヘッダー [course.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _COURSE_H_
#define _COURSE_H_	// 二重インクルード防止

#include "object3DMesh.h"
#include "collisionLine_Box.h"

//==========================================================================
// クラス定義
//==========================================================================
// コースクラス
class CCourse : public CObject3DMesh
{

public:

	//=============================
	// 構造体定義
	//=============================
	// 辺の内部情報
	struct LineInfo
	{
		MyLib::Vector3 pos;	// 位置
		MyLib::Vector3 rot;	// 向き
		float width;		// 幅

		LineInfo() : pos(0.0f), rot(0.0f), width(0.0f) {}
	};

	// 辺情報
	struct Line
	{
		LineInfo lineInfo;	// 辺の内部情報
	};

	CCourse(int nPriority = 1, const LAYER layer = LAYER::LAYER_MAP);
	~CCourse();

	// オーバーライドされた関数
	virtual HRESULT Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void SetVtx();

	void Reset();	// リセット

	// 情報取得・設定
	std::vector<LineInfo> GetLineInfo() { return m_LineInfo; };						// 辺情報取得
	LineInfo GetLineInfo(int idx);													// 辺情報取得
	void SetLineInfo(const std::vector<LineInfo>& vecinfo) { m_LineInfo = vecinfo; }// 辺情報設定
	void SetLineInfo(int idx, const LineInfo& info);								// 辺情報設定
	void PushLineInfo();															// 辺情報追加
	void PopLineInfo();																// 辺情報削除

	// ファイル操作
	void Save();	// セーブ
	HRESULT Load(const std::string& file);	// ロード

	static CCourse *Create(const std::string& file);	// 生成

private:

	// メンバ関数
	void SetVtxPosition();	// 頂点座標

	// メンバ変数
	std::string m_FileName;	// ファイル名
	std::vector<LineInfo> m_LineInfo;	// 辺情報
	std::vector<CCollisionLine_Box*> m_pCollisionLineBox;	// 当たり判定ボックス

};



#endif