#include "stdafx.h"
#include "HpBar.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "Camera.h"

_USING(Client)

HpBar::HpBar(LPDIRECT3DDEVICE9 pDevice)
	:Engine::GameObject(pDevice)
{
}

HpBar::HpBar(const HpBar & rhs)
	: Engine::GameObject(rhs)
{
}

HRESULT HpBar::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	mDeathCount = ONE_GREEN;

	return NOERROR;
}

UPDATE HpBar::Update(const float timeDelta)
{
	if (m_pCamera == nullptr)
	{
		m_pCamera = (Engine::Camera*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Camera", 0);
	}

	return Engine::GameObject::Update(timeDelta);
}

UPDATE HpBar::LastUpdate(const float timeDelta)
{

	return Engine::GameObject::LastUpdate(timeDelta);
}

void HpBar::Render()
{
	if (mDeathCount >= DEATH_COUNT::DEATH_END)
	{
		return;
	}

	Set_GreenWorldMatrix();
	m_pTransform->SetUp_World();
	m_pTexture->SetUp_Device(mDeathCount);
	m_pBufferGreen->Render();

	Set_RedWorldMatrix();
	m_pTransform->SetUp_World();
	m_pTexture->SetUp_Device(DEATH_COUNT::BACK_RED);
	m_pBufferRed->Render();

}

HRESULT HpBar::ReadyPrototype()
{
	Engine::Component* pComponent = nullptr;

	if (FAILED(Init()))
		return E_FAIL;

	return NOERROR;
}

HRESULT HpBar::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Transform
	pComponent = m_pTransform = (Engine::Transform*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Transform");
	if (pComponent == nullptr)
		return E_FAIL;

	mMatrixInfo.matWorld = m_pTransform->GetWolrdPointer();
	if (mMatrixInfo.matWorld == nullptr)
	{
		_MSG_BOX("HpBar ReadyComponent Failed");
		return E_FAIL;
	}

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Transform", pComponent)))
		return E_FAIL;

	// For.Com_Buffer
	pComponent = m_pBufferGreen = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeTexture", pComponent)))
		return E_FAIL;

	// For.Com_Buffer
	pComponent = m_pBufferRed = (Engine::CubeTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_CubeTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_CubeColor", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_PARTICLE, L"Component_Texture_Color");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Texture_Color", pComponent)))
		return E_FAIL;


	return NOERROR;
}

void HpBar::Set_GreenWorldMatrix()
{
	if (mMatrixInfo.matWorld == nullptr)
	{
		_MSG_BOX("HpBar SetWorldMatrix Failed");
	}

	//D3DXMATRIX matInView;
	//D3DXVECTOR3 vLook;

	//D3DXMatrixIdentity(mMatrixInfo.matWorld);

	//m_pDevice->GetTransform(D3DTS_VIEW, &matInView);;
	//D3DXMatrixInverse(&matInView, nullptr, &matInView);
	//
	//memcpy(&vLook, &matInView._31, sizeof(D3DXVECTOR3));
	//D3DXVec3Normalize(&vLook, &vLook);

	//float cameraRadian = acosf(D3DXVec3Dot(&(vLook), &(D3DXVECTOR3{0.f, 0.f, 1.f})));

	//if (cameraRadian <= 0)
	//	cameraRadian = 0;
	//else if (cameraRadian >= 360.f)
	//	cameraRadian = 360.f;

	//if (vLook.x < 0)
	//{
	//	cameraRadian = D3DXToRadian(360.f) - cameraRadian;
	//}

	//cout << D3DXToDegree(cameraRadian)<< endl;
	m_vPos = *m_pTargeInfo->GetInfo(Engine::Transform::INFO_POSITION);
	m_vPos.y += mHeight;

	D3DXMatrixScaling(&mMatrixInfo.matScale, mHp, m_vSize.y, m_vSize.z);
	D3DXMatrixRotationY(&mMatrixInfo.matRot, 0);
	D3DXMatrixTranslation(&mMatrixInfo.matTrans, m_vPos.x - (m_vSize.x - mHp)*0.5f, m_vPos.y, m_vPos.z);

	*mMatrixInfo.matWorld = mMatrixInfo.matScale * mMatrixInfo.matRot * mMatrixInfo.matTrans;
}

void HpBar::Set_RedWorldMatrix()
{
	if (mMatrixInfo.matWorld == nullptr)
	{
		_MSG_BOX("HpBar SetWorldMatrix Failed");
	}

	m_vPos = *m_pTargeInfo->GetInfo(Engine::Transform::INFO_POSITION);
	m_vPos.y += mHeight;

	D3DXMatrixScaling(&mMatrixInfo.matScale, m_vSize.x - mHp, m_vSize.y, m_vSize.z);
	D3DXMatrixRotationY(&mMatrixInfo.matRot, 0);
	D3DXMatrixTranslation(&mMatrixInfo.matTrans, m_vPos.x + mHp*0.5f, m_vPos.y, m_vPos.z);

	*mMatrixInfo.matWorld = mMatrixInfo.matScale * mMatrixInfo.matRot * mMatrixInfo.matTrans;
}

HpBar * HpBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	HpBar* pInstance = new HpBar(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("HpBar Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HpBar * HpBar::Clone()
{
	AddRef();
	return this;
}

DWORD HpBar::Free()
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pBufferGreen);
	Engine::Safe_Release(m_pBufferRed);
	Engine::Safe_Release(m_pTexture);

	GameObject::Free();

	return DWORD();
}

