#ifndef TERRAIN_CUBE_H__
#define TERRAIN_CUBE_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class CubeTex;
	class Texture;
}

_BEGIN(Client)

class TerrainCube final : public Engine::GameObject
{
private:
	explicit TerrainCube(LPDIRECT3DDEVICE9 pDevice);
	explicit TerrainCube(const TerrainCube& rhs);
	virtual ~TerrainCube() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT Ready_GameObjectPrototype();

private:
	HRESULT ReadyComponent(void);

public:	//TY_TerrainCube Class_180614 Add Get &Set Functions
	const D3DXVECTOR3& GetPos() const { return m_vPos; }
	const D3DXVECTOR3& GetRotRadian() const { return m_vRot; } 	//TY_GetRotRadian_180615 Add Function GetRotRadian
	const Engine::MATRIX_INFO& GetMatrixInfo() const { return mMatrixInfo; }

public:
	void SetWorldMatrix();
	void SetPos(const D3DXVECTOR3& vPos) { m_vPos = vPos; }
	void SetRotRadian(const D3DXVECTOR3& vRot) { m_vRot = vRot; }
	void SetRevolRadian(const D3DXVECTOR3& vRevol) { m_vRevol = vRevol; }
	void SetTexNum(const size_t& texNum) { mTextureNum = texNum; }
	void SetParentMatrix(const D3DXMATRIX& matParent) { m_matParent = matParent; }

	void SetTrans(const D3DXVECTOR3& vTrans) { m_vPos += vTrans; };
	void SetRotation(const D3DXVECTOR3& vRot) { m_vRot += vRot; }
	void SetRevolve(const D3DXVECTOR3& vRevol) { m_vRevol += vRevol; }

	void SetCubeState_Transform(bool bTransform) { m_bSetTransform = bTransform; }

public:
	static TerrainCube* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free(void) override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::CubeTex* m_pBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;

private:
	size_t mTextureNum = 0;

private: //TY_TerrainCube Info_180614 Variables Related To TerrainCube
	Engine::MATRIX_INFO mMatrixInfo;
	D3DXMATRIX m_matRevolve, m_matParent;
	D3DXVECTOR3 m_vPos = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vRot = { 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vRevol = { 0.f, 0.f, 0.f };

private:
	bool m_bSetTransform = true;
};

_END

#endif //TERRAIN_CUBE_H__

