#ifndef PLAYER_H__ 
#define PLAYER_H__

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
class Sphere;
class HpBar;
class ShieldBullet;

class Player final : public Engine::GameObject
{
private:
	explicit Player(LPDIRECT3DDEVICE9 pDevice);
	explicit Player(const Player& rhs);
	virtual ~Player() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

public:
	void Set_SuperCube(AnimCube* pAnimCube);
	void SetDamage(const float hp);// TY_HP_180625_Add SetDamage Function
	const SUPER_CUBE_INFO* Get_SuperCubeInfo() const { return &mSuperCube_Info; }
	Engine::Transform* Get_PlayerTransform() { return mSuperCube_Info.pTransform; }
	Sphere* GetSphere() const { return m_pSphere; } //TY_OBB_180618 Add Function To Get Sphere

public: // Shiled Related Function
	ShieldBullet* GetShield() const { return m_pShield; } //TY_FixBullet_180701_Add Function To Get Shield
	void SetShield_RenderState(const bool renderState);

private:
	HRESULT Ready_LayerAnimCube();
	HRESULT ReadyComponent(void);
	HRESULT KeyCheck(const float timeDelta);
	// TY_Sliding_180702_Add Function To Check Wall
	void SlidingWall();

private:
	void AnimStateChange();
	void AnimUpdate();
	void ChangeStandAnim();
	void InitAnimState(const TCHAR* pAnimName, const TCHAR* pCubeName, const D3DXVECTOR3 vAnimSpeed);
	void Set_FrameInfo(const TCHAR* pCubeName) {
		if (m_pFrameInfo == nullptr)
		{
			m_pFrameInfo = m_pAnimMgr->Get_FrameInfo(pCubeName);
		}
	}

	void Set_AnimInfo(const TCHAR* pCubeName) {
		if (m_pAnimInfo == nullptr)
		{
			m_pAnimInfo = m_pAnimMgr->Get_AnimInfo(pCubeName);
		}
	}
	void Attack();


public:
	static Player* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private: // Component Related Variable
	Engine::Renderer* m_pRenderer = nullptr;
	AnimMgr* m_pAnimMgr = nullptr;

private: // AnimSuperCube Info Related Variable
	SUPER_CUBE_INFO mSuperCube_Info;

private: // AnimState Related Variable
	ANIM_STATE mPreAnimState = ANIM_STATE::END;
	ANIM_STATE mAnimState = ANIM_STATE::STAND;

private: // Frame, Anim Info Related Variable 
	const FRAME_INFO* m_pFrameInfo = nullptr;
	const ANIM_INFO* m_pAnimInfo = nullptr;

private: // Sphere Related Variable
	Sphere* m_pSphere = nullptr;
	// TY_HP_180626_Hp Related Variable
	float mHP = 0.f;
	float mMaxHP = 0.f;
	HpBar* m_pHpBar = nullptr;
	//TY_FixBullet_180701_Shield Related Variable
	ShieldBullet* m_pShield = nullptr;

private:
	EffectMgr* m_pEffectMgr = nullptr;

};

_END

#endif

