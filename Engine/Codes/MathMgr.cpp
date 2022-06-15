#include "MathMgr.h"

_USING(Engine)

_IMPLEMENT_SINGLETON(MathMgr)

MathMgr::MathMgr()
{
}

const float MathMgr::CalDistance(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vTarget)
{
	D3DXVECTOR3 targetDir = vTarget - vPos;
	float distance = D3DXVec3Length(&targetDir);

	return distance;
}

const float MathMgr::CalDirDegree(const D3DXVECTOR3 & vSrcPos, const D3DXVECTOR3 & vDestPos)
{
	D3DXVECTOR3 vSrcDir = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vDestDir = D3DXVECTOR3(0.f, 0.f, 0.f);

	D3DXVec3Normalize(&vSrcDir, &vSrcPos);
	D3DXVec3Normalize(&vDestDir, &vDestPos);

	float radian = acosf(D3DXVec3Dot(&vSrcDir, &vDestDir));

	return D3DXToDegree(radian);
}


const D3DXVECTOR3 & MathMgr::CalTargetDir(const D3DXVECTOR3 & vPos, const D3DXVECTOR3 & vTarget)
{
	m_vTargetDir = vTarget - vPos;
	D3DXVec3Normalize(&m_vTargetDir, &m_vTargetDir);

	return m_vTargetDir;
}

const D3DXVECTOR3 & MathMgr::RandomPos(const D3DXVECTOR3 & vPos, float distance)
{
	mCurMotion = (OBJ_MOTION)((rand() % OBJ_MOTION::MOTION_END));

	switch (mCurMotion)
	{
	case OBJ_MOTION::DOWN:
		m_vRandomPos.x = vPos.x;
		m_vRandomPos.z = vPos.z - (1 * distance);
		break;
	case OBJ_MOTION::LD:
		m_vRandomPos.x = vPos.x - (1 * distance);
		m_vRandomPos.z = vPos.z - (1 * distance);
		break;
	case OBJ_MOTION::LEFT:
		m_vRandomPos.x = vPos.x - (1 * distance);
		m_vRandomPos.z = vPos.z;
		break;
	case OBJ_MOTION::LU:
		m_vRandomPos.x = vPos.x - (1 * distance);
		m_vRandomPos.z = vPos.z + (1 * distance);
		break;
	case OBJ_MOTION::RD:
		m_vRandomPos.x = vPos.x + (1 * distance);
		m_vRandomPos.z = vPos.z - (1 * distance);
		break;
	case OBJ_MOTION::RIGHT:
		m_vRandomPos.x = vPos.x + (1 * distance);
		m_vRandomPos.z = vPos.z;
		break;
	case OBJ_MOTION::RU:
		m_vRandomPos.x = vPos.x + (1 * distance);
		m_vRandomPos.z = vPos.z - (1 * distance);
		break;
	case OBJ_MOTION::UP:
		m_vRandomPos.x = vPos.x;
		m_vRandomPos.z = vPos.z - (1 * distance);
		break;
	default:
		assert(false && "MathMgr::RandomPos");
		break;
	}

	m_vRandomPos.y = vPos.y;

	return m_vRandomPos;
}

const D3DXVECTOR3 & MathMgr::RandomDir()
{
	mCurMotion = (OBJ_MOTION)((rand() % OBJ_MOTION::MOTION_END));

	while (mCurMotion == mPreMotion)
	{
		mCurMotion = (OBJ_MOTION)((rand() % OBJ_MOTION::MOTION_END));
	}

	switch (mCurMotion)
	{
	case OBJ_MOTION::DOWN:
		m_vRandomDir.x = 0.f;
		m_vRandomDir.z = -1.f;
		break;
	case OBJ_MOTION::LD:
		m_vRandomDir.x = -1.f;
		m_vRandomDir.z = -1.f;
		break;
	case OBJ_MOTION::LEFT:
		m_vRandomDir.x = -1.f;
		m_vRandomDir.z = 0.f;
		break;
	case OBJ_MOTION::LU:
		m_vRandomDir.x = -1.f;
		m_vRandomDir.z = 1.f;
		break;
	case OBJ_MOTION::RD:
		m_vRandomDir.x = 1.f;
		m_vRandomDir.z = -1.f;
		break;
	case OBJ_MOTION::RIGHT:
		m_vRandomDir.x = 1.f;
		m_vRandomDir.z = 0.f;
		break;
	case OBJ_MOTION::RU:
		m_vRandomDir.x = 1.f;
		m_vRandomDir.z = -1.f;
		break;
	case OBJ_MOTION::UP:
		m_vRandomDir.x = 0.f;
		m_vRandomDir.z = 1.f;
		break;
	default:
		assert(false && "MathMgr::RandomDir");
		break;
	}

	m_vRandomDir.y = 0.f;
	mPreMotion = mCurMotion;

	return m_vRandomDir;
}

DWORD MathMgr::Free()
{
	return 0;
}
