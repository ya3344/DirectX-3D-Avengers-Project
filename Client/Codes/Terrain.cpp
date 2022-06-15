#include "stdafx.h"
#include "Terrain.h"
#include "ObjectMgr.h"
#include "ComponentMgr.h"
#include "CameraMgr.h"
#include "TerrainCube.h"
#include "Layer.h"
#include "TerrainAnim.h"
#include "AnimCube.h"
#include "Boss.h" 
#include "HpBar.h"

_USING(Client)

//TY_ChangeStage_180703_ Add Terrain State Enum 
TERRAIN_STATE  gTerrainState = TERRAIN_STATE::TERRAIN_END;

Terrain::Terrain(LPDIRECT3DDEVICE9 pDevice)
	: GameObject(pDevice)
	, m_pSoundMgr(Engine::SoundMgr::GetInstance())
{
	m_pSoundMgr->AddRef();
}

Terrain::Terrain(const Terrain & rhs)
	: GameObject(rhs)
	, m_pSoundMgr(rhs.m_pSoundMgr)
{

}

HRESULT Terrain::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	ZeroMemory(&mMtrlInfo, sizeof(D3DMATERIAL9));
	mMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	mMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	m_pDevice->SetMaterial(&mMtrlInfo);
	m_pCameraMgr = CameraMgr::GetInstance();

	m_bTornado = true;
	m_bVibrate = true;

	m_vChangedPos = D3DXVECTOR3{ 0.f, -30.f, 0.f };

	AnimCube * pPlayerAnim = (AnimCube*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_PLAYER, L"Layer_Player_AnimCube", L"Waist");
	m_pPlayerTrans = pPlayerAnim->GetTransform();

	AnimCube* pBossAnim = (AnimCube*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_BOSS, L"Layer_Boss_AnimCube", L"Waist");
	m_pBossTrans = pBossAnim->GetTransform();

	// JH_AnimFrame_180624 BossObject Get
	m_pBossObj = (Boss*)Engine::ObjectMgr::GetInstance()->GetGameObject(SCENE_STAGE, L"Layer_Boss");

	// TY_FixBullet_180706_ Set True
	m_bSetTerrainPos = true;

#ifdef _TERRAIN_INIT_DEBUG
	// Initialize Intro_Debug
	m_bTornado = false;
	m_bVibrate = false;
	m_pTerrainAnim->SetTerrainCubeToZERO_DEBUG();

	m_bBossFall = false;

	gTerrainState = TERRAIN_STATE::STAGE1_PLAY;
#else
	gTerrainState = TERRAIN_STATE::TORNADO;
#endif
	return NOERROR;
}

UPDATE Terrain::Update(const float timeDelta)
{
	mTimeDelta = timeDelta;
	m_pTerrainAnim->Update(timeDelta);

	return GameObject::Update(timeDelta);
}

UPDATE Terrain::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr || m_pTerrainAnim == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_NONALPHA, this)))
		return UPDATE::FAIL;

	switch (gTerrainState)
	{
	case TERRAIN_STATE::TORNADO:
		gTerrainState = m_pTerrainAnim->TornadoCube();
		break;
	case TERRAIN_STATE::PLAYER_FALL_FIRST:
		m_bPlayerFall = TransGameObject(m_pPlayerTrans, &D3DXVECTOR3{ 15.f, 1.6f, 2.f }, 40.f);
		if (!m_bPlayerFall)
		{
			//DH_SoundAdd_180706_SoundAdd
			m_pSoundMgr->SoundPlay(L"nensu_crash.ogg", Engine::SoundMgr::EFFECT);
			m_pCameraMgr->ShakeCamera(1.f);
			gTerrainState = TERRAIN_STATE::STAGE1_UI;
		}
		break;
	case TERRAIN_STATE::IRON_FALL:
		m_bBossFall = TransGameObject(m_pBossTrans, &D3DXVECTOR3{ 15.f, 2.5f, 25.f }, 60.f);
		if (!m_bBossFall) // JH_AnimFrame_180624 AI BossLanding True
		{
			// TY_FixBullet_180706_ 무조건 한 번 돌리기
			m_pTerrainAnim->SetTerrainCubeToZERO_DEBUG();
			m_pCameraMgr->ShakeCamera(1.f);
			gTerrainState = TERRAIN_STATE::STAGE1_PLAY;
		}
		break;
	case TERRAIN_STATE::STAGE1_PLAY:
		if (m_bSetTerrainPos == true)
		{
			m_pTerrainAnim->SetCubeState_Transform(false);
			m_bSetTerrainPos = false;
		}
		break;
	case TERRAIN_STATE::VIBRATE:
		m_pCameraMgr->InitActionCam(L"path_data_StageChange");
		gTerrainState = m_pTerrainAnim->DropCube(m_vChangedPos);
		break;
	case TERRAIN_STATE::PLAYER_FALL_SECOND:
		m_bPlayerFall = TransGameObject(m_pPlayerTrans, &D3DXVECTOR3{ 15.f, -28.4f, 2.f }, 40.f);
		if (!m_bPlayerFall)
		{
			//DH_SoundAdd_180706_SoundAdd
			m_pSoundMgr->SoundPlay(L"nensu_crash.ogg", Engine::SoundMgr::EFFECT);
			m_pCameraMgr->ShakeCamera(1.f);
			m_pBossTrans = m_pBossObj->Get_SuperCubeInfo()->pTransform;
			gTerrainState = TERRAIN_STATE::STAGE2_UI;
		}
		break;
	case TERRAIN_STATE::THANOS_FALL:
		m_bBossFall = TransGameObject(m_pBossTrans, &D3DXVECTOR3{ 15.f, -26.5f, 25.f }, 60.f);
		if (!m_bBossFall)
		{
			//DH_SoundAdd_180706_SoundAdd
			m_pSoundMgr->SoundPlay(L"nensu_crash.ogg", Engine::SoundMgr::EFFECT);
			m_pCameraMgr->ShakeCamera(1.f);
			gTerrainState = TERRAIN_STATE::STAGE2_PLAY;
		}
		break;
	case TERRAIN_STATE::STAGE2_PLAY:
		break;
	}

	//TY_ChangeStage_180706_ 시연용 치트키
	if (m_pKeyMgr->KeyDown(DIK_F9) && gTerrainState <= TERRAIN_STATE::STAGE1_PLAY)
	{
		m_pTerrainAnim->SetTerrainCubeToZERO_DEBUG();
		m_pPlayerTrans->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ 15.f, 1.6f, 2.f });
		m_pBossTrans->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ 15.f, 2.5f, 25.f });

		gTerrainState = TERRAIN_STATE::STAGE1_PLAY;
		m_pCameraMgr->ChangeCamera(L"Prototype_GameObject_CameraTps");
	}
	else if (m_pKeyMgr->KeyDown(DIK_F10) && gTerrainState <= TERRAIN_STATE::STAGE2_PLAY)
	{
		m_pTerrainAnim->SetTerrainCubeToZERO_DEBUG(&m_vChangedPos);
		m_pPlayerTrans->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ 15.f, -28.4f, 2.f });
		m_pBossTrans->SetInfo(Engine::Transform::INFO_POSITION, &D3DXVECTOR3{ 15.f, -26.5f, 25.f });

		gTerrainState = TERRAIN_STATE::STAGE2_PLAY;
		m_pCameraMgr->ChangeCamera(L"Prototype_GameObject_CameraTps");
	}

	return GameObject::LastUpdate(timeDelta);
}

void Terrain::Render()
{

	Engine::Layer* pLayer = Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_TERRAIN, L"Layer_Terrain_TerrainCube");

	if (pLayer == nullptr)
	{
		_MSG_BOX("Terrain Render Failed");
		return;
	}

	pLayer->GameObjectRender();

	// TY_FixBullet_180701 Call Function To Set Terrain Not To Move
	if (m_bTerrainTransform_DEBUG)
	{
		m_pTerrainAnim->SetCubeState_Transform(false);
		m_bTerrainTransform_DEBUG = false;
	}

	//m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
}

HRESULT Terrain::ReadyPrototype()
{
	Engine::Component* pComponent = nullptr;



	Ready_LayerTerrainCube();

	return NOERROR;
}

HRESULT Terrain::Ready_LayerTerrainCube()
{
	Engine::GameObject* pGameObject = nullptr;

	return NOERROR;
}

HRESULT Terrain::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	if (FAILED(Engine::GameObject::AddComponent(L"Component_Renderer", pComponent)))
		return E_FAIL;

	FileLoad();

	return NOERROR;
}

void Terrain::FileLoad()
{
	if (m_pDevice == nullptr)
	{
		_MSG_BOX("Ready Object Failed");
	}

	HANDLE hFile = CreateFile(L"../Bin/Data/TerrainData/TerrainCube.dat", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	DWORD byte = 1;
	D3DXMATRIX matParent;
	TCHAR namePath[MAX_PATH] = L"";
	static size_t count = 0;

	D3DXMatrixIdentity(&matParent);
	D3DXMatrixTranslation(&matParent, 15.f, 0.f, 15.f);

	size_t colCube_Size = 0;
	mTexCube_Size = 0;

	ReadFile(hFile, &colCube_Size, sizeof(int), &byte, nullptr);
	ReadFile(hFile, &mTexCube_Size, sizeof(int), &byte, nullptr);

	m_pOriginPos = new D3DXVECTOR3[mTexCube_Size];
	Engine::GameObject* pGameObject = nullptr;
	TerrainCube* pCube = nullptr;

	mTerrainCube_Space.reserve(mTexCube_Size);

	for (size_t i = 0; i < mTexCube_Size; ++i)
	{
		if (byte == 0)
		{
			break;
		}

		pCube = TerrainCube::Create(m_pDevice);
		D3DXVECTOR3 vbufferPos;
		int	texNum;

		ReadFile(hFile, &vbufferPos, sizeof(D3DXVECTOR3), &byte, nullptr);
		ReadFile(hFile, &texNum, sizeof(int), &byte, nullptr);

		pCube->SetPos({ 20.f + float(rand() % 10), float(rand() % 10), 20.f + float(rand() % 10) });
		pCube->SetTexNum(texNum);

		pCube->SetRotRadian(D3DXVECTOR3{ D3DXToRadian(rand() % 360), D3DXToRadian(rand() % 360), D3DXToRadian(rand() % 360) });
		pCube->SetRevolRadian(D3DXVECTOR3{ 0.f , D3DXToRadian(rand() % 360) , 0.f });
		pCube->SetParentMatrix(matParent);

		wsprintf(namePath, L"Terrain_Cube%d", count);
		mNameSpace.emplace_back(namePath);
		pCube->setName(namePath);

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(mNameSpace[i].c_str(), SCENE_TERRAIN, pCube)))
		{
			_MSG_BOX("Terrain File Load Failed");
		}

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(SCENE_TERRAIN, mNameSpace[i].c_str(), SCENE_TERRAIN, L"Layer_Terrain_TerrainCube")))
		{
			_MSG_BOX("Terrain File Load Failed");
		}

		Engine::Layer* pLayer = Engine::ObjectMgr::GetInstance()->FindLayer(SCENE_TERRAIN, L"Layer_Terrain_TerrainCube");

		GameObject* pCube = pLayer->GetGameObject(namePath);
		mTerrainCube_Space.push_back((TerrainCube*)pCube);
		m_pOriginPos[i] = vbufferPos;
		++count;
	}

	CloseHandle(hFile);

	m_pTerrainAnim = TerrainAnim::Create();
	m_pTerrainAnim->Init();
	m_pTerrainAnim->SetTerrainCube(mTerrainCube_Space);
	m_pTerrainAnim->SetOriginalCubePos(m_pOriginPos);
}

bool Terrain::TransGameObject(Engine::Transform* pTrans, D3DXVECTOR3 * vPos, float fallPace)
{
	const D3DXVECTOR3* pPos = pTrans->GetInfo(Engine::Transform::INFO_POSITION);

	if (D3DXVec3Length(&(*pPos - *vPos)) <0.3f)
	{
		pTrans->SetInfo(Engine::Transform::INFO_POSITION, vPos);
		return false;
	}

	pTrans->TargetStep(*vPos, mTimeDelta*fallPace);

	return true;
}

Terrain * Terrain::Create(LPDIRECT3DDEVICE9 pDevice)
{
	Terrain* pInstance = new Terrain(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("Terrain Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * Terrain::Clone()
{
	Terrain* pInstance = new Terrain(*this);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Terrain Clone Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD Terrain::Free(void)
{
	Engine::Safe_Release(m_pRenderer);
	Engine::Safe_Release(m_pTerrainAnim);
	Engine::Safe_Release(m_pSoundMgr);
	Engine::Safe_Delete_Array(m_pOriginPos);

	mNameSpace.clear();

	Engine::GameObject::Free();

	return 0;
}
