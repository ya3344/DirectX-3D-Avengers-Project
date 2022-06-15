#include "stdafx.h"
#include "FullUI.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
_USING(Client)

FullUI::FullUI(LPDIRECT3DDEVICE9 pDevice)
	:UI(pDevice)
{
}

FullUI::FullUI(const FullUI & rhs)
	: UI(rhs)
{

}

HRESULT FullUI::Init()
{
	mUIInfo.X = 640.f;
	mUIInfo.Y = 384.f;
	mUIInfo.sizeX = 1280.f;
	mUIInfo.sizeY = 768.f;

	
	if (FAILED(ReadyComponent()))
	{
		_MSG_BOX("Ready UI Failed");
	}

	m_pTransform->SetScale(mUIInfo.sizeX, mUIInfo.sizeY, 1.f);
	m_pTransform->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3(mUIInfo.X, mUIInfo.Y, 0.f));
	mTextureNum = 0;
	return NOERROR;
}

UPDATE FullUI::Update(const float timeDelta)
{
	return Engine::GameObject::Update(timeDelta);


}

UPDATE FullUI::LastUpdate(const float timeDelta)
{
	return UI::LastUpdate(timeDelta);
}

void FullUI::Render()
{
	if (gTerrainState == TERRAIN_STATE::STAGE1_UI || gTerrainState == TERRAIN_STATE::STAGE2_UI)
	{
		mTextureNum = 0;
		UI::Render();
	}
	else if (m_pKeyMgr->GetInstance()->KeyPressing(DIK_B))
	{
		mTextureNum = 3;
		UI::Render();
	}
	else if (gTerrainState == TERRAIN_STATE::TERRAIN_END)
	{
		mTextureNum = 4;
		UI::Render(true);
	}
	//DH_SoundAdd_180706_SoundAdd
	if (Engine::KeyMgr::GetInstance()->KeyDown(DIK_B))
	{
		m_pSoundMgr->SoundPlay(L"comet_blade_finish.ogg", Engine::SoundMgr::UI);
		m_pSoundMgr->SoundPlay_LoopUI(L"bloodmarble_beat.ogg");
		m_pSoundMgr->SetVolume(Engine::SoundMgr::BGM, 0.2f);
	}
	if (Engine::KeyMgr::GetInstance()->KeyUp(DIK_B))
	{
		m_pSoundMgr->StopSound(Engine::SoundMgr::UI_EFFECT);
		m_pSoundMgr->SetVolume(Engine::SoundMgr::BGM, 1.f);
	}
	
}

HRESULT FullUI::ReadyPrototype()
{
	return UI::ReadyPrototype();
}

FullUI * FullUI::Create(LPDIRECT3DDEVICE9 pDevice)
{
	FullUI* pInstance = new FullUI(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("FullUI Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

FullUI * FullUI::Clone()
{
	FullUI*	pInstance = new FullUI(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("PartUI Clone Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD FullUI::Free()
{
	UI::Free();
	return 0;
}
