#include "stdafx.h"
#include "PathData.h"

_IMPLEMENT_SINGLETON(PathData);

PathData::PathData()
{
}

void PathData::AddCameraPathData(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir)
{
	PATH_DATA* pPath = new PATH_DATA(vPos, vDir);

	mPathData_Space.push_back(pPath);
}

void PathData::SaveCameraPathData(const TCHAR* pPath)
{
	size_t pathData_Num = mPathData_Space.size();

	if (0 == pathData_Num || nullptr == pPath)
	{
		_MSG_BOX("Missing Path Data");
		return;
	}

	TCHAR pathName[MAX_PATH] = L"";
	wsprintf(pathName, L"../Bin/Data/PathData/%s.dat", pPath);

	// 파일 이름 인자로 넣어 불러올 수 있게 수정해놓은 버전으로 바꾸기
	HANDLE hFile = CreateFile(pathName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (FALSE == hFile)
	{
		_MSG_BOX("Missing Path Data");
		CloseHandle(hFile);
		return;
	}

	DWORD dwByte;

	WriteFile(hFile, &pathData_Num, sizeof(int), &dwByte, nullptr);

	for (PATH_DATA* pPath : mPathData_Space)
	{
		WriteFile(hFile, pPath, sizeof(PATH_DATA), &dwByte, nullptr);
	}

	CloseHandle(hFile);
}

void PathData::LoadCameraPathData(vector<PATH_DATA*>* PathData_Space, const TCHAR* pPath)
{
	TCHAR pathName[MAX_PATH] = L"";
	wsprintf(pathName, L"../Bin/Data/PathData/%s.dat", pPath);

	HANDLE hFile = CreateFile(pathName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (FAILED(hFile))
	{
		_MSG_BOX("Load Camera PathData Failed");
		return;
	}

	DWORD		dwByte = 1;
	size_t pathData_Num = 0;
	PATH_DATA* pBufferPath = nullptr;

	ReadFile(hFile, &pathData_Num, sizeof(int), &dwByte, nullptr);

	mPathNum = pathData_Num;
	for (size_t i = 0; i < pathData_Num; ++i)
	{
		if (dwByte == 0)
		{
			break;
		}

		pBufferPath = new PATH_DATA;

		ReadFile(hFile, pBufferPath, sizeof(PATH_DATA), &dwByte, nullptr);

		mPathData_Space.push_back(pBufferPath);
	}

	mPathData_Space.swap(*PathData_Space);

	CloseHandle(hFile);
}

DWORD PathData::Free()
{
	//SaveCameraPathData(L"path_data_StageChange");

	for (PATH_DATA* pPath : mPathData_Space)
	{
		if (nullptr != pPath)
		{
			delete pPath;
			pPath = nullptr;
		}
	}
	mPathData_Space.clear();

	return 0;
}
