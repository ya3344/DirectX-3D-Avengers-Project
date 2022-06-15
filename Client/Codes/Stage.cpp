#include "stdafx.h"
#include "Stage.h"
#include "ComponentMgr.h"
#include "CollisionMgr.h"
#include "ParticleCubeMgr.h"
#include "Terrain.h"
#include "CameraFree.h"
#include "CameraTps.h"
#include "CameraAction.h"
#include "CameraMgr.h"
#include "Player.h"
#include "Boss.h"
#include "AnimCube.h"
#include "SkyBox.h"
#include "Bullet.h"
#include "PartUI.h"
#include "FullUI.h"
#include "Effect.h"

_USING(Client)

Stage::Stage(LPDIRECT3DDEVICE9 pDevice)
	: Scene(pDevice)
	, m_pBulletMgr(BulletMgr::GetInstance())
{
	m_pBulletMgr->AddRef();
}

HRESULT Stage::Init()
{
	// 현재 씬에 셋팅되야할 광원을 생성한다.
	if (FAILED(Ready_LightInfo()))
		return E_FAIL;

	// 현재 씬에 필요한 원형객체들을 생성하자.
	if (FAILED(Ready_GameObjectPrototype()))
		return E_FAIL;

	if (FAILED(ReadyComponent()))
		return E_FAIL;

	// JH_BossAI_180616 항상 Layer_Player 부터 먼저 호출되도록.
	if (FAILED(Ready_LayerPlayer(L"Layer_Player")))
		return E_FAIL;

	// JH_UI_180704 Ready_LayerUI 항상 Boss 먼저추가
	if (FAILED(Ready_LayerUI(L"Layer_UI")))
		return E_FAIL;

	if (FAILED(Ready_LayerBoss(L"Layer_Boss")))
		return E_FAIL;

	// TY_Stage Init_180614 Add Parameter
	// Stage 씬에서 필요한 객체와 컴포넌트들을 생성하자.
	if (FAILED(CameraMgr::GetInstance()->Ready_LayerCamera(L"Prototype_GameObject_CameraAction")))
		return E_FAIL;

	if (FAILED(Ready_LayerBackGround(L"Layer_BackGround")))
		return E_FAIL;

	//TY_OBB_180618 Call Function Init of CollisionMgr
	CollisionMgr::GetInstance()->Init();
	CameraMgr::GetInstance()->Init();

	//TY_PARTICLE_180623 Call Function Init of ParticleCubeMgr
	m_pParticleCubeMgr = ParticleCubeMgr::GetInstance();
	m_pParticleCubeMgr->AddRef();
	m_pParticleCubeMgr->Init();

	// JH_Effect_180704 Effect 생성(Stage)
	if (FAILED(EffectMgr::GetInstance()->ReadyEffect()))
	{
		_MSG_BOX("ReadyEffect");
		return E_FAIL;
	}

	// JH_PlanetAdd_1800705 Planet 생성
	if (FAILED(PlanetMgr::GetInstance()->ReadyPlanet()))
	{
		_MSG_BOX("ReadyPlanet");
		return E_FAIL;
	}

	Engine::SoundMgr::GetInstance()->PlayBGM(L"The Avengers.mp3");
	return NOERROR;
}

UPDATE Stage::Update(const float timeDelta)
{
	CameraMgr::GetInstance()->Update_Camera();
	CollisionMgr::GetInstance()->Update();

	//TY_PARTICLE_180623 Call Function Update of ParticleCubeMgr
	m_pParticleCubeMgr->Update(timeDelta);

	//DH_BulletMgr_180618 BulletMgr Update Call
	m_pBulletMgr->Update(timeDelta);

	return Scene::Update(timeDelta);
}

UPDATE Stage::LastUpdate(const float timeDelta)
{
	//TY_PARTICLE_180619 Call Function LastUpdate of ParticleCubeMgr
	m_pParticleCubeMgr->LastUpdate(timeDelta);

	//DH_BulletMgr_180618 BulletMgr LastUpdate Call
	m_pBulletMgr->LastUpdate(timeDelta);

	return Scene::LastUpdate(timeDelta);
}

void Stage::Render()
{
	//TY_PARTICLE_180623 Call Function Render of ParticleCubeMgr
	m_pParticleCubeMgr->Render();
}

HRESULT Stage::Ready_GameObjectPrototype()
{
	Engine::GameObject* pGameObject = nullptr;

	// DH_SkyBoxAdd_180613 For.Prototype_GameObject_SkyBox
	pGameObject = SkyBox::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_SkyBox", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	// For.Prototype_GameObject_Terrain
	pGameObject = Terrain::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_Terrain", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	// For.Prototype_GameObject_Player
	pGameObject = Player::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_Player", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	// JH_BossAdd_180616 For.Prototype_GameObject_Boss
	pGameObject = Boss::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_Boss", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	// For.Prototype_GameObject_CameraFree
	pGameObject = CameraFree::Create(m_pDevice, D3DXVECTOR3(40.f, 20.f, -15.f), D3DXVECTOR3(40.f, 0.f, 20.f), D3DXVECTOR3(0.f, 1.f, 0.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_CameraFree", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	//TY_Ready_GameObjectPrototype_180613 Add CameraAction Prototype
	// For.Prototype_GameObject_CameraAction
	pGameObject = CameraAction::Create(m_pDevice, D3DXVECTOR3(0.f, 5.f, -20.f), D3DXVECTOR3(0.f, 0.f, 10.f), D3DXVECTOR3(0.f, 1.f, 0.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_CameraAction", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	//TY_Ready_GameObjectPrototype_180615 Add CameraTps Prototype
	// For.Prototype_GameObject_CameraTps
	pGameObject = CameraTps::Create(m_pDevice, D3DXVECTOR3(0.f, 5.f, -20.f), D3DXVECTOR3(0.f, 0.f, 10.f), D3DXVECTOR3(0.f, 1.f, 0.f));
	if (pGameObject == nullptr)
		return E_FAIL;

	if (FAILED(Add_GameObjectPrototype(L"Prototype_GameObject_CameraTps", SCENE_STAGE, pGameObject)))
		return E_FAIL;


	// DH_BulletAdd_180618  For.Prototype_GameObject_Bullet
	m_pBulletMgr->Ready_BulletPrototype(m_pDevice);


	// JH_UI_180704_ For.Prototype_GameObject_UI
	pGameObject = PartUI::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	pGameObject->setName(L"PartUI");

	if (FAILED(Add_GameObjectPrototype(L"Prototype_Part_UI", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	pGameObject = FullUI::Create(m_pDevice);
	if (pGameObject == nullptr)
		return E_FAIL;
	pGameObject->setName(L"FullUI");

	if (FAILED(Add_GameObjectPrototype(L"Prototype_Full_UI", SCENE_STAGE, pGameObject)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Stage::ReadyComponent()
{
	Engine::Component* pComponent = nullptr;

	return NOERROR;
}

//TY_Ready_LayerCamera_180613 Add Parameter For Camera Choice
HRESULT Stage::Ready_LayerCamera(const TCHAR* pLayerTag, TCHAR* pCameraTag)
{
	if (pLayerTag == nullptr && pCameraTag == nullptr)
		return E_FAIL;

	if (m_pCurCamera_Tag != nullptr && lstrcmp(m_pCurCamera_Tag, pLayerTag) == 0)
		return NOERROR;

	m_pCurCamera_Tag = pCameraTag;

	// camera Layer 싹다 비우는 함수 호출 후 add Layer
	ClearLayer(SCENE_STAGE, pLayerTag);

	//TY_Ready_LayerCamera_180613 Add CameraAction To Layer
	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, pCameraTag, SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Stage::Ready_LayerBackGround(const TCHAR * pLayerTag)
{
	// For.Terrain
	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_Terrain", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	// DH_SkyBoxAdd_180613 For.SkyBox
	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_SkyBox", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}


HRESULT Stage::Ready_LayerUI(const TCHAR * pLayerTag)
{
	// JH_UI_180704 Ready_LayerBackGround UI Tag로 레이어로 구성
	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_Full_UI", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_Part_UI", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	

	return NOERROR;
}

HRESULT Stage::Ready_LayerPlayer(const TCHAR * pLayerTag)
{
	// For.Player
	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_Player", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

// JH_BossAdd_180616 BossLayer Functino Add
HRESULT Stage::Ready_LayerBoss(const TCHAR * pLayerTag)
{
	// For.Boss
	if (FAILED(Add_GameObjectToLayer(SCENE_STAGE, L"Prototype_GameObject_Boss", SCENE_STAGE, pLayerTag)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Stage::Ready_LightInfo()
{
	D3DLIGHT9 lightInfo;
	ZeroMemory(&lightInfo, sizeof(D3DLIGHT9));

	lightInfo.Type = D3DLIGHT_DIRECTIONAL;
	lightInfo.Direction = D3DXVECTOR3(0.0f, -0.5f, -1.0f);
	lightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	lightInfo.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 0.4f);

	if (FAILED(m_pDevice->SetLight(0, &lightInfo)))
		return E_FAIL;

	if (FAILED(m_pDevice->LightEnable(0, TRUE)))
		return E_FAIL;

	return NOERROR;
}

Stage * Stage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Stage* pInstance = new Stage(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("StageScene Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Stage::Free()
{
	//TY_OBB_180618 Destroy CollisionMgr
	CollisionMgr::GetInstance()->DestroyInstance();

	// TY_ChangeCamera_180618 Destroy CameraMgr
	CameraMgr::GetInstance()->DestroyInstance();
	Engine::Safe_Release(m_pBulletMgr);
	Engine::Safe_Release(m_pParticleCubeMgr);
	Scene::ClearGameObject(SCENE_STAGE);
	Scene::Free();

	return DWORD();
}
