#ifndef TERRAIN_H__
#define TERRAIN_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Renderer;
	class Transform;
}

_BEGIN(Client)

class TerrainAnim;
class TerrainCube;
class Boss;
class CameraMgr;

class Terrain final : public Engine::GameObject
{
private:
	explicit Terrain(LPDIRECT3DDEVICE9 pDevice);
	explicit Terrain(const Terrain& rhs);
	virtual ~Terrain() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

private:
	HRESULT Ready_LayerTerrainCube();
	HRESULT ReadyComponent(void);

private:
	void FileLoad();
	bool TransGameObject(Engine::Transform* pTrans, D3DXVECTOR3* vPos, float fallPace);

public:
	static Terrain* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Renderer* m_pRenderer = nullptr;

private:
	D3DMATERIAL9 mMtrlInfo;

private:
	vector<TerrainCube*> mTerrainCube_Space;
	size_t mTexCube_Size = 0;

	// Original Position of Cube Per Stage
private:
	D3DXVECTOR3* m_pOriginPos = nullptr;
	D3DXVECTOR3 m_vChangedPos = D3DXVECTOR3{ 0.f, 0.f, 0.f };

private: // Variables Related To Terrain Anim
	TerrainAnim* m_pTerrainAnim = nullptr;
	bool m_bTornado = false;
	bool m_bVibrate = false;

private: // FileRead Path Related Variable
	vector<wstring> mNameSpace;

private:
	Engine::Transform* m_pPlayerTrans = nullptr;
	Engine::Transform* m_pBossTrans = nullptr;
	bool m_bPlayerFall = true;
	bool m_bBossFall = true;
	float mTimeDelta = 0.f;

private: // JH_AnimFrame_180624 BossObj Variable Add
	Boss* m_pBossObj = nullptr;
private: //DH_SoundAdd_180706_SoundAdd
	Engine::SoundMgr* m_pSoundMgr = nullptr;

private:
	bool m_bTerrainTransform_DEBUG = false;
	bool m_bSetTerrainPos = true; 	// TY_FixBullet_180706_ Add Bool For Terrain Pos

private:
	CameraMgr* m_pCameraMgr = nullptr;
};

_END

#endif // TERRAIN_H__
