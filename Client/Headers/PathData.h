#ifndef PATHDATA_H__
#define PATHDATA_H__

#include "Base.h"
#include "Include.h"

typedef struct tag_Path
{
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vDir = { 0.f, 0.f, 0.f };

	tag_Path() {}
	tag_Path(D3DXVECTOR3 vPos, D3DXVECTOR3 vDir)
	{
		this->vPos = vPos;
		this->vDir = vDir;
	}

}PATH_DATA;

class PathData final : public Engine::Base
{
	_DECLARE_SINGLETON(PathData)

private:
	explicit PathData();
	virtual ~PathData() = default;

public:
	size_t GetPathDataNum() const { return mPathNum; }

public:
	void AddCameraPathData(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vDir);
	void SaveCameraPathData(const TCHAR* pPath);
	void LoadCameraPathData(vector<PATH_DATA*>* PathData_Space, const TCHAR* pPath);

public:
	virtual DWORD Free() override;

private:
	vector<PATH_DATA*> mPathData_Space;
	size_t mPathNum = 0;

};
#endif

