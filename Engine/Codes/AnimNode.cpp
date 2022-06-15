#include "AnimNode.h"

_USING(Engine)

AnimNode::AnimNode(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{
}

AnimNode::AnimNode(const AnimNode & rhs)
	: Component(rhs)
	, m_pLeftChild(rhs.m_pLeftChild)
	, m_pRightSibling(rhs.m_pRightSibling)
	, m_pParent(rhs.m_pParent)
	, mHierachyMatrix(rhs.mHierachyMatrix)
	, m_matInfo(rhs.m_matInfo)
	, m_pCubeInfo(rhs.m_pCubeInfo)
	, mFrameInfo(rhs.mFrameInfo)
	, m_pAnimInfo(rhs.m_pAnimInfo)
	, mName(rhs.mName)
	, mParentName(rhs.mParentName)
	, mAnimInfo_Space(rhs.mAnimInfo_Space)
	, mAnimRot(rhs.mAnimRot)
	, mAnimScale(rhs.mAnimScale)
	, mAnimTrans(rhs.mAnimTrans)
{

}

HRESULT AnimNode::Init()
{
	return NOERROR;
}

void AnimNode::AddChild(AnimNode* pAnimNode)
{
	if (m_pLeftChild == nullptr)
	{
		m_pLeftChild = pAnimNode;
		m_pLeftChild->m_pParent = this;
	}
	else
	{
		AnimNode* pTempAnim = m_pLeftChild;

		// search untill nullptr
		while (pTempAnim->m_pRightSibling != nullptr)
		{
			pTempAnim = pTempAnim->m_pRightSibling;
		}

		pTempAnim->m_pRightSibling = pAnimNode;
		pTempAnim->m_pRightSibling->m_pParent = this;
	}
}

void AnimNode::RecursiveMatrix(const D3DXMATRIX* pMatParent)
{
	D3DXMatrixIdentity(&mHierachyMatrix.m_matInvScale);
	D3DXMatrixInverse(&mHierachyMatrix.m_matInvScale, nullptr, &m_matInfo.matScale);

	if (m_pLeftChild != nullptr)
	{
		mHierachyMatrix.m_matParent = mHierachyMatrix.m_matInvScale * (*m_matInfo.pMatWorld);

		(*m_pLeftChild->m_matInfo.pMatWorld) = (m_pLeftChild->mHierachyMatrix.m_matOriginal) * (mHierachyMatrix.m_matParent);

		m_pLeftChild->RecursiveMatrix(&mHierachyMatrix.m_matParent);
	}

	if (m_pRightSibling != nullptr)
	{
		if (pMatParent != nullptr)
		{
			(*m_pRightSibling->m_matInfo.pMatWorld) = (m_pRightSibling->mHierachyMatrix.m_matOriginal) * (*pMatParent);
			m_pRightSibling->RecursiveMatrix(pMatParent);
		}
	}
}

AnimNode * AnimNode::RecursiveNode(AnimNode ** pAnimNode)
{
	if (m_pLeftChild != nullptr)
	{
		m_pLeftChild->RecursiveNode(&m_pLeftChild);
		return m_pLeftChild;
	}

	if (m_pRightSibling != nullptr)
	{
		m_pRightSibling->RecursiveNode(&m_pRightSibling);
		return m_pRightSibling;
	}

	return nullptr;
}

void AnimNode::Draw(size_t depth)
{
	//wcout << mName << "±íÀÌ: " << depth << endl;

	if (m_pLeftChild != nullptr)
	{
		m_pLeftChild->Draw(depth + 1);
	}

	if (m_pRightSibling != nullptr)
	{
		m_pRightSibling->Draw(depth);
	}
}

void AnimNode::MakeMatrix()
{
	*m_matInfo.matWorld = m_matInfo.matScale * m_matInfo.matRot  * m_matInfo.matTrans;
	mHierachyMatrix.m_matOriginal = *m_matInfo.matWorld;
}

void AnimNode::Set_InitMatrix(const wstring& animName)
{
	if (Set_AnimInfo(animName) == false)
		return;

	if (m_pAnimInfo == nullptr)
		return;

	if (m_pAnimInfo->cubeInfo_Space.size() <= 0)
		return;

	D3DXMatrixTranslation(&m_matInfo.matTrans, m_pAnimInfo->cubeInfo_Space[0].vPos.x, m_pAnimInfo->cubeInfo_Space[0].vPos.y, m_pAnimInfo->cubeInfo_Space[0].vPos.z);
	D3DXMatrixScaling(&m_matInfo.matScale, m_pAnimInfo->cubeInfo_Space[0].vScale.x, m_pAnimInfo->cubeInfo_Space[0].vScale.y, m_pAnimInfo->cubeInfo_Space[0].vScale.z);
	D3DXQuaternionRotationYawPitchRoll(&mAnimRot.quat, D3DXToRadian(m_pAnimInfo->cubeInfo_Space[0].vRot.y), D3DXToRadian(m_pAnimInfo->cubeInfo_Space[0].vRot.x), D3DXToRadian(m_pAnimInfo->cubeInfo_Space[0].vRot.z));
	D3DXMatrixRotationQuaternion(&m_matInfo.matRot, &mAnimRot.quat);

	MakeMatrix();
}

void AnimNode::Set_InitRot(const wstring & animName)
{
	if (Set_AnimInfo(animName) == false)
		return;

	if (m_pAnimInfo == nullptr)
		return;

	if (m_pAnimInfo->cubeInfo_Space.size() <= 0)
		return;

	D3DXQuaternionRotationYawPitchRoll(&mAnimRot.quat, D3DXToRadian(m_pAnimInfo->cubeInfo_Space[0].vRot.y), D3DXToRadian(m_pAnimInfo->cubeInfo_Space[0].vRot.x), D3DXToRadian(m_pAnimInfo->cubeInfo_Space[0].vRot.z));
	D3DXMatrixRotationQuaternion(&m_matInfo.matRot, &mAnimRot.quat);

	MakeMatrix();
}


bool AnimNode::Add_CubeInfo(CUBE_INFO cubeInfo)
{
	if (m_pAnimInfo == nullptr)
		return false;

	m_pAnimInfo->cubeInfo_Space.emplace_back(cubeInfo);

	return true;
}

void AnimNode::Set_AnimSpeed(const float scaleSpeed, const float rotSpeed, const float transSpeed)
{
	if (m_pAnimInfo == nullptr)
		return;

	m_pAnimInfo->animValue.scaleSpeed = m_pAnimInfo->anim_OriValue.scaleSpeed * scaleSpeed;
	m_pAnimInfo->animValue.rotSpeed = m_pAnimInfo->anim_OriValue.rotSpeed * rotSpeed;
	m_pAnimInfo->animValue.transSpeed = m_pAnimInfo->anim_OriValue.transSpeed * transSpeed;
}

bool AnimNode::Set_AnimInfo(const wstring & animName)
{
	ANIM_INFO* pAnimInfo = Find_AnimInfo(animName);

	if (pAnimInfo == nullptr)
		return false;

	m_pAnimInfo = pAnimInfo;

	return true;
}

ANIM_INFO* AnimNode::Find_AnimInfo(const wstring & animName)
{
	if (mAnimInfo_Space.size() < 1)
		return nullptr;

	auto iterFind = mAnimInfo_Space.find(animName);

	if (iterFind == mAnimInfo_Space.end())
		return nullptr;

	return iterFind->second;
}

void AnimNode::Set_FrameInfo(const FRAME_INFO & frameInfo)
{
	if (frameInfo.scaleFrame != -1)
	{
		mFrameInfo.scaleFrame = frameInfo.scaleFrame;
	}

	if (frameInfo.rotFrame != -1)
	{
		mFrameInfo.rotFrame = frameInfo.rotFrame;
	}

	if (frameInfo.transFrame != -1)
	{
		mFrameInfo.transFrame = frameInfo.transFrame;
	}

	mFrameInfo.bAnim = frameInfo.bAnim;
}

bool AnimNode::Add_AnimInfo(const TCHAR * pAnimName)
{
	ANIM_INFO* pAnimInfo = Find_AnimInfo(pAnimName);

	if (pAnimInfo != nullptr)
		return false;

	pAnimInfo = new ANIM_INFO();

	mAnimInfo_Space.emplace(pAnimName, pAnimInfo);

	m_pAnimInfo = pAnimInfo;
	return true;
}

ANIM_INFO * AnimNode::Find_AnimInfo(const TCHAR * pAnimName)
{
	if (mAnimInfo_Space.size() < 1)
		return nullptr;

	auto iterFind = mAnimInfo_Space.find(pAnimName);

	if (iterFind == mAnimInfo_Space.end())
		return nullptr;

	return iterFind->second;
}


void AnimNode::SetInterpolationRot(const float timeDelta)
{
	if (m_pAnimInfo == nullptr)
		return;

	if (m_pAnimInfo->anim_OriValue.RotAmount <= 0.01f)
		return;

	if (m_pAnimInfo->animInfo_Space.size() < ANIM_INFO_MIN_SIZE)
		return;

	size_t frameSize = m_pAnimInfo->animInfo_Space.size();

	if (mAnimRot.rotTime >= 1.0f)
	{
		mAnimRot.rotTime = 0.f;
		++mFrameInfo.rotFrame;

		if (mFrameInfo.rotFrame >= frameSize)
		{
			mFrameInfo.rotFrame = 0;
		}
	}
	mAnimRot.rotTime += m_pAnimInfo->animValue.rotSpeed * timeDelta;

	D3DXQuaternionRotationYawPitchRoll(&mAnimRot.aniRot[START_POINT]
		, D3DXToRadian(m_pAnimInfo->animInfo_Space[mFrameInfo.rotFrame].vRot.y)
		, D3DXToRadian(m_pAnimInfo->animInfo_Space[mFrameInfo.rotFrame].vRot.x)
		, D3DXToRadian(m_pAnimInfo->animInfo_Space[mFrameInfo.rotFrame].vRot.z));

	mAnimRot.rotNextFrame = mFrameInfo.rotFrame + 1;
	if (mAnimRot.rotNextFrame >= frameSize)
	{
		mAnimRot.rotNextFrame = 0;
	}

	D3DXQuaternionRotationYawPitchRoll(&mAnimRot.aniRot[END_POINT]
		, D3DXToRadian(m_pAnimInfo->animInfo_Space[mAnimRot.rotNextFrame].vRot.y)
		, D3DXToRadian(m_pAnimInfo->animInfo_Space[mAnimRot.rotNextFrame].vRot.x)
		, D3DXToRadian(m_pAnimInfo->animInfo_Space[mAnimRot.rotNextFrame].vRot.z));

	D3DXQuaternionSlerp(&mAnimRot.quat, &mAnimRot.aniRot[START_POINT], &mAnimRot.aniRot[END_POINT], mAnimRot.rotTime);

	D3DXMatrixRotationQuaternion(&m_matInfo.matRot, &mAnimRot.quat);
	
	MakeMatrix();
}

void AnimNode::SetInterpolationScale(const float timeDelta)
{
	if (m_pAnimInfo == nullptr)
		return;

	if (m_pAnimInfo->anim_OriValue.ScaleAmount <= 0.01f)
		return;

	if (m_pAnimInfo->cubeInfo_Space.size() < 2)
		return;

	if (mAnimScale.scaleTime >= 1.0f)
	{
		mAnimScale.scaleTime = 0.f;
		++mFrameInfo.scaleFrame;

		if (mFrameInfo.scaleFrame >= m_pAnimInfo->cubeInfo_Space.size())
		{
			mFrameInfo.scaleFrame = 0;
		}
	}

	mAnimScale.scaleTime += m_pAnimInfo->animValue.scaleSpeed  * timeDelta;

	mAnimScale.vAniScale[START_POINT] = m_pAnimInfo->cubeInfo_Space[mFrameInfo.scaleFrame].vScale;

	mAnimScale.scaleNextFrame = mFrameInfo.scaleFrame + 1;
	if (mAnimScale.scaleNextFrame >= m_pAnimInfo->cubeInfo_Space.size())
	{
		mAnimScale.scaleNextFrame = 0;
	}

	mAnimScale.vAniScale[END_POINT] = m_pAnimInfo->cubeInfo_Space[mAnimScale.scaleNextFrame].vScale;

	D3DXVec3Lerp(&mAnimScale.vScale, &mAnimScale.vAniScale[START_POINT], &mAnimScale.vAniScale[END_POINT], mAnimScale.scaleTime);
	D3DXMatrixScaling(&m_matInfo.matScale, mAnimScale.vScale.x, mAnimScale.vScale.y, mAnimScale.vScale.z);

	MakeMatrix();
}

void AnimNode::SetInterpolationTrans(const float timeDelta)
{
	if (m_pAnimInfo == nullptr)
		return;

	if (m_pAnimInfo->anim_OriValue.TransAmount <= 0.01f)
		return;

	if (m_pAnimInfo->animInfo_Space.size() < ANIM_INFO_MIN_SIZE)
		return;

	size_t frameSize = m_pAnimInfo->animInfo_Space.size();

	if (mAnimTrans.transTime >= 1.0f)
	{
		mAnimTrans.transTime = 0.f;
		++mFrameInfo.transFrame;

		if (mFrameInfo.transFrame >= frameSize)
		{
			mFrameInfo.transFrame = 0;
		}
	}
	mAnimTrans.transTime += m_pAnimInfo->animValue.transSpeed * timeDelta;

	mAnimTrans.vAnimTrans[START_POINT] = m_pAnimInfo->animInfo_Space[mFrameInfo.transFrame].vPos;

	mAnimTrans.transNextFrame = mFrameInfo.transFrame + 1;
	if (mAnimTrans.transNextFrame >= frameSize)
	{
		mAnimTrans.transNextFrame = 0;
	}

	mAnimTrans.vAnimTrans[END_POINT] = m_pAnimInfo->animInfo_Space[mAnimTrans.transNextFrame].vPos;

	D3DXVec3Lerp(&mAnimTrans.vTrans, &mAnimTrans.vAnimTrans[START_POINT], &mAnimTrans.vAnimTrans[END_POINT], mAnimTrans.transTime);
	D3DXMatrixTranslation(&m_matInfo.matTrans, mAnimTrans.vTrans.x, mAnimTrans.vTrans.y, mAnimTrans.vTrans.z);

	MakeMatrix();
}

void AnimNode::ChildToSibling(AnimNode * pAnimNode)
{
	AnimNode* pTempNode = nullptr;
	AnimNode* pPreNode = nullptr;
	AnimNode* pParentNode = nullptr;

	if (m_pLeftChild != nullptr)
	{
		if (m_pLeftChild == pAnimNode)
		{
			if (m_pLeftChild->m_pRightSibling != nullptr)
			{
				m_pLeftChild = m_pLeftChild->m_pRightSibling;
			}
			else
			{
				pParentNode = m_pLeftChild->m_pParent;

				if (pParentNode != nullptr)
				{
					if (m_pLeftChild->m_pLeftChild != nullptr)
					{
						m_pLeftChild->m_pLeftChild->m_pParent = pParentNode;
						pParentNode->m_pLeftChild = m_pLeftChild->m_pLeftChild;
					}
					else
					{
						m_pLeftChild = nullptr;
					}
				}
			}
		}
		else
		{
			m_pRightSibling = m_pLeftChild->m_pRightSibling;

			pPreNode = m_pLeftChild;
			pTempNode = m_pRightSibling;

			while (pTempNode != pAnimNode)
			{
				pPreNode = pTempNode;
				pTempNode = pTempNode->m_pRightSibling;
			}

			if (pTempNode->m_pRightSibling != nullptr)
			{
				pPreNode->m_pRightSibling = pTempNode->m_pRightSibling;
			}
			else
			{
				pPreNode->m_pRightSibling = nullptr;
			}

			m_pRightSibling = nullptr;
		}
	}
}



AnimNode * AnimNode::Create(LPDIRECT3DDEVICE9 pDevice)
{
	AnimNode* pInstance = new AnimNode(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("Animation Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component * AnimNode::Clone()
{
	return new AnimNode(*this);
}

DWORD AnimNode::Free()
{
	m_pLeftChild = nullptr;
	m_pRightSibling = nullptr;
	m_pParent = nullptr;

	for (PAIR_ANIMINFO pair : mAnimInfo_Space)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	mAnimInfo_Space.clear();

	Component::Free();
	return 0;
}
