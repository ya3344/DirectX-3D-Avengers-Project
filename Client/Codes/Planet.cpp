#include "stdafx.h"
#include "Planet.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"

_USING(Client)

Planet::Planet(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{

}

Planet::Planet(const Planet & rhs)
	:GameObject(rhs)
	, mPlanetInfo(rhs.mPlanetInfo)
	, m_matInfo(rhs.m_matInfo)
	,m_matRevolRot(rhs.m_matRevolRot)
	,m_matParent(rhs.m_matParent)
	,m_matRotX(rhs.m_matRotX)
{

}

HRESULT Planet::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	D3DXMatrixScaling(&m_matInfo.matScale, mPlanetInfo.vScale.x, mPlanetInfo.vScale.y, mPlanetInfo.vScale.z);
	D3DXMatrixTranslation(&m_matInfo.matTrans, mPlanetInfo.vPos.x, mPlanetInfo.vPos.y, mPlanetInfo.vPos.z);
	D3DXMatrixTranslation(&m_matParent, 15.f, 0.f, 15.f); // stage1 y축 : 0 , stage2 y축: -30

	m_matInfo.matWorld = m_pTransform->GetWolrdPointer();

	return NO_ERROR;
}

UPDATE Planet::Update(const float timeDelta)
{
	mPlanetInfo.rotDegree += mPlanetInfo.rotSpeed * timeDelta;
	mPlanetInfo.revolRotDegree += mPlanetInfo.revolSpeed * timeDelta;

	//D3DXMatrixRotationX(&m_matRotX, D3DXToRadian(mPlanetInfo.rotDegree));
	D3DXMatrixRotationY(&m_matInfo.matRot, D3DXToRadian(mPlanetInfo.rotDegree));
	D3DXMatrixRotationY(&m_matRevolRot, D3DXToRadian(mPlanetInfo.revolRotDegree));
	MakeMatrix();
	return GameObject::Update(timeDelta);
}

UPDATE Planet::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	return GameObject::LastUpdate(timeDelta);;
}

void Planet::Render()
{
	if (m_pBuffer == nullptr)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//m_pDevice->SetRenderState(D3DRS_ZENABLE, false);

	m_pTexture->SetUp_Device(mPlanetInfo.textureNum);
	m_pTransform->SetUp_World();

	m_pBuffer->Render();
	//m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT Planet::ReadyPrototype(const PLANET_INFO& planetInfo)
{
	mPlanetInfo = planetInfo;
	return NO_ERROR;
}

void Planet::Set_PlanetInfo(const PLANET_INFO & planetInfo)
{
	mPlanetInfo = planetInfo;

	D3DXMatrixTranslation(&m_matInfo.matTrans, mPlanetInfo.vPos.x, mPlanetInfo.vPos.y, mPlanetInfo.vPos.z);
	D3DXMatrixTranslation(&m_matParent, 15.f, -30.f, 15.f); // stage1 y축 : 0 , stage2 y축: -30
}

void Planet::Set_CenterPos()
{
	D3DXMatrixTranslation(&m_matParent, 15.f, -30.f, 15.f); // stage1 y축 : 0 , stage2 y축: -30
}

HRESULT Planet::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

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

	// For.Com_Buffer
	pComponent = m_pBuffer = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeTexture", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_PLANENT, L"Component_Texture_Planet");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_Planet", pComponent)))
		return E_FAIL;

	return NOERROR;
}

void Planet::MakeMatrix()
{
	*m_matInfo.matWorld = m_matInfo.matScale * m_matInfo.matRot /** m_matRotX*/  * m_matInfo.matTrans * m_matRevolRot * m_matParent;
}

Planet * Planet::Create(LPDIRECT3DDEVICE9 pDevice, const PLANET_INFO& planetInfo)
{
	Planet* pInstance = new Planet(pDevice);

	if (FAILED(pInstance->ReadyPrototype(planetInfo)))
	{
		_MSG_BOX("Object Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Planet * Planet::Clone()
{
	Planet*	pInstance = new Planet(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Object Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Planet::Free()
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pTexture);

	GameObject::Free();

	return DWORD();
}
