#ifndef ENGINE_STRUCT_H__
#define ENGINE_STRUCT_H__

namespace Engine
{
	typedef struct tagVertexColor
	{
		D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
		DWORD color = 0;
	}VTXCOL;
	constexpr DWORD VTXCOL_FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexNoramlColor
	{
		D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
		D3DXVECTOR3 vNormal = { 0.f, 0.f, 0.f };
		DWORD color = 0;
	}VTXNORMAL_COL;
	constexpr DWORD VTXNORMAL_COL_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;

	typedef struct tagVertexTexture
	{
		D3DXVECTOR3 vPos = { 0.f, 0.f, 0.f };
		D3DXVECTOR2 vTexUV = { 0.f, 0.f };
	}VTXTEX;
	constexpr DWORD VTXTEX_FVF = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertex_NormalTexture
	{
		D3DXVECTOR3	vPos = { 0.f, 0.f, 0.f };
		D3DXVECTOR3	vNormal = { 0.f, 0.f, 0.f };
		D3DXVECTOR2	vTexUV = { 0.f, 0.f };
	}VTXNORMAL_TEX;
	constexpr DWORD VTXNORMAL_TEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;


	typedef struct tagVertexCube
	{
		D3DXVECTOR3		vPos;
		D3DXVECTOR3		vTex;
	}VTXCUBE;
	constexpr DWORD	VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagVertexNormalCube
	{
		D3DXVECTOR3	vPos = { 0.f ,0.f, 0.f };
		D3DXVECTOR3	vNormal = { 0.f, 0.f, 0.f };
		D3DXVECTOR3	vTex = { 0.f, 0.f, 0.f };
	}VTX_NORMAL_CUBE;
	constexpr DWORD	VTXFVF_NORAML_CUBE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	// TY_UI_180627_ Add Struct For ViewportTexture
	typedef struct tagVertex_UI_Texture
	{
		D3DXVECTOR4	vPos; // ViewPort영역상의 위치를 설정한다.
		D3DXVECTOR2	vTexUV;
	}VTX_UI;

	constexpr DWORD	VTXFVF_UI= D3DFVF_XYZRHW | D3DFVF_TEX1;


	typedef struct tagPolygon16
	{
		WORD _1, _2, _3;
	}POLYGON16;

	typedef struct tagPolygon32
	{
		DWORD _1, _2, _3;
	}PLYGON32;

	typedef struct tagMatrix
	{
		tagMatrix()
		{
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixIdentity(&matScale);
		};

		D3DXMATRIX* pMatWorld = nullptr;
		D3DXMATRIX matScale;
		D3DXMATRIX matRot;
		D3DXMATRIX matTrans;

	}MATRIX_INFO;
}


#endif