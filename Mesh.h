#pragma once

/*������ ������ ��������(Random) �����ϱ� ���� ����Ѵ�.
�� ������ ������ ����(Random Number)�� �����Ͽ� �����Ѵ�.*/
#define RANDOM_COLOR D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

struct MESHINTERSECTINFO {
	DWORD m_dwFaceIndex;
	float m_fU;
	float m_fV;
	float m_fDistance;
};

class CVertex
{
	//������ ��ġ ����(3���� ����)�� �����ϱ� ���� ��� ������ �����Ѵ�. 
	D3DXVECTOR3 m_d3dxvPosition;
public:
	//CVertex Ŭ������ �����ڿ� �Ҹ��ڸ� ������ ���� �����Ѵ�. 
	CVertex() { m_d3dxvPosition = D3DXVECTOR3(0, 0, 0); }
	CVertex(D3DXVECTOR3 d3dxvPosition) { m_d3dxvPosition = d3dxvPosition; }
	~CVertex() { }
};

class AABB
{
public:
	//�ٿ�� �ڽ��� �ּ����� �ִ����� ��Ÿ���� �����̴�.
	D3DXVECTOR3 m_d3dxvMinimum;
	D3DXVECTOR3 m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); }
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	//�� ���� �ٿ�� �ڽ��� ���Ѵ�.
	void Union(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Union(AABB *pAABB);
	//�ٿ�� �ڽ��� 8���� �������� ��ķ� ��ȯ�ϰ� �ּ����� �ִ����� �ٽ� ����Ѵ�.
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
	//Mesh.cpp ���Ͽ��� CMesh::AddRef(), CMesh::Release() �Լ��� ���Ǹ� �����Ѵ�. 

protected:
	AABB m_bcBoundingCube;

protected:
	D3D11_PRIMITIVE_TOPOLOGY m_d3dPrimitiveTopology;
	//���� �����͸� �����ϱ� ���� ���� ���� �������̽� �����͸� �����Ѵ�.
	ID3D11Buffer *m_pd3dVertexBuffer;

	//������ ��ġ ���Ϳ� ������ �����ϱ� ���� ���ۿ� ���� �������̽� �������̴�. 
	ID3D11Buffer *m_pd3dPositionBuffer;
	ID3D11Buffer *m_pd3dColorBuffer;

	//���۵��� �Է������⿡ �����ϱ� ���� ���� ���� ��ȣ�̴�. 
	UINT m_nSlot;
	/*�ν��Ͻ��� ���� ���� ���۴� �޽��� ���� �����Ϳ� �ν��Ͻ� ������(��ü�� ��ġ�� ����)�� ���´�. �׷��Ƿ� �ν��Ͻ��� ���� ���� ���۴� �ϳ��� �ƴ϶� ���۵��� �迭�̴�. ������ ��ҵ��� ��Ÿ���� ���۵��� �Է������⿡ �����ϱ� ���� �����̴�.*/
	ID3D11Buffer **m_ppd3dVertexBuffers;
	//������ �����ϱ� ���� �ʿ��� ������ �����̴�. 
	int m_nBuffers;

	//������ �����̴�. 
	int m_nVertices;
	UINT m_nStartVertex;
	//������ ��ҵ��� ��Ÿ���� ���۵��� ������ ����Ʈ ���� ��Ÿ���� �迭�̴�. 
	UINT *m_pnVertexStrides;
	//������ ��ҵ��� ��Ÿ���� ���۵��� ���� ��ġ(����Ʈ ��)�� ��Ÿ���� �迭�̴�. 
	UINT *m_pnVertexOffsets;

	/*���� ������ ���� ����, ������ ����Ʈ ��, ���� �����Ͱ�
	���� ������ ��𿡼����� �����ϴ� ���� ��Ÿ���� ������ �����Ѵ�.*/
 	UINT m_nStride;
	UINT m_nOffset;

	//�ε��� ����(�ε����� �迭)�� ���� �������̽� �������̴�. 
	ID3D11Buffer *m_pd3dIndexBuffer;
	//�ε��� ���۰� �����ϴ� �ε����� �����̴�. 
	UINT m_nIndices;
	//�ε��� ���ۿ��� �޽��� ǥ���ϱ� ���� ���Ǵ� ���� �ε����̴�. 
	UINT m_nStartIndex;
	//�� �ε����� ������ �ε����̴�. 
	int m_nBaseVertex;
	UINT m_nIndexOffset;
	//�� �ε����� ����(DXGI_FORMAT_R32_UINT �Ǵ� DXGI_FORMAT_R16_UINT)�̴�. 
	DXGI_FORMAT	m_dxgiIndexFormat;

	ID3D11RasterizerState *m_pd3dRasterizerState;

public:
	AABB GetBoundingCube() { return(m_bcBoundingCube); }

	//�޽��� ���� ���۵��� �迭�� �����Ѵ�. 
	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	//�ν��Ͻ��� ����Ͽ� �������Ѵ�. 
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);


public:
	/* �� ������ ��ġ ���͸� ��ŷ�� ���Ͽ� �����Ѵ�.(���� ���۸� DYNAMIC���� �����ϰ� Map()�� ���� �ʾƵ� �ǵ���)*/
	D3DXVECTOR3 *m_pd3dxvPositions;
	/* �޽��� �ε����� �����Ѵ�. (�ε��� ���۸� DYNAMIC���� �����ϰ� MAP()�� ���� �ʾƵ� �ǵ���)*/
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
	//������ ������ ��Ÿ���� ��� ����(D3DXCOLOR ����ü)�� �����Ѵ�. 
	D3DXCOLOR m_d3dxcDiffuse;
public:
	//�����ڿ� �Ҹ��ڸ� �����Ѵ�.
	CDiffusedVertex(float x, float y, float z, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = D3DXVECTOR3(x, y, z); m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex(D3DXVECTOR3 d3dxvPosition, D3DXCOLOR d3dxcDiffuse) { m_d3dxvPosition = d3dxvPosition; m_d3dxcDiffuse = d3dxcDiffuse; }
	CDiffusedVertex() { m_d3dxvPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f); m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); }
	~CDiffusedVertex() { }
};

class CCubeMesh : public CMesh
{
public:
	//������ü�� ����, ����, ������ ũ�⸦ �����Ͽ� ������ü �޽��� �����Ѵ�.
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


/*  ���� ���� ����Ͽ� ���� ������ ǥ���� �� �ִ� ���� �޽��� �����Ѵ�.
CHeightMapGridMesh Ŭ������ ���� ���� ����ϴ� �ϳ��� ���� �޽��� ǥ���Ѵ�.
�� ���ڴ� ��ǥ���� xz-��� ���� �ִٰ� �����Ѵ�. �׸��� ������ ������ 1�� �ǵ��� ���� ���̴�.
(���� �������� ������ ������ ������ ���Ϳ� ���� �޶��� ���̴�.) �׷��� ������ �� �������� �޽��� ������ �� ���̰�,
x-��ǥ�� z-��ǥ�� ���ϴ��� ����(xStart, 0, zStart)�� �������� ������, �������� ���� ������ ������.
���� �� ������ �� �������� y-��ǥ(����)�� �������� x-��ǥ�� a, z-��ǥ�� b�϶� ���� ��(�̹���)���� (a, b)��ǥ�� �ȼ� ���� ����Ѵ�.*/

class CHeightMapGridMesh : public CMesh
{
protected:
	// ������ ũ��(���� x ����, ���� z-����)�̴�.
	int m_nWidth;
	int m_nLength;

	/* ������ ������(����: x-����, ���� z-����, ���� y-����) �����̴�. ���� ���� �޽��� �� ������ x-��ǥ, y-��ǥ, z-��ǥ�� ������ ������
	x-��ǥ, y-��ǥ, z-��ǥ�� ���� ���� ������. ��, ���� ������ x-�� ������ ������ 1�� �ƴ϶� ������ ������ x��ǥ�� �ȴ�.
	�̷��� �ϸ� ���� ���ڸ� ����ϴ���� ū ���ڸ� ������ �� �ִ�.*/
	D3DXVECTOR3 m_d3dxvScale;

public:
	CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f), void* pContext = NULL);
	~CHeightMapGridMesh();

	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return (m_nLength); }

	// ������ ����(����)�� ���̸� �����Ѵ�.
	virtual float OnGetHeight(int x, int z, void* pContext);
	//������ ����(����)�� ������ �����Ѵ�.
	virtual D3DXCOLOR OnGetColor(int x, int z, void*pContext);
};