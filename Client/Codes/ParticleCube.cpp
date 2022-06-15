#include "stdafx.h"
#include "ParticleCube.h"
#include "ComponentMgr.h"

_USING(Client)

ParticleCube::ParticleCube(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
{
}

ParticleCube::ParticleCube(const ParticleCube & rhs)
	: Engine::GameObject(rhs)
{
	m_vPos = rhs.m_vPos;
	m_vDir = rhs.m_vDir;

	mObjectName = rhs.mObjectName;
}

HRESULT ParticleCube::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	m_vPos = D3DXVECTOR3{ 0.f, 0.f, 0.f };
	m_vDir = D3DXVECTOR3{ float((rand() % 20) - 10), 1.f, float((rand() % 20) - 10) };

	mAlpha = 255.f;
	return NOERROR;
}

UPDATE ParticleCube::Update(const float timeDelta)
{
	mAccTime += timeDelta;

	m_vPos += m_vDir*mRange;
	mAccY = 5.f*sinf(D3DXToRadian(90.f))*mAccTime - 0.5f*9.8f*mAccTime*mAccTime;

	m_vPos.y = mOriginY + mAccY;

	//mColor = D3DCOLOR_ARGB(int(mAlpha), 255, 0, 0);

	//mAlpha -= 0.1f;
	//m_pBuffer->SetColor(mColor);

	if((gTerrainState <= TERRAIN_STATE::STAGE1_PLAY && m_vPos.y < 0.f)
		|| (gTerrainState <= TERRAIN_STATE::STAGE2_PLAY && m_vPos.y < -30.f))
	{
		mDead = true;
	}

	return Engine::GameObject::Update(timeDelta);
}

UPDATE ParticleCube::LastUpdate(const float timeDelta)
{
	SetWorldMatrix();
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_ALPHA, this)))
		return UPDATE::FAIL;

	return Engine::GameObject::LastUpdate(timeDelta);
}

void ParticleCube::Render()
{
	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	//m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
	m_pTransform->SetUp_World();
	m_pTexture->SetUp_Device(texNum);
	m_pBuffer->Render();
	//m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

}

HRESULT ParticleCube::Ready_GameObjectPrototype()
{
	Init();

	Engine::Component* pComponent = nullptr;

	return NOERROR;
}

HRESULT ParticleCube::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	mMatrixInfo.matWorld = m_pTransform->GetWolrdPointer();
	if (mMatrixInfo.matWorld == nullptr)
	{
		_MSG_BOX("TerrainCube ReadyComponent Failed");
		return E_FAIL;
	}

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
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_PARTICLE, L"Component_Texture_Color");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_Color", pComponent)))
		return E_FAIL;


	return NOERROR;
}

void ParticleCube::SetWorldMatrix()
{
	if (mMatrixInfo.matWorld == nullptr)
	{
		_MSG_BOX("TerrainCube SetWorldMatrix Failed");
	}
	D3DXMatrixIdentity(mMatrixInfo.matWorld);

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, m_vPos.x, m_vPos.y, m_vPos.z);
	D3DXMatrixScaling(&mMatrixInfo.matScale, 0.2f, 0.2f, 0.2f);


	*mMatrixInfo.matWorld = mMatrixInfo.matScale * mMatrixInfo.matTrans;
}

void ParticleCube::SetDir(const D3DXVECTOR3 & vDir)
{
	m_vDir = D3DXVECTOR3{ float((rand() % 10) - 5), float((rand() % 10) - 5), float((rand() % 10) - 5) };
	D3DXVec3Normalize(&m_vDir, &m_vDir);

	m_vDir += vDir*5.f;
}

void ParticleCube::SetPos(const D3DXVECTOR3 & vPos)
{
	//TY_FixParticle_180705_ Set Random pos
	float randomNum = float(rand() % 30 - 15)*0.05f;
	m_vPos = { vPos.x + randomNum, vPos.y + randomNum, vPos.z + randomNum };

	mOriginY = vPos.y;
}

// TY_FixBullet_180630_ Change Parameter
void ParticleCube::SetColor(const int colorTexNum)
{
	texNum = colorTexNum;
}

void ParticleCube::SetRange(const int range)
{
	mRange = float(rand() % range + 1)*0.01f;
}

ParticleCube * ParticleCube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	ParticleCube* pInstance = new ParticleCube(pDevice);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
	{
		_MSG_BOX("ParticleCube Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * ParticleCube::Clone()
{
	AddRef();
	return this;
}

DWORD ParticleCube::Free(void)
{
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pTexture);

	GameObject::Free();

	return 0;
}
