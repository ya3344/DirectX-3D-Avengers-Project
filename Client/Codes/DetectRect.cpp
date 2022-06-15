#include "stdafx.h"
#include "DetectRect.h"
#include "Player.h"
#include "ComponentMgr.h"

_USING(Client)

DetectRect::DetectRect(LPDIRECT3DDEVICE9 pDevice)
	:Engine::GameObject(pDevice)
{
}

DetectRect::DetectRect(const DetectRect & rhs)
	: Engine::GameObject(rhs)
{
}

HRESULT DetectRect::Init()
{
	if (FAILED(ReadyComponent()))
		return E_FAIL;

	// Initialize DetectRect Buffer
	mVtxSize = sizeof(Engine::VTXCOL);
	mVtxCnt = 4;
	mVtxFVF = Engine::VTXCOL_FVF;

	mIdxSize = sizeof(Engine::POLYGON16);
	mIdxFmt = D3DFMT_INDEX16;
	mTriCnt = 2;

	HRESULT		hr = E_FAIL;
	hr = m_pDevice->CreateVertexBuffer(mVtxSize * mVtxCnt, 0, mVtxFVF
		, D3DPOOL_MANAGED, &m_pVB, nullptr);
	if (hr)
	{
		_MSG_BOX("Create Vertex Buffer Failed");
	}

	hr = m_pDevice->CreateIndexBuffer(mIdxSize * mTriCnt, 0, mIdxFmt
		, D3DPOOL_MANAGED, &m_pIB, nullptr);
	if (hr)
	{
		_MSG_BOX("Create Index Buffer Failed");
	}

	Engine::VTXCOL* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].color = D3DCOLOR_ARGB(200, 255, 0, 0);
	pVertex[1].color = D3DCOLOR_ARGB(200, 255, 0, 0);
	pVertex[2].color = D3DCOLOR_ARGB(200, 255, 0, 0);
	pVertex[3].color = D3DCOLOR_ARGB(200, 255, 0, 0);

	m_pVB->Unlock();

	Engine::POLYGON16*	pIndex = nullptr;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;	pIndex[1]._1 = 0;
	pIndex[0]._2 = 1;	pIndex[1]._2 = 2;
	pIndex[0]._3 = 2;	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	mDetectRadius = 5.f;

	// Initialize Fences Points
	vFencePos[0] = { -1.f, 0.f, 30.f };
	vFencePos[1] = { 30.f, 0.f, 30.f };
	vFencePos[2] = { 30.f, 0.f, -1.f };
	vFencePos[3] = { -1.f, 0.f, -1.f };

	m_pCenterPos = nullptr;
	m_pCenterDir = nullptr;

	return NOERROR;
}

UPDATE DetectRect::Update(const float timeDelta)
{
	if (m_pBossInfo == nullptr)
	{
		_MSG_BOX("UPdate_DetectRect Failed");
		return UPDATE::FAIL;
	}

	m_pCenterPos = m_pBossInfo->pMyPos;
	m_pCenterDir = m_pBossInfo->pMyLook;

	mRangeRadian = D3DXToRadian(45.f);
	float bufferRadian = mRangeRadian*0.5f;

	Engine::VTXCOL*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 0 Vertex
	pVertex[0].vPos = D3DXVECTOR3(0.f, 4.f, 0.f);
	// 1 Vertex
	pVertex[1].vPos = D3DXVECTOR3(cosf(bufferRadian)*mDetectRadius, 0.f, sinf(bufferRadian)*mDetectRadius);
	// 2 Vertex
	pVertex[2].vPos = D3DXVECTOR3(mDetectRadius, 0.f, 0.f);
	// 3 Vertex
	pVertex[3].vPos = D3DXVECTOR3(cosf(bufferRadian)*mDetectRadius, 0.f, -sinf(bufferRadian)*mDetectRadius);


	// Adjust Direction
	D3DXVECTOR3 vTmpDir = *m_pCenterDir;
	D3DXVec3Normalize(&vTmpDir, &vTmpDir);
	mDirRadian = acosf(D3DXVec3Dot(&vTmpDir, &D3DXVECTOR3{ 0.f, 0.f, 1.f })) + D3DXToRadian(-90.f);

	if (m_pCenterDir->x < 0)
	{
		mDirRadian = D3DXToRadian(180.f) - mDirRadian;
	}

	D3DXMATRIX matWorld, matRotY, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixRotationY(&matRotY, mDirRadian);
	D3DXMatrixTranslation(&matTrans, m_pCenterPos->x, m_pCenterPos->y - 3.f, m_pCenterPos->z);

	matWorld = matRotY*matTrans;

	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&pVertex[i].vPos, &pVertex[i].vPos, &matWorld);
	}


	m_pVB->Unlock();

	m_pVertex = pVertex;
	AdjustDetectRect();

	return UPDATE::NORMAL;
}

UPDATE DetectRect::LastUpdate(const float timeDelta)
{
	if (m_pRenderer == nullptr)
		return UPDATE::FAIL;

	if (FAILED(m_pRenderer->Add_RenderGroup(Engine::Renderer::RENDER_ALPHA, this)))
		return UPDATE::FAIL;

	// JH_AnimFrame_180624 DetectRect Return 
	if (m_bOperation == false)
		return UPDATE::NORMAL;

	// JH_AnimFrame_180624 PlayerDetect Check
	DetectObject();

	if (mRectColor != mOldColor)
	{
		mOldColor = mRectColor;
		Engine::VTXCOL* pVertex = nullptr;
		m_pVB->Lock(0, 0, (void**)&pVertex, 0);

		for (int i = 0; i < 4; ++i)
		{
			pVertex[i].color = mRectColor;
		}
		m_pVB->Unlock();
	}

	return UPDATE::NORMAL;
}

void DetectRect::Render()
{
	if (m_pCenterPos == nullptr)
		return;

	// JH_AnimFrame_180624 DetectRect Return 
	if (m_bOperation == false)
		return;

	D3DXMatrixIdentity(&m_matWorld);
	m_pDevice->SetTexture(0, nullptr);
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);

	m_pDevice->SetStreamSource(0, m_pVB, 0, mVtxSize);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->SetFVF(mVtxFVF);

	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, mVtxCnt, 0, mTriCnt);
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, true);
}

HRESULT DetectRect::ReadyPrototype()
{
	Init();

	return NOERROR;
}

HRESULT DetectRect::ReadyComponent(void)
{
	Engine::Component* pComponent = nullptr;

	// For.Com_Renderer
	pComponent = m_pRenderer = (Engine::Renderer*)Engine::ComponentMgr::GetInstance()->Clone(SCENE_STATIC, L"Component_Renderer");
	if (pComponent == nullptr)
		return E_FAIL;

	return NOERROR;
}

void DetectRect::SetTarget(Player * pPlayer)
{
	if (pPlayer == nullptr)
	{
		_MSG_BOX("SetGameObject Failed");
	}
	m_pTargetPos = pPlayer->Get_SuperCubeInfo()->pMyPos;
}

void DetectRect::SetFencePoint(D3DXVECTOR3 * vPoint)
{
	if (vPoint == nullptr)
	{
		_MSG_BOX("SetGameObject Failed");
	}

	for (int i = 0; i < 4; ++i)
	{
		vFencePos[i] = vPoint[i];
	}
}

void DetectRect::AdjustDetectRect()
{
	if (m_pVertex == nullptr)
	{
		_MSG_BOX("AdjustDetecRect Failed");
		return;
	}

	if (mDirRadian > D3DXToRadian(360.f))
	{
		mDirRadian -= D3DXToRadian(360.f);
	}
	else if (mDirRadian < D3DXToRadian(0.f))
	{
		mDirRadian += D3DXToRadian(360.f);
	}

	//1사분면
	if (mDirRadian >= D3DXToRadian(270.f) && mDirRadian < D3DXToRadian(360.f))
	{
		// 기울기 벡터 만들기
		D3DXVECTOR3 vIncline = m_pVertex[3].vPos - *m_pCenterPos;
		D3DXVec3Normalize(&vIncline, &vIncline);

		// 3번 방정식을 두개의 변 중 어느 변과 비교할지 결정
		D3DXVECTOR3 vCenterDir = vFencePos[1] - *m_pCenterPos;
		D3DXVec3Normalize(&vCenterDir, &vCenterDir);
		D3DXVECTOR3 vCrossResult;
		D3DXVec3Cross(&vCrossResult, &vCenterDir, &vIncline);

		// 우측 변과 비교
		if (vCrossResult.y > vFencePos[1].y)
		{
			m_pVertex[3].vPos.x = vFencePos[1].x;
			m_pVertex[3].vPos.z = (vIncline.z / vIncline.x) * vFencePos[1].x + (m_pCenterPos->z - (vIncline.z / vIncline.x) * m_pCenterPos->x);
		}// 상단 변과 비교
		else
		{
			m_pVertex[3].vPos.x = (vIncline.x / vIncline.z) * vFencePos[1].z + (m_pCenterPos->x - (vIncline.x / vIncline.z) * m_pCenterPos->z);
			m_pVertex[3].vPos.z = vFencePos[1].z;
		}

		// 기울기 벡터 만들기
		vIncline = m_pVertex[1].vPos - *m_pCenterPos;
		D3DXVec3Normalize(&vIncline, &vIncline);

		// 1번 방정식을 두개의 변 중 어느 변과 비교할지 결정
		vCrossResult = D3DXVECTOR3{ 0.f, 0.f, 0.f };
		D3DXVec3Cross(&vCrossResult, &vCenterDir, &vIncline);

		// 우측 변과 비교
		if (vCrossResult.y > vFencePos[1].y)
		{
			m_pVertex[1].vPos.x = vFencePos[1].x;
			m_pVertex[1].vPos.z = (vIncline.z / vIncline.x) * vFencePos[1].x + (m_pCenterPos->z - (vIncline.z / vIncline.x) * m_pCenterPos->x);
		}// 상단 변과 비교
		else
		{
			m_pVertex[1].vPos.x = m_pCenterPos->x + (m_pVertex[1].vPos.x - m_pCenterPos->x) * ((vFencePos[1].z - m_pCenterPos->z) / (m_pVertex[1].vPos.z - m_pCenterPos->z));
			m_pVertex[1].vPos.z = vFencePos[1].z;
		}

		m_pVertex[2].vPos.z = m_pVertex[1].vPos.z;
		m_pVertex[2].vPos.x = m_pVertex[3].vPos.x;
	}//2사분면
	else if (mDirRadian >= 0.f && mDirRadian < D3DXToRadian(90.f))
	{
		// 기울기 벡터 만들기
		D3DXVECTOR3 vIncline;
		D3DXVec3Normalize(&vIncline, &(m_pVertex[3].vPos - *m_pCenterPos));

		// 3번 방정식을 두개의 변 중 어느 변과 비교할지 결정
		D3DXVECTOR3 vCenterDir = vFencePos[2] - *m_pCenterPos;
		D3DXVec3Normalize(&vCenterDir, &vCenterDir);
		D3DXVECTOR3 vCrossResult;
		D3DXVec3Cross(&vCrossResult, &vCenterDir, &vIncline);

		// 하단 변과 비교
		if (vCrossResult.y > vFencePos[2].y)
		{
			m_pVertex[3].vPos.x = (vIncline.x / vIncline.z) * vFencePos[2].z + (m_pCenterPos->x - (vIncline.x / vIncline.z) * m_pCenterPos->z);
			m_pVertex[3].vPos.z = vFencePos[2].z;

		}// 우측 변과 비교
		else
		{
			m_pVertex[3].vPos.x = vFencePos[2].x;
			m_pVertex[3].vPos.z = (vIncline.z / vIncline.x) * vFencePos[2].x + (m_pCenterPos->z - (vIncline.z / vIncline.x) * m_pCenterPos->x);
		}


		// 기울기 벡터 만들기
		vIncline = m_pVertex[1].vPos - *m_pCenterPos;
		D3DXVec3Normalize(&vIncline, &vIncline);

		// 1번 방정식을 두개의 변 중 어느 변과 비교할지 결정
		vCenterDir = vFencePos[2] - *m_pCenterPos;
		D3DXVec3Normalize(&vCenterDir, &vCenterDir);
		vCrossResult = D3DXVECTOR3{ 0.f, 0.f, 0.f };
		D3DXVec3Cross(&vCrossResult, &vCenterDir, &vIncline);

		// 하단 변과 비교
		if (vCrossResult.y > vFencePos[2].y)
		{
			m_pVertex[1].vPos.x = (vIncline.x / vIncline.z) * vFencePos[2].z + (m_pCenterPos->x - (vIncline.x / vIncline.z) * m_pCenterPos->z);
			m_pVertex[1].vPos.z = vFencePos[2].z;
		}// 우측 변과 비교
		else
		{
			m_pVertex[1].vPos.x = vFencePos[2].x;
			m_pVertex[1].vPos.z = (vIncline.z / vIncline.x) * vFencePos[2].x + (m_pCenterPos->z - (vIncline.z / vIncline.x) * m_pCenterPos->x);
		}

		m_pVertex[2].vPos.x = m_pVertex[1].vPos.x;
		m_pVertex[2].vPos.z = m_pVertex[3].vPos.z;

	}//3사분면
	else if (mDirRadian >= D3DXToRadian(90.f) && mDirRadian < D3DXToRadian(180.f))
	{
		// 기울기 벡터 만들기
		D3DXVECTOR3 vIncline = m_pVertex[3].vPos - *m_pCenterPos;
		D3DXVec3Normalize(&vIncline, &vIncline);

		// 3번 방정식을 두개의 변 중 어느 변과 비교할지 결정
		D3DXVECTOR3 vCenterDir = vFencePos[3] - *m_pCenterPos;
		D3DXVec3Normalize(&vCenterDir, &vCenterDir);
		D3DXVECTOR3 vCrossResult;
		D3DXVec3Cross(&vCrossResult, &vCenterDir, &vIncline);

		// 좌측 변과 비교
		if (vCrossResult.y > vFencePos[3].y)
		{
			m_pVertex[3].vPos.x = vFencePos[3].x;
			m_pVertex[3].vPos.z = (vIncline.z / vIncline.x) * vFencePos[3].x + (m_pCenterPos->z - (vIncline.z / vIncline.x) * m_pCenterPos->x);
		}// 하단 변과 비교
		else
		{
			m_pVertex[3].vPos.x = (vIncline.x / vIncline.z) * vFencePos[3].z + (m_pCenterPos->x - (vIncline.x / vIncline.z) * m_pCenterPos->z);
			m_pVertex[3].vPos.z = vFencePos[3].z;
		}


		// 기울기 벡터 만들기
		vIncline = m_pVertex[1].vPos - *m_pCenterPos;
		D3DXVec3Normalize(&vIncline, &vIncline);

		// 1번 방정식을 두개의 변 중 어느 변과 비교할지 결정
		vCrossResult = D3DXVECTOR3{ 0.f, 0.f, 0.f };
		D3DXVec3Cross(&vCrossResult, &vCenterDir, &vIncline);

		// 좌측 변과 비교
		if (vCrossResult.y > vFencePos[3].y)
		{
			m_pVertex[1].vPos.x = vFencePos[3].x;
			m_pVertex[1].vPos.z = (vIncline.z / vIncline.x) * vFencePos[3].x + (m_pCenterPos->z - (vIncline.z / vIncline.x) * m_pCenterPos->x);
		}// 하단 변과 비교
		else
		{
			m_pVertex[1].vPos.x = m_pCenterPos->x + (m_pVertex[1].vPos.x - m_pCenterPos->x) * ((vFencePos[3].z - m_pCenterPos->z) / (m_pVertex[1].vPos.z - m_pCenterPos->z));
			m_pVertex[1].vPos.z = vFencePos[3].z;
		}

		m_pVertex[2].vPos.z = m_pVertex[1].vPos.z;
		m_pVertex[2].vPos.x = m_pVertex[3].vPos.x;
	}//4사분면
	else if (mDirRadian >= D3DXToRadian(180.f) && mDirRadian < D3DXToRadian(270.f))
	{
		// 기울기 벡터 만들기
		D3DXVECTOR3 vIncline = m_pVertex[3].vPos - *m_pCenterPos;
		D3DXVec3Normalize(&vIncline, &vIncline);

		// 3번 방정식을 두개의 변 중 어느 변과 비교할지 결정
		D3DXVECTOR3 vCenterDir = vFencePos[0] - *m_pCenterPos;
		D3DXVec3Normalize(&vCenterDir, &vCenterDir);
		D3DXVECTOR3 vCrossResult;
		D3DXVec3Cross(&vCrossResult, &vCenterDir, &vIncline);

		// 우측 변과 비교
		if (vCrossResult.y > vFencePos[0].y)
		{
			m_pVertex[3].vPos.x = (vIncline.x / vIncline.z) * vFencePos[0].z + (m_pCenterPos->x - (vIncline.x / vIncline.z) * m_pCenterPos->z);
			m_pVertex[3].vPos.z = vFencePos[0].z;
		}// 상단 변과 비교
		else
		{
			m_pVertex[3].vPos.x = vFencePos[0].x;
			m_pVertex[3].vPos.z = (vIncline.z / vIncline.x) * vFencePos[0].x + (m_pCenterPos->z - (vIncline.z / vIncline.x) * m_pCenterPos->x);
		}


		// 기울기 벡터 만들기
		vIncline = m_pVertex[1].vPos - *m_pCenterPos;
		D3DXVec3Normalize(&vIncline, &vIncline);

		// 1번 방정식을 두개의 변 중 어느 변과 비교할지 결정
		vCrossResult = D3DXVECTOR3{ 0.f, 0.f, 0.f };
		D3DXVec3Cross(&vCrossResult, &vCenterDir, &vIncline);

		// 우측 변과 비교
		if (vCrossResult.y > vFencePos[0].y)
		{
			m_pVertex[1].vPos.x = m_pCenterPos->x + (m_pVertex[1].vPos.x - m_pCenterPos->x) * ((vFencePos[0].z - m_pCenterPos->z) / (m_pVertex[1].vPos.z - m_pCenterPos->z));
			m_pVertex[1].vPos.z = vFencePos[0].z;
		}// 상단 변과 비교
		else
		{
			m_pVertex[1].vPos.x = vFencePos[0].x;
			m_pVertex[1].vPos.z = (vIncline.z / vIncline.x) * vFencePos[0].x + (m_pCenterPos->z - (vIncline.z / vIncline.x) * m_pCenterPos->x);
		}

		m_pVertex[2].vPos.z = m_pVertex[3].vPos.z;
		m_pVertex[2].vPos.x = m_pVertex[1].vPos.x;
	}
}

void DetectRect::DetectObject()
{
	if (m_pVertex == nullptr || m_pTargetPos == nullptr)
	{
		//_MSG_BOX("DetectObject Failed");
		return;
	}

	if (m_bOperation == false)
		return;


	// 1번 기울기
	D3DXVECTOR3 vIncline_num1 = m_pVertex[1].vPos - *m_pCenterPos;
	vIncline_num1.y = 0.f;
	D3DXVec3Normalize(&vIncline_num1, &vIncline_num1);

	// 3번 기울기
	D3DXVECTOR3 vIncline_num3 = m_pVertex[3].vPos - *m_pCenterPos;
	vIncline_num3.y = 0.f;
	D3DXVec3Normalize(&vIncline_num3, &vIncline_num3);

	// 타겟 기울기
	D3DXVECTOR3 vIncline_Target = *m_pTargetPos - *m_pCenterPos;
	vIncline_Target.y = 0.f;
	D3DXVec3Normalize(&vIncline_Target, &vIncline_Target);

	// 1번 - 타겟 내각
	float radian1 = acosf(D3DXVec3Dot(&vIncline_num1, &vIncline_Target));

	// 3번 - 타겟 내각
	float radian2 = acosf(D3DXVec3Dot(&vIncline_num3, &vIncline_Target));

	// 1번 - 3번  내각
	float radian3 = acosf(D3DXVec3Dot(&vIncline_num1, &vIncline_num3));

	// JH_AnimFrame_180624  DetectCheck Variable Apply
	if (abs(radian3 - (radian1 + radian2)) <= 0.01f)
	{
		if (radian1 < D3DXToRadian(10.f) || radian2 < D3DXToRadian(10.f))
		{
			mRectColor = D3DCOLOR_ARGB(200, 255, 211, 0); // 주황색
			m_bPlayerDetect[ORANGE] = true;
			m_bPlayerDetect[RED] = false;

		}
		else
		{
			mRectColor = D3DCOLOR_ARGB(200, 255, 0, 0); // 빨간색
			m_bPlayerDetect[ORANGE] = false;
			m_bPlayerDetect[RED] = true;
		}

	}
	else
	{
		mRectColor = D3DCOLOR_ARGB(200, 0, 255, 0); // 녹색
		m_bPlayerDetect[ORANGE] = false;
		m_bPlayerDetect[RED] = false;
	}



}

DetectRect * DetectRect::Create(LPDIRECT3DDEVICE9 pDevice)
{
	DetectRect* pInstance = new DetectRect(pDevice);

	if (FAILED(pInstance->ReadyPrototype()))
	{
		_MSG_BOX("DetectRect Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::GameObject * DetectRect::Clone()
{
	AddRef();
	return this;
}

DWORD DetectRect::Free()
{
	Engine::Safe_Release(m_pVB);
	Engine::Safe_Release(m_pIB);
	Engine::Safe_Release(m_pRenderer);

	GameObject::Free();

	return DWORD();
}
