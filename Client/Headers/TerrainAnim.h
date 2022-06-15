#ifndef TERRAIN_ANIM_H__
#define TERRAIN_ANIM_H__

#include "Base.h"
#include "Include.h"

_BEGIN(Client)

class TerrainCube;
class TerrainAnim final : public Engine::Base
{
private:
	explicit TerrainAnim();
	virtual ~TerrainAnim() = default;

public:
	HRESULT Init();
	UPDATE Update(const float timeDelta);

public:
	void SetTerrainCube(const vector<TerrainCube*>& pCubeSpace);
	void SetOriginalCubePos(D3DXVECTOR3* pOriginPos); // Terrain Cube Position per Scene

public: //TY_ChangeStage_180703_ Change Return Type
	TERRAIN_STATE TornadoCube();
	TERRAIN_STATE DropCube(const D3DXVECTOR3& vDest);

public:
	void VibrateCube(TerrainCube* pCube, float range);
	void EndCubeToStartCube();

public:
	void SetTerrainCubeToZERO_DEBUG(D3DXVECTOR3* pDest = nullptr); //TY_ChangeStage_180705_ Add Parameter
	void SetCubeState_Transform(bool bTransform); // TY_FixBullet_180701 Add Function To Set Terrain

public:
	static TerrainAnim* Create();
	virtual DWORD Free() override;

	// Vectors Managing Change of Cube States
private:
	vector<TerrainCube*> mStartCube_Space;
	vector<TerrainCube*> mEndCube_Space;
	size_t mCubeNum = 0;

private:
	D3DXVECTOR3* m_pOriginPos = nullptr;
	bool m_bFormComplete = false;

private:
	size_t mVibrateCount = 0;
	bool m_bVibrateComplete = false;
	float* m_pVibrateTime = nullptr;

private:
	float mTimeDelta = 0.f;
	int mRandomNum = 0;
};
_END
#endif


