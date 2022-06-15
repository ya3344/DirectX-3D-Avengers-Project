#include "stdafx.h"
#include "PartUI.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
_USING(Client)

PartUI::PartUI(LPDIRECT3DDEVICE9 pDevice)
	:UI(pDevice)
{
}

PartUI::PartUI(const PartUI & rhs)
	: UI(rhs)
	,mAcceleration(rhs.mAcceleration)
{

}

HRESULT PartUI::Init()
{
	mUIInfo.X = -640.f; // 640.f
	mUIInfo.Y = 384.f; // 384.f
	mUIInfo.sizeX = 1280.f;
	mUIInfo.sizeY = 234.f;

	
	if (FAILED(ReadyComponent()))
	{
		_MSG_BOX("Ready UI Failed");
	}
	m_pTransform->SetScale(mUIInfo.sizeX, mUIInfo.sizeY, 1.f);
	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(mUIInfo.X, mUIInfo.Y, 0.f));
	mTextureNum = 1;
	mAcceleration = 10.f;
	return NOERROR;
}

UPDATE PartUI::Update(const float timeDelta)
{
	if (gTerrainState != TERRAIN_STATE::STAGE1_UI && gTerrainState != TERRAIN_STATE::STAGE2_UI)
		return Engine::GameObject::Update(timeDelta);

	mAcceleration += 300.f * timeDelta;
	mLastTime += timeDelta;

	if (mLastTime > 3.f)
	{
		if (mUIInfo.X < 640.f)
		{
			mUIInfo.X += mAcceleration * timeDelta;

			if (mUIInfo.X >= 640.f)
			{
				mUIInfo.X = 640.f;
			}

			m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(mUIInfo.X, mUIInfo.Y, 0.f));
		}
	}

	if (mLastTime > 6.5f)
	{
		mUIInfo.X = -640.f;
		mLastTime = 0.f;
		mAcceleration = 10.f;
		mTextureNum = 2;
		m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(mUIInfo.X, mUIInfo.Y, 0.f));
		if (gTerrainState == TERRAIN_STATE::STAGE1_UI)
		{
			gTerrainState = TERRAIN_STATE::IRON_FALL;
		}
		else if (gTerrainState == TERRAIN_STATE::STAGE2_UI)
		{
			gTerrainState = TERRAIN_STATE::THANOS_FALL;
		}
	}
	

	return Engine::GameObject::Update(timeDelta);
}

UPDATE PartUI::LastUpdate(const float timeDelta)
{
	return UI::LastUpdate(timeDelta);
}

void PartUI::Render()
{
	if (gTerrainState == TERRAIN_STATE::STAGE1_UI || gTerrainState == TERRAIN_STATE::STAGE2_UI)
	{
		UI::Render();
	}
}

HRESULT PartUI::ReadyPrototype()
{
	return UI::ReadyPrototype();
}

PartUI * PartUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	PartUI* pInstance = new PartUI(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("PartUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

PartUI * PartUI::Clone()
{
	PartUI*	pInstance = new PartUI(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("PartUI Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD PartUI::Free()
{
	UI::Free();
	return 0;
}
