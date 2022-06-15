#ifndef STAGE_H__
#define STAGE_H__

#include "Scene.h"
#include "Include.h"

_BEGIN(Client)

class ParticleCubeMgr;
class Stage final : public Engine::Scene
{
private:
	explicit Stage(LPDIRECT3DDEVICE9 pDevice);
	virtual ~Stage() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;

public:
	HRESULT Ready_GameObjectPrototype();
	HRESULT ReadyComponent();
	HRESULT Ready_LayerCamera(const TCHAR* pLayerTag, TCHAR* pCameraTag); //TY_Ready_LayerCamera_180613 Add Parameter For Camera Choice
	HRESULT Ready_LayerBackGround(const TCHAR* pLayerTag);
	HRESULT Ready_LayerUI(const TCHAR* pLayerTag); // JH_UI_180704 Ready_LayerBackGround UI Tag로 레이어로 구성
	HRESULT Ready_LayerPlayer(const TCHAR* pLayerTag);
	HRESULT Ready_LayerBoss(const TCHAR* pLayerTag); // JH_BossAdd_180616 BossLayer Functino Add
private:
	HRESULT Ready_LightInfo();

public:
	static Stage* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual DWORD Free();

private: // Camera Choice Related Variable
	TCHAR* m_pCurCamera_Tag = nullptr;

private: //DH_BulletMgr_180618 Ready To BulletMgr
	BulletMgr* m_pBulletMgr;
	//TY_PARTICLE_180623 Add ParticleCubeMgr Variable
	ParticleCubeMgr* m_pParticleCubeMgr = nullptr;

};

_END

#endif // STAGE_H__ 

