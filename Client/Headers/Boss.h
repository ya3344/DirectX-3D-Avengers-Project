#ifndef BOSS_H__ 
#define BOSS_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class Renderer;
	class CubeTex;
	class Texture;
	class Layer;
	class AnimNode;
}

_BEGIN(Client)

class AnimCube;
class AnimMgr;
class BossAI;
class Sphere;
class DetectRect;
class HpBar;

class Boss final : public Engine::GameObject
{
public: 
	enum BOSS_TYPE { IRON, THANOS, END };

private:
	explicit Boss(LPDIRECT3DDEVICE9 pDevice);
	explicit Boss(const Boss& rhs);
	virtual ~Boss() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

public:
	void Set_SuperCube(AnimCube* pAnimCube, const float HP, const D3DXVECTOR3 vPos);
	bool SetDamage(const float damage);// TY_HP_180626_Add SetDamage Function
	const SUPER_CUBE_INFO* Get_SuperCubeInfo() const { return &mSuperCube_Info; }

public: // TY_OBB_180618 Add Function To Get Sphere
	Sphere* GetSphere() const { return m_pSphere; }
	HpBar* GetHpBar() const { return m_pHpBar; } // TY_ChangeStage_180703_ Add Function To Get HpBar

public:
	void SetThanos();
	void SetDebugThanos();

private:
	HRESULT Ready_LayerAnimCube();
	HRESULT ReadyComponent(void);

private: // DebugFunction
	HRESULT KeyCheck(const float timeDelta);

public:
	static Boss* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private: 
	Engine::Renderer* m_pRenderer = nullptr;
	AnimMgr* m_pAnimMgr = nullptr;
	EffectMgr* m_pEffectMgr = nullptr;

private: // AnimSuperCube Info Related Variable
	SUPER_CUBE_INFO mSuperCube_Info;
	AnimCube* m_pAnimCube[END] = { nullptr, nullptr };

private: // AI Related Variable
	BossAI* m_pBossAI[END] = { nullptr, nullptr }; // IRON, BOSS AI 저장
	BossAI* m_pCurrentAI = nullptr; // 진행중인 AI
	BOSS_TYPE mBossType = IRON;

private: // Debug Boss Free Control Variable
	bool m_bFreeControl = false;

private: // Sphere Related Variable
	Sphere* m_pSphere = nullptr;

private: // DetectRect Related Variable
	DetectRect* m_pDetectRect = nullptr;

private: // TY_HP_180626_Hp Related Variable
	bool m_bSecondHP = false; 	//TY_ChangeStage_180703_ Second HP Related Variable
	float mHP = 0.f;
	float mMaxHP = 0.f;
	HpBar* m_pHpBar = nullptr;
};

_END

#endif // BOSS_H__

