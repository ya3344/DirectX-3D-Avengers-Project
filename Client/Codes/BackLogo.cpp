#include "stdafx.h"
#include "BackLogo.h"
#include "ComponentMgr.h"
#include "GraphicDevice.h"
#include <process.h>

_USING(Client);

BackLogo::BackLogo(LPDIRECT3DDEVICE9 pDevice)
	: GameObject(pDevice)
{

}

BackLogo::BackLogo(const BackLogo & rhs)
	: GameObject(rhs)
{
	
}

HRESULT BackLogo::Init()
{
	// 이 객체에 필요한 컴포넌트를 추가하자.
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_pTransform->SetScale(2.f, 2.3f, 0.f);
	
	return NOERROR;
}


UPDATE BackLogo::Update(const float timeDelta)
{
	if (GetKeyState(VK_RETURN) & 0x8000)
	{
		if (g_bLoading)
		{
			m_bNotRender = true;
		}
	}
	return UPDATE();
}

UPDATE BackLogo::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;
	
	return UPDATE();
}

void BackLogo::Render()
{
	if (m_pRenderer == nullptr)
		return;

	if (m_bNotRender == true)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pTexture->SetUp_Device(0);
	m_pTransform->SetUp_World();
	m_pRectTex->Render();
	
	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

HRESULT BackLogo::ReadyPrototype()
{
	// 실제 원형 객체를 생성하기 위한 작업을 수행한다.
	// ex> 파일입출력을 통한 데이터 셋

	Engine::Component* pComponent = nullptr;

	{   // For.Component_Logo_Texture
		pComponent = Engine::Texture::Create(m_pDevice, Engine::Texture::GENERAL, L"../Bin/Resources/Textures/Logo/MarbleLogo.png");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_LOGO, L"Component_Texture_Logo", pComponent)))
			return E_FAIL;
	}

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, this, 0, nullptr);

	return NOERROR;
}



HRESULT BackLogo::ReadyComponent()
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_LOGO, L"Component_Texture_Logo");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_Logo", pComponent)))
		return E_FAIL;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	// For.Com_RetColor
	pComponent = m_pRectTex = (Engine::RectTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_RectTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_RectTexture", pComponent)))
		return E_FAIL;

	return NOERROR;
}

unsigned int BackLogo::ThreadFunc(void * pArg)
{
	Engine::Component* pComponent = nullptr;
	BackLogo* pBackLogo = (BackLogo*)pArg;

	{
		// For.Component_Texture_Player
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::CUBE, L"../Bin/Resources/Textures/Player/Player%d.dds", 22);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PLAYER, L"Component_Texture_Player", pComponent)))
			return E_FAIL;

		// FileLoadMgr 에서 플레이어 정보 받아옴
		if (FAILED(FileLoadMgr::GetInstance()->Obj_FileLoad(Engine::GraphicDevice::GetInstance()->GetDevice(), SCENE_PLAYER,
			L"Layer_Player_AnimCube", L"Component_Texture_Player", L"../Bin/Data/AnimObj/Player.dat")))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Boss
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::CUBE, L"../Bin/Resources/Textures/Boss/Boss%d.dds", 12);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_BOSS, L"Component_Texture_Boss", pComponent)))
			return E_FAIL;

		// FileLoadMgr 에서 Boss 정보 받아옴
		if (FAILED(FileLoadMgr::GetInstance()->Obj_FileLoad(Engine::GraphicDevice::GetInstance()->GetDevice(), SCENE_BOSS,
			L"Layer_Boss_AnimCube", L"Component_Texture_Boss", L"../Bin/Data/AnimObj/Boss.dat")))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Thanos
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::CUBE, L"../Bin/Resources/Textures/Thanos/Thanos%d.dds", 12);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_THANOS, L"Component_Texture_Thanos", pComponent)))
			return E_FAIL;

		// FileLoadMgr 에서 Thanos 정보 받아옴
		if (FAILED(FileLoadMgr::GetInstance()->Obj_FileLoad(Engine::GraphicDevice::GetInstance()->GetDevice(), SCENE_THANOS,
			L"Layer_Thanos_AnimCube", L"Component_Texture_Thanos", L"../Bin/Data/AnimObj/Thanos.dat")))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Effect, HP Bar
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::CUBE, L"../Bin/Resources/Textures/Color/Color%d.dds", 4);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PARTICLE, L"Component_Texture_Color", pComponent)))
			return E_FAIL;
	}

	{   // For.Compnent_Texture_UI
		Engine::Component* pComponent = nullptr;

		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::GENERAL, L"../Bin/Resources/Textures/UI/UI%d.png", 5);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STAGE, L"Component_Texture_UI", pComponent)))
			return E_FAIL;
	}

	{
		//TY_ChangeStage_180705_ Change Num of Texture
		// For.Component_Texture_Bullet
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::CUBE, L"../Bin/Resources/Textures/Bullet/Bullet%d.dds", 23);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_BULLET, L"Component_Texture_Bullet", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Terrain
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::CUBE, L"../Bin/Resources/Textures/Terrain/Nam/Cube%d.dds", 4);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_TERRAIN, L"Component_Texture_Terrain", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_SkyBox
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::CUBE, L"../Bin/Resources/Textures/SkyBox/burger2.dds");
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_STATIC, L"Component_Texture_SkyBox", pComponent)))
			return E_FAIL;
	}

	// JH_Effect_180704 Effect Texture 추가
	{
		// For.Component_Texture_Gun
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::GENERAL, L"../Bin/Resources/Textures/Gun/gun%d.png", 11);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PARTICLE, L"Component_Texture_Gun", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Sword
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::GENERAL, L"../Bin/Resources/Textures/Sword/sword%d.png", 11);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PARTICLE, L"Component_Texture_Sword", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Explosion
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::GENERAL, L"../Bin/Resources/Textures/Explosions/Explosions%d.png", 9);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PARTICLE, L"Component_Texture_Explosion", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Explosion
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::GENERAL, L"../Bin/Resources/Textures/Explosion/Explosion%d.png", 90);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PARTICLE, L"Component_Texture_Explosion2", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Dust
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::GENERAL, L"../Bin/Resources/Textures/Dust/%d.png", 54);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PARTICLE, L"Component_Texture_Dust", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Explosion
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::GENERAL, L"../Bin/Resources/Textures/MagicHit/MagicHit%d.png", 4);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PARTICLE, L"Component_Texture_MagicHit", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_EggHit
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::GENERAL, L"../Bin/Resources/Textures/EggHit/%d.png", 37);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PARTICLE, L"Component_Texture_EggHit", pComponent)))
			return E_FAIL;
	}

	{
		// For.Component_Texture_Planet
		pComponent = Engine::Texture::Create(Engine::GraphicDevice::GetInstance()->GetDevice(), Engine::Texture::CUBE, L"../Bin/Resources/Textures/Planet/Planet%d.dds", 5);
		if (pComponent == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ComponentMgr::GetInstance()->AddPrototype(SCENE_PLANENT, L"Component_Texture_Planet", pComponent)))
			return E_FAIL;
	}
	{
		//For.Setting_SoundMgr
		//DH.SoudMgrSetting_180705_LoadSoundFile
		if (FAILED(Engine::SoundMgr::GetInstance()->LoadSoundFile()))
			return E_FAIL;
	}

	g_bLoading = true;

	return 0;
}

BackLogo * BackLogo::Create(LPDIRECT3DDEVICE9 pDevice)
{
	BackLogo* pInstance = new BackLogo(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Back_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * BackLogo::Clone()
{
	BackLogo* pInstance = new BackLogo(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Back_Logo Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD BackLogo::Free()
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pRectTex);
	Engine::Safe_Release(m_pTexture);


	Engine::GameObject::Free();

	return 0;
}
