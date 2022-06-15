#include "stdafx.h"
#include "TerrainAnim.h"
#include "TerrainCube.h"

_USING(Client)

TerrainAnim::TerrainAnim()
{
}

HRESULT TerrainAnim::Init()
{
	return NOERROR;
}

UPDATE TerrainAnim::Update(const float timeDelta)
{
	++mVibrateCount;

	mTimeDelta = timeDelta;

	return UPDATE::NORMAL;
}

void TerrainAnim::SetTerrainCube(const vector<TerrainCube*>& pCubeSpace)
{
	if (pCubeSpace.size() == 0)
	{
		_MSG_BOX("SetTerrainCube Failed");
	}

	mCubeNum = pCubeSpace.size();
	mRandomNum = mCubeNum;

	mStartCube_Space.reserve(mCubeNum);
	mEndCube_Space.reserve(mCubeNum);

	for (size_t i = 0; i < mCubeNum; ++i)
	{
		// TY_FixBullet_180706_ Change To Emplace
		mStartCube_Space.emplace_back(pCubeSpace[i]);
		mEndCube_Space.emplace_back(nullptr);
	}

	m_pVibrateTime = new float[mCubeNum];

	for (size_t i = 0; i < mCubeNum; ++i)
	{
		m_pVibrateTime[i] = 0.f;
	}
}

void TerrainAnim::SetOriginalCubePos(D3DXVECTOR3 * pOriginPos)
{
	m_pOriginPos = pOriginPos;
}

//TY_FixChangeTerrain_180704_ Fix Terrain
TERRAIN_STATE TerrainAnim::TornadoCube()
{
	if (m_pOriginPos == nullptr)
	{
		_MSG_BOX("TornadoCube Failed");
	}

	if (m_bFormComplete)
	{
		return TERRAIN_STATE::PLAYER_FALL_FIRST;
	}

	m_bFormComplete = true;

	if (mRandomNum < 10)
	{
		for (size_t i = 0; i < mCubeNum; ++i)
		{
			if (mStartCube_Space[i] == nullptr)
				continue;

			m_bFormComplete = false;

			mEndCube_Space[i] = mStartCube_Space[i];
			mStartCube_Space[i] = nullptr;
		}
	}
	else
	{
		for (size_t i = 0; i < mCubeNum; ++i)
		{
			if (mStartCube_Space[i] == nullptr)
				continue;

			m_bFormComplete = false;

			mStartCube_Space[i]->SetRevolve({ 0.f, D3DXToRadian(90.f)*mTimeDelta*0.5f , 0.f });

			// Pick Cubes To Be Sent To Original Position
			// TY_FixBullet_180706_ Add Condition
			if (mRandomNum != 0 && rand() % mRandomNum == 0)
			{
				mEndCube_Space[i] = mStartCube_Space[i];
				mStartCube_Space[i] = nullptr;
				--mRandomNum;
			}
		}
	}


	D3DXVECTOR3 distance = { 0.f, 0.f, 0.f };
	D3DXMATRIX* matCurrentPos = nullptr;

	int endCube_Num = 0;
	for (size_t i = 0; i < mCubeNum; ++i)
	{
		if (mEndCube_Space[i] == nullptr)
		{
			continue;
		}
		m_bFormComplete = false;

		distance = m_pOriginPos[i] - mEndCube_Space[i]->GetPos();
		float lengthDist = D3DXVec3Length(&distance);

		if (lengthDist >= 0.01f)
		{
			matCurrentPos = mEndCube_Space[i]->GetMatrixInfo().matWorld;

			mEndCube_Space[i]->SetPos(D3DXVECTOR3{ matCurrentPos->m[3][0], matCurrentPos->m[3][1], matCurrentPos->m[3][2] });
			mEndCube_Space[i]->SetTrans(distance*mTimeDelta);
			mEndCube_Space[i]->SetRevolRadian({ 0.f, 0.f, 0.f });
			D3DXVECTOR3 vRot = mEndCube_Space[i]->GetRotRadian();
			mEndCube_Space[i]->SetRotation({ -vRot.x*mTimeDelta, -vRot.y*mTimeDelta, -vRot.z*mTimeDelta });
		}
		else
		{
			mEndCube_Space[i]->SetRotRadian({ 0.f, 0.f, 0.f });
			mEndCube_Space[i]->SetPos(m_pOriginPos[i]);
			mEndCube_Space[i]->SetCubeState_Transform(false);
			++endCube_Num;
		}
	}

	if (endCube_Num == mCubeNum)
	{
		m_bFormComplete = true;
		EndCubeToStartCube();
		mRandomNum = mCubeNum;
		return TERRAIN_STATE::TORNADO;
	}

	return TERRAIN_STATE::TORNADO;
}

//TY_FixChangeTerrain_180704_ Fix Terrain
TERRAIN_STATE TerrainAnim::DropCube(const D3DXVECTOR3 & vDest)
{
	if (m_bVibrateComplete)
	{
		return TERRAIN_STATE::PLAYER_FALL_SECOND;
	}

	if (mRandomNum < 10)
	{
		for (size_t i = 0; i < mCubeNum; ++i)
		{
			if (mStartCube_Space[i] == nullptr)
				continue;

			mEndCube_Space[i] = mStartCube_Space[i];
			mStartCube_Space[i] = nullptr;
			mEndCube_Space[i]->SetCubeState_Transform(true);
		}
	}
	else
	{
		for (size_t i = 0; i < mCubeNum; ++i)
		{
			if (mStartCube_Space[i] == nullptr)
				continue;

			// TY_FixBullet_180706_ Add Condition
			if (mRandomNum != 0 && rand() % mRandomNum == 0)
			{
				mEndCube_Space[i] = mStartCube_Space[i];
				mStartCube_Space[i] = nullptr;
				mEndCube_Space[i]->SetCubeState_Transform(true);

				--mRandomNum;
			}
		}
	}



	D3DXVECTOR3 distance = { 0.f, 0.f, 0.f };
	D3DXMATRIX* matCurrentPos = nullptr;

	int endCube_Num = 0;
	for (size_t i = 0; i < mCubeNum; ++i)
	{
		if (mEndCube_Space[i] == nullptr)
		{
			continue;
		}
		distance = (m_pOriginPos[i] + vDest) - mEndCube_Space[i]->GetPos();
		float lengthDist = D3DXVec3Length(&distance);

		if (m_pVibrateTime[i] < 1.f)
		{
			m_pVibrateTime[i] += mTimeDelta;

			VibrateCube(mEndCube_Space[i], 0.2f);
		}// Cubes Are Sent To Next Stage Position After 1 Sec
		else
		{
			if (lengthDist >= 0.05f)
			{
				matCurrentPos = mEndCube_Space[i]->GetMatrixInfo().matWorld;
				mEndCube_Space[i]->SetPos(D3DXVECTOR3{ matCurrentPos->m[3][0], matCurrentPos->m[3][1], matCurrentPos->m[3][2] });
				mEndCube_Space[i]->SetRotation({ D3DXToRadian(3.f), D3DXToRadian(3.f), D3DXToRadian(3.f) });
				mEndCube_Space[i]->SetTrans(distance*mTimeDelta);
			}
			else
			{
				mEndCube_Space[i]->SetRotRadian({ D3DXToRadian(90.f), 0.f, 0.f });
				mEndCube_Space[i]->SetPos((m_pOriginPos[i] + vDest));
				//mEndCube_Space[i]->SetCubeState_Transform(false
				++endCube_Num;
			}
		}
	}

	if (endCube_Num == mCubeNum)
	{
		EndCubeToStartCube();
		m_bVibrateComplete = true;
		return TERRAIN_STATE::PLAYER_FALL_SECOND;
	}


	return TERRAIN_STATE::VIBRATE;
}

void TerrainAnim::VibrateCube(TerrainCube * pCube, float range)
{
	if (pCube == nullptr)
	{
		_MSG_BOX("VibrateCube Failed");
	}

	float vibrateRange = range*0.5f;

	if (mVibrateCount % 4 == 1)
	{
		pCube->SetTrans(D3DXVECTOR3{ -vibrateRange, -vibrateRange, -vibrateRange });
	}
	else if (mVibrateCount % 4 == 3)
	{
		pCube->SetTrans(D3DXVECTOR3{ vibrateRange, vibrateRange, vibrateRange });
	}
	else if (mVibrateCount >= 4)
	{
		mVibrateCount = 0;
	}
}

void TerrainAnim::EndCubeToStartCube()
{
	if (mStartCube_Space.size() == 0
		|| mEndCube_Space.size() == 0)
	{
		_MSG_BOX("EndCube To StartCube Failed");
		return;
	}

	for (size_t i = 0; i < mCubeNum; ++i)
	{
		if (nullptr != mEndCube_Space[i])
		{
			mStartCube_Space[i] = mEndCube_Space[i];
		}
		mEndCube_Space[i] = nullptr;
	}
}

//TY_ChangeStage_180705_ Add Parameter
void TerrainAnim::SetTerrainCubeToZERO_DEBUG(D3DXVECTOR3* pDest)
{
	D3DXVECTOR3* pDestPos = pDest;

	EndCubeToStartCube();
	mRandomNum = mCubeNum;

	if (pDestPos == nullptr)
	{
		for (size_t i = 0; i < mCubeNum; ++i)
		{
			if (mStartCube_Space[i] == nullptr)
				continue;

			// TY_FixBullet_180706_ SetTransform 추가
			mStartCube_Space[i]->SetCubeState_Transform(true);
			mStartCube_Space[i]->SetPos(m_pOriginPos[i]);
			mStartCube_Space[i]->SetRevolRadian(D3DXVECTOR3{ 0.f, 0.f, 0.f });
			mStartCube_Space[i]->SetRotRadian(D3DXVECTOR3{ 0.f, 0.f, 0.f });
		}
	}
	else
	{
		for (size_t i = 0; i < mCubeNum; ++i)
		{
			if (m_pOriginPos[i] == nullptr || pDest == nullptr)
				continue;

			// TY_FixBullet_180706_ SetTransform 추가
			mStartCube_Space[i]->SetCubeState_Transform(true);
			mStartCube_Space[i]->SetPos(m_pOriginPos[i] + *pDest);
			mStartCube_Space[i]->SetRevolRadian(D3DXVECTOR3{ 0.f, 0.f, 0.f });
			mStartCube_Space[i]->SetRotRadian(D3DXVECTOR3{ 0.f, 0.f, 0.f });
		}
	}

}

void TerrainAnim::SetCubeState_Transform(bool bTransform)
{
	for (size_t i = 0; i < mCubeNum; ++i)
	{
		if (mStartCube_Space[i] != nullptr)
			mStartCube_Space[i]->SetCubeState_Transform(false);

		if (mEndCube_Space[i] != nullptr)
			mEndCube_Space[i]->SetCubeState_Transform(false);
	}
}

TerrainAnim * TerrainAnim::Create()
{
	TerrainAnim* pInstance = new TerrainAnim();

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("TerrainAnim Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

DWORD TerrainAnim::Free()
{
	Engine::Safe_Delete_Array(m_pVibrateTime);

	return 0;
}
