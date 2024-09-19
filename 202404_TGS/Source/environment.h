//=============================================================================
// 
//  自然物ヘッダー [environment.h]
//  Author : 相馬靜雅
// 
//=============================================================================

#ifndef _ENVIRONMENT_H_
#define _ENVIRONMENT_H_	// 二重インクルード防止

//==========================================================================
// クラス定義
//==========================================================================
// 自然物クラス
class CSpawnEnvironment
{
public:

	CSpawnEnvironment() : m_fSpawnTimer(0.0f), m_fSpawnInterval(0.0f) {}
	CSpawnEnvironment(float timer, float interval) : m_fSpawnTimer(timer), m_fSpawnInterval(interval) {}
	virtual ~CSpawnEnvironment() {}

	// 更新処理
	virtual void Update(float deltaTime);

protected:

	//=============================
	// メンバ関数
	//=============================
	virtual void TriggerSpawn() = 0;	// 生成時のトリガー

	//=============================
	// メンバ変数
	//=============================
	float m_fSpawnTimer;	// 生成タイマー
	float m_fSpawnInterval;	// 生成間隔


};

//=============================
// 空気の生成クラス
//=============================
class CSpawn_Air : public CSpawnEnvironment
{
public:

	CSpawn_Air() : CSpawnEnvironment() {}
	CSpawn_Air(float timer, float interval) : CSpawnEnvironment(timer, interval) {}
	~CSpawn_Air() {}

protected:

	//=============================
	// メンバ関数
	//=============================
	void TriggerSpawn() override;	// 生成時のトリガー

};

//=============================
// 流れる葉の生成クラス
//=============================
class CSpawn_FlowLeaf : public CSpawnEnvironment
{
public:

	CSpawn_FlowLeaf() : CSpawnEnvironment() {}
	CSpawn_FlowLeaf(float timer, float interval) : CSpawnEnvironment(timer, interval) {}
	~CSpawn_FlowLeaf() {}

protected:

	//=============================
	// メンバ関数
	//=============================
	void TriggerSpawn() override;	// 生成時のトリガー

};



//=============================
// 降る葉の生成クラス
//=============================
class CSpawn_Leaf : public CSpawnEnvironment
{
public:

	CSpawn_Leaf() : CSpawnEnvironment() {}
	CSpawn_Leaf(float timer, float interval) : CSpawnEnvironment(timer, interval) {}
	~CSpawn_Leaf() {}

	// 更新処理
	virtual void Update(float deltaTime) override;

protected:

	//=============================
	// メンバ関数
	//=============================
	virtual void TriggerSpawn() override;	// 生成時のトリガー

};

class CSpawn_Leaf_Title : public CSpawn_Leaf
{
public:

	CSpawn_Leaf_Title() : CSpawn_Leaf() {}
	CSpawn_Leaf_Title(float timer, float interval) : CSpawn_Leaf(timer, interval) {}
	~CSpawn_Leaf_Title() {}

	// 更新処理
	virtual void Update(float deltaTime) override;

protected:

	//=============================
	// メンバ関数
	//=============================
	void TriggerSpawn() override;	// 生成時のトリガー

};



#endif