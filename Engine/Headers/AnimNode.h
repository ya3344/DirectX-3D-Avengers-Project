#ifndef ANIMATION_H__
#define ANIMATION_H__

#include "Component.h"
#include "AnimationStruct.h"

_BEGIN(Engine)

class _ENGINE_DLL AnimNode final : public Component
{
private:
	explicit AnimNode(LPDIRECT3DDEVICE9 pDevice);
	explicit AnimNode(const AnimNode& rhs);
	virtual ~AnimNode() = default;

public:
	HRESULT Init();

public:
	void SetMatrixWorld(D3DXMATRIX* matWorld) { m_matInfo.matWorld = matWorld; }
	void SetName(const wstring& name) { mName = name; }
	void Set_ParentName(const wstring& parentName) { mParentName = parentName; }

public:
	AnimNode* GetParent() { return m_pParent; }
	AnimNode* Get_LeftChild() const { return m_pLeftChild; }
	ANIM_INFO* GetAnimInfo() const { return m_pAnimInfo; }

public:
	const wstring& GetName() { return mName; }
	const wstring& Get_ParentName() { return mParentName; }

public: // Scale, Move, Pos Matrix Related Funciton
	void Set_InitMatrix(const wstring& animName);
	void Set_InitRot(const wstring& animName);

public: // Animation Hierarchy Related Function
	void AddChild(AnimNode* pAnimNode);
	void RecursiveMatrix(const D3DXMATRIX* pMatParent = nullptr);
	AnimNode* RecursiveNode(AnimNode** pAnimNode);
	void Draw(size_t depth); // Debug Funciton
	void MakeMatrix();

public: // Animation Info Releated Function
	bool Add_CubeInfo(CUBE_INFO cubeInfo);
	void Set_AnimSpeed(const float scaleSpeed, const float rotSpeed, const float transSpeed);
	bool Set_AnimInfo(const wstring& animName);
	ANIM_INFO* Find_AnimInfo(const wstring& animName);
	void Set_AnimValueInfo(const ANIM_VALUE& animVale) { m_pAnimInfo->anim_OriValue = animVale; }

public: // Frame Info Releated Function
	const FRAME_INFO& Get_FrameInfo() const { return mFrameInfo; }
	void Set_FrameInfo(const FRAME_INFO& frameInfo);

public:// AnimInfo Map Container Related Function
	bool Add_AnimInfo(const TCHAR* pAnimName);
	ANIM_INFO* Find_AnimInfo(const TCHAR* pAnimName);

public: // Interpolation Related Function
	void SetInterpolationRot(const float timeDelta);
	void SetInterpolationScale(const float timeDelta);
	void SetInterpolationTrans(const float timeDelta);

public: // Hierachyt Delete Related Function
	void ChildToSibling(AnimNode* pAnimNode = nullptr);

public:
	static AnimNode* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

private:
	static constexpr size_t ANIM_INFO_MIN_SIZE = 2;

private: // Animation Hierarchy Related Variable
	AnimNode* m_pLeftChild = nullptr;
	AnimNode* m_pRightSibling = nullptr;
	AnimNode* m_pParent = nullptr;
	HIERACHY_MATRIX mHierachyMatrix;

private: // Animation Common Related Variable
	MATRIX_INFO m_matInfo;
	CUBE_INFO* m_pCubeInfo = nullptr;
	FRAME_INFO mFrameInfo;
	ANIM_INFO* m_pAnimInfo = nullptr;
	wstring mName = L"";
	wstring mParentName = L"";

private:// AnimInfo Map Container Related Variable
	unordered_map<wstring, ANIM_INFO*>  mAnimInfo_Space;

private: //Interpolation Related Variable
	ANIM_ROT mAnimRot;
	ANIM_SCALE mAnimScale;
	ANIM_TRANS mAnimTrans;

};

_END

#endif
