#include "Camera.h"

_USING(Engine)

Camera::Camera(LPDIRECT3DDEVICE9 pDevice)
	: GameObject(pDevice)
	

{

}


Camera::Camera(const Camera & rhs)
	: GameObject(rhs)
	, mViewInfo(rhs.mViewInfo)
	, mProjInfo(rhs.mProjInfo)
	, m_matWorld(rhs.m_matWorld)
{
	
}

HRESULT Camera::Init()
{
	return NOERROR;
}

HRESULT Camera::Init(const D3DXVECTOR3& vEye, const D3DXVECTOR3& vAt, const D3DXVECTOR3& vAxisY, const float fovY, const float aspect, const float projNear, const float projFar)
{
	mViewInfo.vEye = vEye;
	mViewInfo.vAt = vAt;
	mViewInfo.vAxisY = vAxisY;

	if (FAILED(Cal_ViewMatrix()))
		return E_FAIL;

	m_pDevice->SetTransform(D3DTS_VIEW, &mViewInfo.matView);

	mProjInfo.fovY = fovY;
	mProjInfo.aspect = aspect;
	mProjInfo.projNear = projNear;
	mProjInfo.projFar = projFar;

	if (FAILED(Cal_ProjMatrix()))
		return E_FAIL;
	
	m_pDevice->SetTransform(D3DTS_PROJECTION, &mProjInfo.matProj);

	return NOERROR;
}

UPDATE Camera::Update(const float timeDelta)
{
	return UPDATE::NORMAL;
}

UPDATE Camera::LastUpdate(const float timeDelta)
{
	//TY_CameraShake_180624_Call Function To Shake Camera
	if (m_bShake)
	{
		CameraShaking(timeDelta);
	}

	Cal_ViewMatrix();
	m_pDevice->SetTransform(D3DTS_VIEW, &mViewInfo.matView);

	Cal_ProjMatrix();
	m_pDevice->SetTransform(D3DTS_PROJECTION, &mProjInfo.matProj);

	return UPDATE::NORMAL;
}

void Camera::SetCameraShake(const float shakeTime)
{
	if (m_bShake == true)
		return;

	mShakeTime = shakeTime;
	m_bShake = true;
	mShakeInterval = 0.01f;
}

void Camera::CameraShaking(const float timeDelta)
{
	mShakeTime -= timeDelta;
	++mShakeAcc_Count;

	if (mShakeTime <= 0.f)
	{
		m_bShake = false;
		mShakeAcc_Count = 0;
	}

	if (mShakeAcc_Count < 100*timeDelta)
	{
		mViewInfo.vAt += D3DXVECTOR3{ mShakeInterval , mShakeInterval , mShakeInterval };
	}
	else if (mShakeAcc_Count < 200 * timeDelta)
	{
		mViewInfo.vAt -= D3DXVECTOR3{ mShakeInterval , mShakeInterval , mShakeInterval };
	}
	else
	{
		mShakeAcc_Count = 0;
		mShakeInterval -= 0.0005f;
	}

}

HRESULT Camera::Cal_ViewMatrix()
{
	D3DXVECTOR3	vRight, vUp, vLook, vPosition;

	D3DXVec3Normalize(&vLook, &(mViewInfo.vAt - mViewInfo.vEye));
	D3DXVec3Normalize(&vRight, D3DXVec3Cross(&vRight, &mViewInfo.vAxisY, &vLook));
	D3DXVec3Normalize(&vUp, D3DXVec3Cross(&vUp, &vLook, &vRight));
	vPosition = mViewInfo.vEye;

	D3DXMatrixIdentity(&m_matWorld);

	// 카메라의 월드행렬을 구한다.
	memcpy(&m_matWorld.m[0][0], &vRight, sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld.m[1][0], &vUp, sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld.m[2][0], &vLook, sizeof(D3DXVECTOR3));
	memcpy(&m_matWorld.m[3][0], &vPosition, sizeof(D3DXVECTOR3));

	// 뷰스페이스 변환행렬을 구한다.
	D3DXMatrixInverse(&mViewInfo.matView, nullptr, &m_matWorld);

	return NOERROR;
}

HRESULT Camera::Cal_ProjMatrix()
{
	mProjInfo.matProj._11 = (1.f / tanf(mProjInfo.fovY * 0.5f)) / mProjInfo.aspect;
	
	mProjInfo.matProj._22 = (1.f / tanf(mProjInfo.fovY * 0.5f));
	mProjInfo.matProj._33 = mProjInfo.projFar / (mProjInfo.projFar - mProjInfo.projNear);
	mProjInfo.matProj._43 = -(mProjInfo.projFar * mProjInfo.projNear) / (mProjInfo.projFar - mProjInfo.projNear);
	mProjInfo.matProj._34 = 1.f;
	mProjInfo.matProj._44 = 0.f;

	return NOERROR;
}

DWORD Camera::Free()
{
	
	GameObject::Free();

	return DWORD();
}


