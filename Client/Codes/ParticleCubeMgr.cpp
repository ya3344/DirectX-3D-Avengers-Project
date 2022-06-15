#include "stdafx.h"
#include "ParticleCubeMgr.h"
#include "ParticleCube.h"
#include "ComponentMgr.h"
#include "ObjectMgr.h"
#include "GameObject.h"
#include "Layer.h"
#include "GraphicDevice.h"
_USING(Client)

_IMPLEMENT_SINGLETON(ParticleCubeMgr)

ParticleCubeMgr::ParticleCubeMgr()
{
}

HRESULT ParticleCubeMgr::Init()
{
	m_pDevice = Engine::GraphicDevice::GetInstance()->GetDevice();
	m_pDevice->AddRef();

	mParticleInfo_Space.reserve(100);

	return NOERROR;
}

UPDATE ParticleCubeMgr::Update(const float timeDelta)
{
	auto iterBegin_vec = mParticleInfo_Space.begin();
	auto iterEnd_vec = mParticleInfo_Space.end();

	for (; iterBegin_vec != iterEnd_vec;)
	{
		(*iterBegin_vec).accTime += timeDelta;

		if ((*iterBegin_vec).durationTime < (*iterBegin_vec).accTime)
		{
			iterBegin_vec = mParticleInfo_Space.erase(iterBegin_vec);
			iterEnd_vec = mParticleInfo_Space.end();
		}
		else
		{
			AddParticle((*iterBegin_vec));
			++iterBegin_vec;
		}
	}

	for (ParticleCube* pCube : mParticleSpace)
	{
		pCube->Update(timeDelta);
	}

	// 파티클 Dead 체크해서 삭제하기 
	auto& iterBegin = mParticleSpace.begin();
	auto& iterEnd = mParticleSpace.end();

	for (; iterBegin != iterEnd; )
	{
		if ((*iterBegin)->IsDead())
		{
			Safe_Release(*iterBegin);
			iterBegin = mParticleSpace.erase(iterBegin);
		}
		else
		{
			++iterBegin;
		}
	}

	return UPDATE::NORMAL;
}

UPDATE ParticleCubeMgr::LastUpdate(const float timeDelta)
{
	for (ParticleCube* pCube : mParticleSpace)
	{
		pCube->LastUpdate(timeDelta);
	}

	return UPDATE::NORMAL;
}

void ParticleCubeMgr::Render()
{
	for (ParticleCube* pCube : mParticleSpace)
	{
		pCube->Render();
	}
}

// TY_FixParticle_180702 Modify Parameter
HRESULT ParticleCubeMgr::Add_PartiCleCube(const PARTICLE_INFO& particleInfo)
{
	mParticleInfo_Space.emplace_back(particleInfo);

	return NOERROR;
}

HRESULT ParticleCubeMgr::AddParticle(const PARTICLE_INFO & particleInfo)
{
	Engine::GameObject* pGameObject = m_pParticleCube = ParticleCube::Create(m_pDevice);
	if (m_pParticleCube == nullptr)
		return E_FAIL;

	m_pParticleCube->SetDir(particleInfo.vDir);
	m_pParticleCube->SetPos(particleInfo.vPos);
	m_pParticleCube->SetRange(particleInfo.range);
	m_pParticleCube->SetColor(particleInfo.colorNum);

	mParticleSpace.emplace_back(m_pParticleCube);

	return NOERROR;
}

DWORD ParticleCubeMgr::Free()
{
	for (ParticleCube* pCube : mParticleSpace)
	{
		Engine::Safe_Release(pCube);
	}

	mParticleSpace.clear();

	Engine::Safe_Release(m_pDevice);
	return 0;
}

