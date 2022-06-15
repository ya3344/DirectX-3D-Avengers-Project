#include "stdafx.h"
#include "TerrainCube.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
_USING(Client)


TerrainCube::TerrainCube(LPDIRECT3DDEVICE9 pDevice)
	: Engine::GameObject(pDevice)
{

}

//TY_TerrainCube Contructor_180615 Copy Variables
TerrainCube::TerrainCube(const TerrainCube & rhs)
	: Engine::GameObject(rhs)
{
	mTextureNum = rhs.mTextureNum;
	m_vPos = rhs.m_vPos;
	m_vRot = rhs.m_vRot;
	m_vRevol = rhs.m_vRevol;
	m_matParent = rhs.m_matParent;

	mObjectName = rhs.mObjectName;
}

HRESULT TerrainCube::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	//TY_Terrain Init_180615 Initialize Matrix
	D3DXMatrixIdentity(&m_matParent);
	D3DXMatrixIdentity(&m_matRevolve);

	m_pTransform->SetRotation(D3DXToRadian(45.0f));
	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(rand() % 10 + 5.f, rand() % 10 + 5.f, rand() % 10 + 5.f));

	return NOERROR;
}

UPDATE TerrainCube::Update(const float timeDelta)
{
	return Engine::GameObject::Update(timeDelta);
}

UPDATE TerrainCube::LastUpdate(const float timeDelta)
{
	if (m_bSetTransform)
	{
		SetWorldMatrix();
	}
	
	return Engine::GameObject::LastUpdate(timeDelta);
}

void TerrainCube::Render()
{
	m_pTexture->SetUp_Device(mTextureNum);
	m_pTransform->SetUp_World();
	m_pBuffer->Render();
}

HRESULT TerrainCube::Ready_GameObjectPrototype()
{
	Engine::Component* pComponent = nullptr;

	return NOERROR;
}

HRESULT TerrainCube::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	//TY_TerrainCube ReadyComponent_180615 Initialize matWorld Matrix
	mMatrixInfo.matWorld = m_pTransform->GetWolrdPointer();
	if (mMatrixInfo.matWorld == nullptr)
	{
		_MSG_BOX("TerrainCube ReadyComponent Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Buffer
	pComponent = m_pBuffer = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeTexture", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_TERRAIN, L"Component_Texture_Terrain");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_Terrain", pComponent)))
		return E_FAIL;

	return NO_ERROR;
}

//TY_SetWorldMatrix_180614 Add SetWorldMatrix Function
void TerrainCube::SetWorldMatrix()
{
	if (mMatrixInfo.matWorld == nullptr)
	{
		_MSG_BOX("TerrainCube SetWorldMatrix Failed");
	}
	D3DXMatrixIdentity(mMatrixInfo.matWorld);

	D3DXQUATERNION qRot;
	D3DXQuaternionRotationYawPitchRoll(&qRot, m_vRot.y, m_vRot.x, m_vRot.z);
	D3DXMatrixRotationQuaternion(&mMatrixInfo.matRot, &qRot);

	D3DXMatrixTranslation(&mMatrixInfo.matTrans, m_vPos.x, m_vPos.y, m_vPos.z);

	D3DXQUATERNION qRevol;
	D3DXQuaternionRotationYawPitchRoll(&qRevol, m_vRevol.y, m_vRevol.x, m_vRevol.z);
	D3DXMatrixRotationQuaternion(&m_matRevolve, &qRevol);

	*mMatrixInfo.matWorld = mMatrixInfo.matRot * mMatrixInfo.matTrans * m_matRevolve * m_matParent;
}


TerrainCube * TerrainCube::Create(LPDIRECT3DDEVICE9 pDevice)
{
	TerrainCube* pInstance = new TerrainCube(pDevice);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
	{
		_MSG_BOX("TerrainCube Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * TerrainCube::Clone()
{
	TerrainCube* pInstance = new TerrainCube(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("TerrainCube Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD TerrainCube::Free(void)
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pTexture);

	GameObject::Free();

	return DWORD();
}
