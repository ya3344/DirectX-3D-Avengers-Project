#include "Management.h"
#include "Scene.h"
#include "ObjectMgr.h"


_USING(Engine);

_IMPLEMENT_SINGLETON(Management)

Management::Management()
{
}


UPDATE Management::Update(const float timeDelta)
{
	if (m_pCurrentScene == nullptr)
		return UPDATE::FAIL;

	UPDATE exitCode = UPDATE::FAIL;

	exitCode = m_pCurrentScene->Update(timeDelta);
	if (exitCode == UPDATE::FAIL)
		return exitCode;

	exitCode = m_pCurrentScene->LastUpdate(timeDelta);
	if (exitCode == UPDATE::FAIL)
		return exitCode;

	return exitCode;
}

void Management::Render(void)
{
	if (m_pCurrentScene == nullptr)
		return;

	m_pCurrentScene->Render();
}

HRESULT Management::ReserveContainer(const WORD maxScene)
{
	if (FAILED(ObjectMgr::GetInstance()->ReserveContainer(maxScene)))
		return E_FAIL;

	return NOERROR;
}

HRESULT Management::SetUp_CurrentScene(Scene * pNewScene)
{
	if (pNewScene == nullptr)
		return E_FAIL;

	DWORD refCnt = 0;

	if (refCnt = Safe_Release(m_pCurrentScene))
		return E_FAIL;

	m_pCurrentScene = pNewScene;

	m_pCurrentScene->AddRef();

	return NOERROR;
}

DWORD Management::Free(void)
{
	DWORD refCnt = 0;

	if (refCnt = Safe_Release(m_pCurrentScene))
	{

	}

	return refCnt;
}
