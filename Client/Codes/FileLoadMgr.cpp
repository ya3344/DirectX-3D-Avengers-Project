#include "stdafx.h"
#include "FileLoadMgr.h"
#include "AnimNode.h"
#include "AnimCube.h"
#include "ObjectMgr.h"

_USING(Client)

_IMPLEMENT_SINGLETON(FileLoadMgr)

FileLoadMgr::FileLoadMgr()
{

}

HRESULT FileLoadMgr::Obj_FileLoad(LPDIRECT3DDEVICE9 pDevice, size_t sceneIdx, const TCHAR* pLayerTag, const TCHAR* pComTextureTag, const TCHAR* pFilePath)
{
	HANDLE hFile = CreateFile(pFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		_MSG_BOX("FileLoad INVALID_HANDLE_VALUE!");
		return E_FAIL;
	}

	DWORD byte = 0;

	TCHAR name[20] = L"";
	TCHAR parentName[20] = L"";
	TCHAR animName[20] = L"";
	wstring objName = L"";

	size_t cubeSpace_size = 0;
	size_t cubeInfo_size = 0;
	size_t animSize = 0;
	size_t texNum = 0;

	CUBE_INFO cubeInfo;
	ANIM_VALUE animValue;

	AnimCube* pAnimCube = nullptr;
	Engine::AnimNode *pAnimNode = nullptr;
	bool bCheck = false;

	ReadFile(hFile, &cubeSpace_size, sizeof(size_t), &byte, nullptr);

	for (size_t i = 0; i < cubeSpace_size; ++i)
	{
		ReadFile(hFile, &texNum, sizeof(size_t), &byte, nullptr);
		ReadFile(hFile, &name, sizeof(name), &byte, nullptr);
		ReadFile(hFile, &parentName, sizeof(parentName), &byte, nullptr);

		mNameSpace.emplace_back(name);
		// pDevice, true, name, parentName, texNum, pComTextureTag

		pAnimCube = AnimCube::Create(pDevice, ANIM_CUBE_INFO(true, name, parentName, texNum, (WORD)sceneIdx, pComTextureTag));

		if (pAnimCube == nullptr)
			return E_FAIL;

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectPrototype(mNameSpace[i].c_str(), sceneIdx, pAnimCube)))
			return E_FAIL;

		if (FAILED(Engine::ObjectMgr::GetInstance()->Add_GameObjectToLayer(sceneIdx, mNameSpace[i].c_str(), sceneIdx, pLayerTag)))
			return E_FAIL;

		pAnimNode = pAnimCube->Get_AnimNode();
		if (pAnimNode == nullptr)
		{
			_MSG_BOX("FileLoad Animation Crated Failed!")
				return E_FAIL;
		}

		ReadFile(hFile, &animSize, sizeof(size_t), &byte, nullptr);

		for (size_t i = 0; i < animSize; i++)
		{
			ReadFile(hFile, &animName, sizeof(animName), &byte, nullptr);
			pAnimNode->Add_AnimInfo(animName);

			if (pAnimNode->GetAnimInfo() == nullptr)
			{
				_MSG_BOX("FileLoad AnimInfo Crated Failed!")
					return E_FAIL;
			}

			ReadFile(hFile, &animValue, sizeof(animValue), &byte, nullptr);
			pAnimNode->Set_AnimValueInfo(animValue);

			ReadFile(hFile, &cubeInfo_size, sizeof(size_t), &byte, nullptr);

			for (size_t i = 0; i < cubeInfo_size; i++)
			{
				ReadFile(hFile, &cubeInfo, sizeof(CUBE_INFO), &byte, nullptr);
				bCheck = pAnimNode->Add_CubeInfo(cubeInfo);

				if (bCheck == false)
				{
					_MSG_BOX("Add CubeInfo Failed!");
					return E_FAIL;
				}
			}
		}

		// 정점 초기화
		pAnimNode->Set_InitMatrix(L"Stand");
	}

	int finalTest = 0;
	ReadFile(hFile, &finalTest, sizeof(int), &byte, nullptr);

	if (byte == 0)
	{
		//_MSG_BOX("FileLoad Success!");
	}
	else
	{
		_MSG_BOX("FileLoad FinalTest Fail!");
		return E_FAIL;
	}

	CloseHandle(hFile);

	return NO_ERROR;
}

DWORD FileLoadMgr::Free()
{
	mNameSpace.clear();
	return 0;
}
