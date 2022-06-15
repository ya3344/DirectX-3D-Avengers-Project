#include "stdafx.h"
#include "AnimMgr.h"
#include "Layer.h"
#include "AnimCube.h"
#include "ComponentMgr.h"

_USING(Client)

AnimMgr::AnimMgr(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{

}

AnimMgr::AnimMgr(const AnimMgr & rhs)
	: Component(rhs)
	, m_pAnimLayer(rhs.m_pAnimLayer)
	, m_pGameObject_Space(rhs.m_pGameObject_Space)
{
}

HRESULT AnimMgr::Init()
{
	return NO_ERROR;
}

void AnimMgr::Set_AnimLayer(Engine::Layer * pLayer)
{
	if (pLayer != nullptr)
	{
		m_pAnimLayer = pLayer;
		m_pGameObject_Space = m_pAnimLayer->Get_GameObjectSpace();
	}
}

HRESULT AnimMgr::SetHierachy()
{
	if (m_pGameObject_Space->size() <= 0)
		return E_FAIL;

	Engine::AnimNode* pAnimNode = nullptr;
	Engine::AnimNode* pParent_AnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();


	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			return E_FAIL;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			return E_FAIL;

		if (pAnimNode->Get_ParentName() != L"NotParent")
		{
			pParent_AnimNode = FindHierachy(pAnimNode->Get_ParentName());
			if (pParent_AnimNode == nullptr)
				return E_FAIL;

			pParent_AnimNode->AddChild(pAnimNode);
		}
	}

	return NO_ERROR;
}

Engine::AnimNode * AnimMgr::FindHierachy(const wstring & parentName)
{
	if (m_pGameObject_Space->size() <= 0)
		return nullptr;

	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			return false;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			return false;

		if (pAnimNode != nullptr)
		{
			if (pAnimNode->GetName() == parentName)
			{
				return pAnimNode;
			}
		}
	}

	return nullptr;
}

HRESULT AnimMgr::Delete_HierachyCube(const wstring & cubeName)
{
	if (m_pGameObject_Space->size() <= 0)
		return E_FAIL;

	Engine::AnimNode* pAnimNode = nullptr;
	Engine::AnimNode* pChildAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; )
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			return E_FAIL;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			return E_FAIL;

		if (pAnimNode->GetName() == cubeName)
		{
			pChildAnimNode = pAnimNode->Get_LeftChild();
			if (pChildAnimNode != nullptr)
			{
				RecursiveDelete(pChildAnimNode);
				DeleteCube(pChildAnimNode->GetName());
			}

			DeleteCube(cubeName);
			return NO_ERROR;
		}
		else
		{
			++iterBegin;
		}
	}

	return NO_ERROR;
}

void AnimMgr::RecursiveDelete(Engine::AnimNode * pAnimNode)
{
	Engine::AnimNode* pDeleteNode = nullptr;
	Engine::AnimNode* pTempNode = nullptr;

	pTempNode = pAnimNode->RecursiveNode(&pDeleteNode);
	while (pTempNode != nullptr)
	{
		pDeleteNode = pTempNode;
		pTempNode = pTempNode->RecursiveNode(&pDeleteNode);
		DeleteCube(pDeleteNode->GetName());
	}

}

HRESULT AnimMgr::DeleteCube(const wstring & cubeName)
{
	Engine::AnimNode* pAnimNode = nullptr;
	Engine::AnimNode* pChildAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; )
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			return E_FAIL;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			return E_FAIL;

		if (pAnimNode->GetName() == cubeName)
		{
			if (pAnimNode->GetParent() != nullptr)
			{
				pAnimNode->GetParent()->ChildToSibling(pAnimNode);
			}

			Engine::Safe_Release(pAnimCube);
			iterBegin = m_pGameObject_Space->erase(iterBegin);

			return NO_ERROR;
		}
		else
		{
			++iterBegin;
		}
	}

	return NO_ERROR;
}

HRESULT AnimMgr::DeleteAllCube()
{
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; )
	{
		pAnimCube = (AnimCube*)(*iterBegin);

		if (pAnimCube == nullptr)
			return E_FAIL;

		if (pAnimCube->GetName() == L"Waist")
		{
			++iterBegin;
			continue;
		}


		Engine::Safe_Release(pAnimCube);
		iterBegin = m_pGameObject_Space->erase(iterBegin);
	}


	return NO_ERROR;
}

bool AnimMgr::Set_AnimInfo(const wstring & animName)
{
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			continue;

		if (pAnimNode != nullptr)
		{
			if (pAnimNode->Set_AnimInfo(animName) == false)
			{
				_MSG_BOX("SetAnimInfo Failed!");
				return false;
			}
		}
	}

	return true;
}

void AnimMgr::Set_FrameInfo(const FRAME_INFO & frameInfo)
{
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			continue;

		if (pAnimNode != nullptr)
		{
			pAnimNode->Set_FrameInfo(frameInfo);
		}
	}
}

void AnimMgr::Set_InitRot(const wstring& animName)
{
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			continue;

		if (pAnimNode != nullptr)
		{
			if (pAnimNode->GetName() != L"Waist")
			{
				pAnimNode->Set_InitRot(animName);
			}

		}
	}
}

void AnimMgr::Set_InitMatrix(const wstring & animName)
{
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			continue;

		if (pAnimNode != nullptr)
		{
			if (pAnimNode->GetName() != L"Waist")
			{
				pAnimNode->Set_InitMatrix(animName);
			}

		}
	}
}

void AnimMgr::SetShiled_RenderState(const bool renderState)
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		if (pAnimCube->GetName().find(L"Shield") == string::npos)
			continue;

		pAnimCube->SetShiledRender(renderState);
	}
}

const FRAME_INFO * AnimMgr::Get_FrameInfo(const TCHAR* pCubeName) const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{

		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		if (pAnimCube->GetName() != pCubeName)
			continue;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			continue;

		if (pAnimNode != nullptr)
		{
			return &pAnimNode->Get_FrameInfo();
		}
	}

	return nullptr;
}

const ANIM_INFO * AnimMgr::Get_AnimInfo(const TCHAR * pCubeName) const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		if (pAnimCube->GetName() != pCubeName)
			continue;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			continue;

		if (pAnimNode != nullptr)
		{
			return pAnimNode->GetAnimInfo();
		}
	}

	return nullptr;
}

void AnimMgr::Set_AnimSpeed(const float scaleSpeed, const float rotSpeed, const float transSpeed)
{
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			continue;

		if (pAnimNode != nullptr)
		{
			pAnimNode->Set_AnimSpeed(scaleSpeed, rotSpeed, transSpeed);
		}
	}

}

void AnimMgr::Render()
{
	if (m_pAnimLayer != nullptr)
	{
		m_pAnimLayer->GameObjectRender();
	}
}

void AnimMgr::InterpolationUpdate(const float timeDelta)
{
	Engine::AnimNode* pAnimNode = nullptr;
	AnimCube* pAnimCube = nullptr;

	auto iterBegin = m_pGameObject_Space->begin();
	auto iterEnd = m_pGameObject_Space->end();

	for (; iterBegin != iterEnd; ++iterBegin)
	{
		pAnimCube = (AnimCube*)(*iterBegin);
		if (pAnimCube == nullptr)
			continue;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
			continue;

		if (pAnimNode != nullptr)
		{
			if (pAnimNode->Get_FrameInfo().bAnim)
			{
				pAnimNode->SetInterpolationScale(timeDelta);
				pAnimNode->SetInterpolationRot(timeDelta);
				pAnimNode->SetInterpolationTrans(timeDelta);
			}
		}
	}
}

AnimMgr * AnimMgr::Create(LPDIRECT3DDEVICE9 pDevice)
{
	AnimMgr* pInstance = new AnimMgr(pDevice);

	if (FAILED(pInstance->Init()))
	{
		_MSG_BOX("AnimMgr Created Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

AnimMgr * AnimMgr::Clone()
{
	return new AnimMgr(*this);
}

DWORD AnimMgr::Free()
{
	Component::Free();
	return 0;
}
