#ifndef STRUCT_H__
#define STRUCT_H__


namespace Engine
{
	class Transform;
	class AnimNode;
}

namespace Client
{
	class AnimCube;
}

typedef struct SuperCube_Info
{
	Client::AnimCube* pCube = nullptr;
	Engine::AnimNode* pAnimNode = nullptr;
	Engine::Transform* pTransform = nullptr;
	const D3DXVECTOR3* pMyPos = nullptr;
	const D3DXVECTOR3* pMyLook = nullptr;
	float *pMaxHP = nullptr;
	float *pHP = nullptr;
}SUPER_CUBE_INFO;



// JH_ThanosAdd_180703 AnimCubeInfo Struct Add
typedef struct AnimCube_Info
{
	AnimCube_Info() {}
	explicit AnimCube_Info(const bool bAreadyComponent, const wstring& name, const wstring& parentName, const size_t textureNum, const WORD comSceneIdx, const TCHAR* pComTextureTag)
	{
		this->bAlreadyComponent = bAreadyComponent;
		this->name = name;
		this->parentName = parentName;
		this->textureNum = textureNum;
		this->comSceneIdx = comSceneIdx;
		this->pComTextureTag = pComTextureTag;
	}

	bool bAlreadyComponent = false;
	wstring name = L"";
	wstring parentName = L"";
	size_t textureNum = 0;
	WORD comSceneIdx = 0;
	const TCHAR* pComTextureTag = nullptr;

}ANIM_CUBE_INFO;

// TY_FixBullet_180702 Add Bullet Info Struct
typedef struct tag_BulletInfo
{
	tag_BulletInfo() {}

	explicit tag_BulletInfo(size_t sceneIdx, TCHAR * pProtoTag, size_t textureNum, D3DXVECTOR3 vPos, float angle)
	{
		this->sceneIdx = sceneIdx;
		this->pProtoTag = pProtoTag;
		this->textureNum = textureNum;
		this->vPos = vPos;
		this->angle = angle;
	}

	size_t sceneIdx = 0;
	TCHAR * pProtoTag = nullptr;
	size_t textureNum = 0;
	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	float angle = 0.f;
	float speed = 0.f;

}BULLET_INFO;

// JH_Effect_180705 EffectInitInfo Struct 추가
typedef struct Effect_InitInfo
{
	Effect_InitInfo() {}
	explicit Effect_InitInfo(const TCHAR* pTextureName, const size_t frameMax, const float frameSpeed, const bool bForver = false)
	{
		this->pTextureName = pTextureName;
		this->frameMax = frameMax;
		this->frameSpeed = frameSpeed;
		this->bForever = bForever;
	}
	
	const TCHAR* pTextureName = nullptr;
	size_t frameMax = 0;
	float frameSpeed = 0.f;
	bool bForever = false;

}EFFECT_INIT_INFO;

// JH_Effect_180705 Effect_Info Struct 추가
typedef struct Effect_Info
{
	Effect_Info() {}
	explicit Effect_Info(const D3DXVECTOR3& vPos, const D3DXVECTOR3& vRandomPos, const D3DXVECTOR3& vScale)
	{
		this->vPos = vPos;
		this->vRandomPos = vRandomPos;
		this->vScale = vScale;
	}

	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vRandomPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vScale = { 0.f, 0.f, 0.f };

}EFFECT_INFO;

typedef struct PlanetInfo
{
	PlanetInfo() {}
	explicit PlanetInfo(const size_t textureNum, const float rotSpeed, const float revolSpeed, const D3DXVECTOR3& vPos, const D3DXVECTOR3& vScale)
	{
		this->textureNum = textureNum;
		this->rotSpeed = rotSpeed;
		this->revolSpeed = revolSpeed;
		this->vPos = vPos;
		this->vScale = vScale;
	}

	size_t textureNum = 0;
	float rotSpeed = 0.f;
	float revolSpeed = 0.f;
	float rotDegree = 0.f;
	float revolRotDegree = 0.f;
	D3DXVECTOR3 vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vScale = D3DXVECTOR3(0.f, 0.f, 0.f);

}PLANET_INFO;

#endif

