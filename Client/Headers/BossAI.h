#ifndef BOSS_AI_H__
#define BOSS_AI_H__

#include "Component.h"
#include "Include.h"


namespace Engine
{
	class MathMgr;
}

_BEGIN(Client)

class Boss;
class DetectRect;
class HpBar;

typedef struct tagTimeInfo
{
public:
	float accumulateTime = 0.f;
public:
	float moveTime = 0.f;
	float idleTime = 0.f;
public:
	float lastTime = 0.f;
	bool bLastTime = false;

}TIME_INFO;

class BossAI : public Engine::Component
{
public:
	enum SEARCH { READY, START, END };

protected:
	explicit BossAI(LPDIRECT3DDEVICE9 pDevice);
	explicit BossAI(const BossAI& rhs);
	virtual ~BossAI() = default;

public:
	HRESULT Init(SUPER_CUBE_INFO* pSuperCubeInfo);

public:
	HRESULT SetPlayer();
	HRESULT SetBoss(Boss* pBossObj);
	HRESULT SetAnimMgr(AnimMgr* pAnimMgr);
	HRESULT SetDetectRect(DetectRect* pAnimMgr);
	HRESULT SetHpBar(HpBar* pHpBar);

public:
	virtual void AnimState_Change(const float timeDelta) = 0;
	virtual void AnimUpdate(const float timeDelta) = 0;
	virtual void SelectAttackMode() = 0;
	virtual void DestroyPart() = 0;

public:
	void Set_AnimState(const ANIM_STATE animState) { mAnimState = animState; }
	ANIM_STATE Get_AnimState() const { return mAnimState; }
	void Set_AIMode(const AI_MODE AIMode) { mAIMode = AIMode; }
	void AI_Mode(const float timeDelta);
	bool Get_AttackState() const { return m_bAttack; }
	void SetBossInfo(SUPER_CUBE_INFO* pBossInfo) { m_pBossInfo = pBossInfo; }

protected: // Animation Attribute Related Function
	void LastTime(const float time);
	void InitAnimState(const TCHAR* pAnimName, const TCHAR* pCubeName, const D3DXVECTOR3 vAnimSpeed, const bool bDetectOperaction);
	bool FrameControl(const TCHAR* pName, size_t frame);
	void InitStateVariable();

private:
	void Set_FrameInfo(const TCHAR* pCubeName) { if (m_pFrameInfo == nullptr) { m_pFrameInfo = m_pAnimMgr->Get_FrameInfo(pCubeName); } }
	void Set_AnimInfo(const TCHAR* pCubeName) { if (m_pAnimInfo == nullptr) { m_pAnimInfo = m_pAnimMgr->Get_AnimInfo(pCubeName); } }

private:
	void Idle(const float timeDelta);
	void Cal_CenterPos(const float timeDelta);
	void Attack();
	void ChangeSearchAnim();

public:
	virtual Component* Clone() = 0;
	virtual DWORD Free() override;

protected:
	//DH_SoundMgrSetting_180705_Setting to SoudnMgr
	Engine::SoundMgr * m_pSoundMgr = nullptr;
	BulletMgr* m_pBulletMgr = nullptr;
	AnimMgr* m_pAnimMgr = nullptr;
	HpBar* m_pHpBar = nullptr;
	TIME_INFO mTimeInfo;
	DetectRect* m_pDetectRect = nullptr;
	Boss* m_pBossObj = nullptr;
	const SUPER_CUBE_INFO* m_pBossInfo = nullptr;
	const SUPER_CUBE_INFO* m_pPlayerInfo = nullptr;

protected:
	const bool* m_pDetectColor = nullptr;
	float mAttackTime = 0.f;
	float mRandomDegree = 0.f;

protected: // AI & Animation Related Variable
	ANIM_STATE mPreAnimState = ANIM_STATE::END;
	ANIM_STATE mAttackAnimState = ANIM_STATE::END;
	ANIM_STATE mAnimState = ANIM_STATE::END;
	const FRAME_INFO* m_pFrameInfo = nullptr;
	const ANIM_INFO* m_pAnimInfo = nullptr;
	AI_MODE mAIMode = AI_MODE::END;

protected: // AnimState Related Variable
	bool m_bSearch = true;
	bool m_bAttack = false;
	bool m_bCenter = true;
	bool m_bFrameControl = false;

private:
	Engine::MathMgr* m_pMathMgr = nullptr;
	

private: // IDle Related Varible
	float mDistance = 100.f;
	D3DXVECTOR3 m_vCenterPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	float mCenterDegree = 90.f;

protected://DH_SoundAdd_180706_SoundAdd
	float mSoundTime = 0.f;
};

_END
#endif // BOSS_AI_H__