#ifndef ANIMATION_STRUCT_H__
#define ANIMATION_STRUCT_H__

enum ANIM_INTERVAL
{
	START_POINT,
	END_POINT,
	ANIM_END
};

typedef struct tagAnimValue
{
	float ScaleAmount = 0.f;
	float scaleSpeed = 0.f;

	float TransAmount = 0.f;
	float transSpeed = 0.f;

	float RotAmount = 0.f;
	float rotSpeed = 0.f;

}ANIM_VALUE;

typedef struct tagCubeInfo
{
	tagCubeInfo() {};
	explicit tagCubeInfo(D3DXVECTOR3 vPos, D3DXVECTOR3 vScale, D3DXVECTOR3 vRot)
	{
		this->vPos = vPos;
		this->vScale = vScale;
		this->vRot = vRot;
	}

	D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vScale = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 vRot = { 0.f, 0.f, 0.f };

}CUBE_INFO;

typedef struct tagAnimInfo
{
	tagAnimInfo()
	{
		animInfo_Space.clear();
	}
	~tagAnimInfo()
	{
		animInfo_Space.clear();
	}

	vector<CUBE_INFO> animInfo_Space;
	ANIM_VALUE animValue;
	ANIM_VALUE anim_OriValue;

}ANIM_INFO;

typedef struct tagFrameInfo
{
	tagFrameInfo() {};
	explicit tagFrameInfo(const size_t scaleFrame, const size_t rotFrame, const size_t transFrame, bool bAnim)
	{
		this->scaleFrame = scaleFrame;
		this->rotFrame = rotFrame;
		this->transFrame = transFrame;
		this->bAnim = bAnim;
	}

	size_t scaleFrame = 0;
	size_t rotFrame = 0;
	size_t transFrame = 0;
	bool bAnim = false;

}FRAME_INFO;

typedef struct tagAnimRotation
{
	float rotTime = 0.f;
	size_t rotNextFrame = 0;
	D3DXQUATERNION quat;
	D3DXQUATERNION aniRot[ANIM_END] = {};

}ANIM_ROT;

typedef struct tagAnimScale
{
	float scaleTime = 0.f;
	size_t scaleNextFrame = 0;
	D3DXVECTOR3 vScale = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vAniScale[ANIM_END] = {};

}ANIM_SCALE;

typedef struct tagAnimTrans
{
	float transTime = 0.f;
	size_t transNextFrame = 0;
	D3DXVECTOR3 vTrans = D3DXVECTOR3(0.f, 0.f, 0.f);
	D3DXVECTOR3 vAnimTrans[ANIM_END] = {};

}ANIM_TRANS;


typedef struct tagHierachyMatrix
{
	tagHierachyMatrix()
	{
		D3DXMatrixIdentity(&m_matOriginal);
		D3DXMatrixIdentity(&m_matInvScale);
		D3DXMatrixIdentity(&m_matParent);
	}

	D3DXMATRIX m_matOriginal;
	D3DXMATRIX m_matInvScale;
	D3DXMATRIX m_matParent;

}HIERACHY_MATRIX;


typedef unordered_map<wstring, ANIM_INFO*> MAP_ANIMIFNO;
typedef pair<wstring, ANIM_INFO*> PAIR_ANIMINFO;


#endif