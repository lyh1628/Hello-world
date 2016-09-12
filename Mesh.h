#pragma once

/*정점의 색상을 무작위로(Random) 설정하기 위해 사용한다.
각 정점의 색상은 난수(Random Number)를 생성하여 지정한다.*/
#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

struct MESHINTERSECTINFO {
	DWORD m_dwFaceIndex;
	float m_fU;
	float m_fV;
	float m_fDistance;
};

class CVertex
{
	//정점의 위치 정보(3차원 벡터)를 저장하기 위한 멤버 변수를 선언한다. 
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//CVertex 클래스의 생성자와 소멸자를 다음과 같이 선언한다. 
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
};

class AABB
{
public:
	//바운딩 박스의 최소점과 최대점을 나타내는 벡터이다.
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	//두 개의 바운딩 박스를 합한다.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Union(AABB *pAABB);
	//바운딩 박스의 8개의 꼭지점을 행렬로 변환하고 최소점과 최대점을 다시 계산한다.
	void Update(D3DXMATRIX *pd3dxmtxTransform);
};

class CMesh
{
public:
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

private:
	int m_nReferences;

public:
	void AddRef();
	void Release();
	//Mesh.cpp 파일에서 CMesh::AddRef(), CMesh::Release() 함수의 정의를 삭제한다. 

protected:
	AABB m_bcBoundingCube;

protected:
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
	//정점 데이터를 저장하기 위한 정점 버퍼 인터페이스 포인터를 선언한다.
	ID3D11Buffer *m_pd3dVertexBuffer;

	//정점의 위치 벡터와 색상을 저장하기 위한 버퍼에 대한 인터페이스 포인터이다. 
	ID3D11Buffer *m_pd3dPositionBuffer;
	ID3D11Buffer *m_pd3dColorBuffer;

	//버퍼들을 입력조립기에 연결하기 위한 시작 슬롯 번호이다. 
	UINT m_nSlot;
	/*인스턴싱을 위한 정점 버퍼는 메쉬의 정점 데이터와 인스턴싱 데이터(객체의 위치와 방향)를 갖는다. 그러므로 인스턴싱을 위한 정점 버퍼는 하나가 아니라 버퍼들의 배열이다. 정점의 요소들을 나타내는 버퍼들을 입력조립기에 전달하기 위한 버퍼이다.*/
	ID3D11Buffer **m_ppd3dVertexBuffers;
	//정점을 조립하기 위해 필요한 버퍼의 개수이다. 
	int m_nBuffers;

	//정점의 개수이다. 
	int m_nVertices;
	UINT m_nStartVertex;
	//정점의 요소들을 나타내는 버퍼들의 원소의 바이트 수를 나타내는 배열이다. 
	UINT *m_pnVertexStrides;
	//정점의 요소들을 나타내는 버퍼들의 시작 위치(바이트 수)를 나타내는 배열이다. 
	UINT *m_pnVertexOffsets;

	/*정점 버퍼의 정점 개수, 정점의 바이트 수, 정점 데이터가
	정점 버퍼의 어디에서부터 시작하는 가를 나타내는 변수를 선언한다.*/
 	UINT m_nStride;
	UINT m_nOffset;

	//인덱스 버퍼(인덱스의 배열)에 대한 인터페이스 포인터이다. 
	ID3D11Buffer *m_pd3dIndexBuffer;
	//인덱스 버퍼가 포함하는 인덱스의 개수이다. 
	UINT m_nIndices;
	//인덱스 버퍼에서 메쉬를 표현하기 위해 사용되는 시작 인덱스이다. 
	UINT m_nStartIndex;
	//각 인덱스에 더해질 인덱스이다. 
	int m_nBaseVertex;
	UINT m_nIndexOffset;
	//각 인덱스의 형식(DXGI_FORMAT_R32_UINT 또는 DXGI_FORMAT_R16_UINT)이다. 
	DXGI_FORMAT	m_dxgiIndexFormat;

	ID3D11RasterizerState *m_pd3dRasterizerState;

public:
	AABB GetBoundingCube() { return(m_bcBoundingCube); }

	//메쉬의 정점 버퍼들을 배열로 조립한다. 
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	//인스턴싱을 사용하여 렌더링한다. 
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);


public:
	/* 각 정점의 위치 벡터를 픽킹을 위하여 저장한다.(정점 버퍼를 DYNAMIC으로 생성하고 Map()을 하지 않아도 되도록)*/
	D3DXVECTOR3 *m_pd3dxvPositions;
	/* 메쉬의 인덱스를 저장한다. (인덱스 버퍼를 DYNAMIC으로 생성하고 MAP()을 하지 않아도 되도록)*/
	UINT *m_pnIndices;

public:
	int CheckRayIntersection(D3DXVECTOR3 *pd3dxvRayPosition, D3DXVECTOR3 *pd3dxvRayDirection, MESHINTERSECTINFO *pd3dxIntersectInfo);
};

class CTriangleMesh : public CMesh
{
public:
	CTriangleMesh(ID3D11Device *pd3dDevice);
	virtual ~CTriangleMesh();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
};

class CDiffusedVertex
{
	D3DXVECTOR3 m_d3dxvPosition;
	//정점의 색상을 나타내는 멤버 변수(D3DXCOLOR 구조체)를 선언한다. 
	D3DXCOLOR m_d3dxcDiffuse;
public:
	//생성자와 소멸자를 선언한다.
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = d3dxvPosition; m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	~CDiffusedVertex() { }
};

class CCubeMesh : public CMesh
{
public:
	//직육면체의 가로, 세로, 높이의 크기를 지정하여 직육면체 메쉬를 생성한다.
	CCubeMesh(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f)); 
	virtual ~CCubeMesh();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);

};

class CAirplaneMesh : public CMesh
{
public:
	CAirplaneMesh(ID3D11Device *pd3dDevice, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMesh();
};

class CSphereMesh : public CMesh
{
public:
	CSphereMesh(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CSphereMesh();

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
};


/*  높이 맵을 사용하여 작은 지형을 표현할 수 있는 격자 메쉬를 정의한다.
CHeightMapGridMesh 클래스는 높이 맵을 사용하는 하나의 격자 메쉬를 표현한다.
이 격자는 좌표계의 xz-평면 위에 있다고 가정한다. 그리고 격자의 간격은 1이 되도록 만들 것이다.
(실제 지형에서 격자의 간격은 스케일 벡터에 따라 달라질 것이다.) 그러면 격자의 각 교차점은 메쉬의 정점이 될 것이고,
x-좌표와 z-좌표는 좌하단을 원점(xStart, 0, zStart)을 기준으로 오른쪽, 위쪽으로 값이 증가해 나간다.
이제 이 격자의 각 교차점의 y-좌표(높이)는 교차점의 x-좌표가 a, z-좌표가 b일때 높이 맵(이미지)에서 (a, b)좌표의 픽셀 값을 사용한다.*/

class CHeightMapGridMesh : public CMesh
{
protected:
	// 격자의 크기(가로 x 방향, 세로 z-방향)이다.
	int m_nWidth;
	int m_nLength;

	/* 격자의 스케일(가로: x-방향, 세로 z-방향, 높이 y-방향) 벡터이다. 실제 격자 메쉬의 각 정점의 x-좌표, y-좌표, z-좌표는 스케일 벡터의
	x-좌표, y-좌표, z-좌표로 곱한 값을 가진다. 즉, 실제 격자의 x-축 방향의 간격은 1이 아니라 스케일 벡터의 x좌표가 된다.
	이렇게 하면 작은 격자를 사용하더라고 큰 격자를 생성할 수 있다.*/
	D3DXVECTOR3 m_d3dxvScale;

public:
	CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f), void* pContext = NULL);
	~CHeightMapGridMesh();

	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return (m_nLength); }

	// 격자의 교점(정점)의 높이를 설정한다.
	virtual float OnGetHeight(int x, int z, void* pContext);
	//격자의 교점(정점)의 색상을 설정한다.
	virtual D3DXCOLOR OnGetColor(int x, int z, void*pContext);
};