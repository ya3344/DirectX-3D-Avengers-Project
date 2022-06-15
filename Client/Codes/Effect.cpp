#include "stdafx.h"
#include "Effect.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"

_USING(Client)


Effect::Effect(LPDIRECT3DDEVICE9 pDevice)
	:GameObject(pDevice)
{
}

Effect::Effect(const Effect & rhs)
	:GameObject(rhs)
	,mEffect_InitInfo(rhs.mEffect_InitInfo)
	,mEffectInfo(rhs.mEffectInfo)
	,m_bRender(rhs.m_bRender)
{
}

HRESULT Effect::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	/*m_pTransCom->Set_Rotation(D3DXToRadian(45.0f));*/
	//m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &mEffectInfo.vPos);
	//m_pTransform->SetScale(mEffectInfo.vScale.x, mEffectInfo.vScale.y, mEffectInfo.vScale.z);

	return NOERROR;
}

UPDATE Effect::Update(const float timeDelta)
{
	if (m_bRender == false)
		return GameObject::Update(timeDelta);

	if (mFrame < mEffect_InitInfo.frameMax)
	{
		mFrame += mEffect_InitInfo.frameSpeed * timeDelta;
	}

	if (mFrame >= mEffect_InitInfo.frameMax)
	{
		mFrame = 0.f;

		if (mEffect_InitInfo.bForever == false)
		{
			m_bRender = false;
		}
	}


	Compute_ViewZ(m_pTransform->GetInfo(Engine::Transform::INFO_POSITION));

	return GameObject::Update(timeDelta);
}

UPDATE Effect::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		UPDATE::FAIL;

	if (FAILED(SetUp_BillBoard()))
		UPDATE::FAIL;

	if (m_bRender == false)
		GameObject::LastUpdate(timeDelta);;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_ALPHA, this)))
		return UPDATE::FAIL;
		

	return GameObject::LastUpdate(timeDelta);;
}

void Effect::Render()
{
	if (m_pBuffer == nullptr)
		return;

	if (m_bRender == false)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, false);
	//pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 100);
	//pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pTexture->SetUp_Device((size_t)mFrame);
	m_pTransform->SetUp_World();

	m_pBuffer->Render();
	m_pDevice->SetRenderState(D3DRS_ZENABLE, true);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

HRESULT Effect::ReadyPrototype(const EFFECT_INIT_INFO& effect_InitInfo)
{
	if (effect_InitInfo.pTextureName == nullptr)
		return E_FAIL;

	mEffect_InitInfo = effect_InitInfo;

	return NOERROR;
}

void Effect::EffectOperation(const EFFECT_INFO& effectInfo)
{
	D3DXVECTOR3 vResultPos = effectInfo.vPos + effectInfo.vRandomPos;

	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &vResultPos);
	m_pTransform->SetScale(effectInfo.vScale.x, effectInfo.vScale.y, effectInfo.vScale.z);

	m_bRender = true;
}

HRESULT Effect::ReadyComponent(void)
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
	pComponent = m_pBuffer = (Engine::RectTex*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Buffer_RectTexture");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Buffer_RectTexture", pComponent)))
		return E_FAIL;

	// For.Com_Texture
	pComponent = m_pTexture = (Engine::Texture*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_PARTICLE, mEffect_InitInfo.pTextureName);
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(mEffect_InitInfo.pTextureName, pComponent)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Effect::SetUp_BillBoard(void)
{
	D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, nullptr, &matView);

	D3DXVECTOR3	vScale = m_pTransform->GetScale();

	m_pTransform->SetInfo(Engine::Transform::INFO_RIGHT, &(*(D3DXVECTOR3*)&matView.m[0][0] * vScale.x));
	//m_pTransCom->Set_Infomation(CTransform::INFO_UP, &(*(_vec3*)&matView.m[1][0] * vScale.y));
	m_pTransform->SetInfo(Engine::Transform::INFO_LOOK, &(*(D3DXVECTOR3*)&matView.m[2][0] * vScale.z));

	return NOERROR;
}

Effect * Effect::Create(LPDIRECT3DDEVICE9 pDevice, EFFECT_INIT_INFO effect_InitInfo)
{
	Effect* pInstance = new Effect(pDevice);

	if (FAILED(pInstance->ReadyPrototype(effect_InitInfo)))
	{
		_MSG_BOX("Effect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Effect * Effect::Clone()
{
	Effect*	pInstance = new Effect(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Effect Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Effect::Free()
{
	Engine::Safe_Release(m_pTransform);
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pBuffer);
	Engine::Safe_Release(m_pTexture);

	GameObject::Free();

	return DWORD();
}
