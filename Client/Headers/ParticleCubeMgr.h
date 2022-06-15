
#ifndef PARTICLE_MGR_H__
#define PARTICLE_MGR_H__

#include "Base.h"

typedef struct tag_ParticleInfo
{
	tag_ParticleInfo()
	{}
	explicit tag_ParticleInfo(D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, int colorNum, int range, float durationTime)
	{
		this->vPos = vPos;
		this->vDir = vDir;
		this->colorNum = colorNum;
		this->range = range;
		this->durationTime = durationTime;
	}

	D3DXVECTOR3 vPos = D3DXVECTOR3{ 0.f, 0.f, 0.f };
	D3DXVECTOR3 vDir = D3DXVECTOR3{ 0.f, 0.f, 0.f };
	int colorNum = 0;
	int range = 0;
	float durationTime = 0.f;
	float accTime = 0.f;

}PARTICLE_INFO;


namespace Engine
{
	class Layer;
}

_BEGIN(Client)

class ParticleCube;
class ParticleCubeMgr final : public Engine::Base
{
	_DECLARE_SINGLETON(ParticleCubeMgr)
public:
	explicit ParticleCubeMgr();
	virtual ~ParticleCubeMgr() = default;

public:
	HRESULT Init();
	UPDATE Update(const float timeDelta);
	UPDATE LastUpdate(const float timeDelta);
	void Render();

public:
	const list<ParticleCube*>* GetParticleList() const { return &mParticleSpace; }
	HRESULT Add_PartiCleCube(const PARTICLE_INFO& particleInfo);

private:
	HRESULT AddParticle(const PARTICLE_INFO& particleInfo);

public:
	virtual DWORD Free() override;

private: // Component Related Variable
	Engine::Layer* m_pLayer = nullptr;

private:
	LPDIRECT3DDEVICE9 m_pDevice = nullptr;

private:
	ParticleCube* m_pParticleCube = nullptr;
	list<ParticleCube*> mParticleSpace;

private:
	vector<PARTICLE_INFO> mParticleInfo_Space;
	int mParticleInfo_Size = 0;

};
_END
#endif