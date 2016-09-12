#include "stdafx.h"
#include "Object.h"
#include "Shader.h"

CGameObject::CGameObject(int nMeshes)
{
	D3DXMatrixIdentity(&m_d3dxmtxWorld);

	m_nReferences = 0;
	m_bActive = true;

	m_nMeshes = nMeshes;

	m_ppMeshes = NULL;
	if (m_nMeshes > 0)
		m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++)
		m_ppMeshes[i] = NULL;

	m_bcMeshBoundingCube = AABB();
}
CGameObject::~CGameObject()
{
	if (m_ppMeshes)
	{
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i])
				m_ppMeshes[i]->Release();
			m_ppMeshes[i] = NULL;
		}
		delete[] m_ppMeshes;
	}
}
void CGameObject::AddRef()
{
	m_nReferences++;
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_d3dxmtxWorld._41 = x;
	m_d3dxmtxWorld._42 = y;
	m_d3dxmtxWorld._43 = z;
}

void CGameObject::SetPosition(D3DXVECTOR3 d3dxvPosition)
{
	m_d3dxmtxWorld._41 = d3dxvPosition.x;
	m_d3dxmtxWorld._42 = d3dxvPosition.y;
	m_d3dxmtxWorld._43 = d3dxvPosition.z;
}

D3DXVECTOR3 CGameObject::GetPosition()
{
	return(D3DXVECTOR3(m_d3dxmtxWorld._41, m_d3dxmtxWorld._42, m_d3dxmtxWorld._43));
}

void CGameObject::Release()
{
	if (m_nReferences > 0) m_nReferences--;
	if (m_nReferences <= 0) delete this;
}
void CGameObject::SetMesh(CMesh *pMesh, int nIndex)
{
	if (m_ppMeshes)
	{
		if (m_ppMeshes[nIndex]) m_ppMeshes[nIndex]->Release();
		m_ppMeshes[nIndex] = pMesh;
		if (pMesh) pMesh->AddRef();
	}

	if (pMesh)
	{
		AABB bcBoundingCube = pMesh->GetBoundingCube();
		m_bcMeshBoundingCube.Union(&bcBoundingCube);
	}
}

void CGameObject::Animate(float fTimeElapsed)
{
}

void CGameObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CShader::UpdateShaderVariables(pd3dDeviceContext, &m_d3dxmtxWorld);
	if (m_ppMeshes)
		for (int i = 0; i < m_nMeshes; i++)
		{
			if (m_ppMeshes[i])
				m_ppMeshes[i]->Render(pd3dDeviceContext);
		}
}

bool CGameObject::IsVisible(CCamera* pCamera)
{
	OnPrepareRender();

	bool bIsVisible = false;

	if (m_bActive)
	{
		AABB bcBoundingCube;
		bcBoundingCube.Update(&m_d3dxmtxWorld);
		if (pCamera) bIsVisible = pCamera->IsInFrustum(&bcBoundingCube);
	}
	return(bIsVisible);
}

D3DXVECTOR3 CGameObject::GetLookAt()
{
	//���� ��ü�� ���� z-�� ���͸� ��ȯ�Ѵ�.
	D3DXVECTOR3 d3dxvLookAt(m_d3dxmtxWorld._31, m_d3dxmtxWorld._32, m_d3dxmtxWorld._33);
	D3DXVec3Normalize(&d3dxvLookAt, &d3dxvLookAt);
	return(d3dxvLookAt);
}

D3DXVECTOR3 CGameObject::GetUp()
{
	//���� ��ü�� ���� y-�� ���͸� ��ȯ�Ѵ�.
	D3DXVECTOR3 d3dxvUp(m_d3dxmtxWorld._21, m_d3dxmtxWorld._22, m_d3dxmtxWorld._23);
	D3DXVec3Normalize(&d3dxvUp, &d3dxvUp);
	return(d3dxvUp);
}

D3DXVECTOR3 CGameObject::GetRight()
{
	//���� ��ü�� ���� x-�� ���͸� ��ȯ�Ѵ�.
	D3DXVECTOR3 d3dxvRight(m_d3dxmtxWorld._11, m_d3dxmtxWorld._12, m_d3dxmtxWorld._13);
	D3DXVec3Normalize(&d3dxvRight, &d3dxvRight);
	return(d3dxvRight);
}

void CGameObject::MoveStrafe(float fDistance)
{
	//���� ��ü�� ���� x-�� �������� �̵��Ѵ�.
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvRight = GetRight();
	d3dxvPosition += fDistance * d3dxvRight;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveUp(float fDistance)
{
	//���� ��ü�� ���� y-�� �������� �̵��Ѵ�.
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvUp = GetUp();
	d3dxvPosition += fDistance * d3dxvUp;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::MoveForward(float fDistance)
{
	//���� ��ü�� ���� z-�� �������� �̵��Ѵ�.
	D3DXVECTOR3 d3dxvPosition = GetPosition();
	D3DXVECTOR3 d3dxvLookAt = GetLookAt();
	d3dxvPosition += fDistance * d3dxvLookAt;
	CGameObject::SetPosition(d3dxvPosition);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	//���� ��ü�� �־��� ������ ȸ���Ѵ�.
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationYawPitchRoll(&mtxRotate, (float)D3DXToRadian(fYaw), (float)D3DXToRadian(fPitch), (float)D3DXToRadian(fRoll));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CGameObject::Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle)
{
	//���� ��ü�� �־��� ȸ������ �߽����� ȸ���Ѵ�.
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, pd3dxvAxis, (float)D3DXToRadian(fAngle));
	m_d3dxmtxWorld = mtxRotate * m_d3dxmtxWorld;
}

void CGameObject::GenerateRayForPicking(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxWorld, D3DXMATRIX *pd3dxmtxView, D3DXVECTOR3 *pd3dxvPickRayPosition, D3DXVECTOR3 *pd3dxvPickRayDirection)
{
	//	pd3dxvPickPosition: ī�޶� ��ǥ���� �� (ȭ�� ��ǥ�迡�� ���콺�� Ŭ���� ���� ����ȯ�� ��)
	//	pd3dxmtxWorld : ���� ��ȯ ���, pd3dxmtxView: ī�޶� ��ȯ ���
	//	pd3dxvPickRayPosition: ��ŷ ������ ������. pd3dxvPickRayDirection: ��ŷ ���� ����

	/* 1)��ü�� ���� ��ȯ ����� �־����� ��ü�� ���� ��Ȳ ��İ� ī�޶� ��ȯ ����� ���ϰ�, ������� ���Ѵ�.
	('�̰�'�� ī�޶� ��Ȳ ����� ����İ� ��ü�� ���� ��ȯ ����� ������� ���� ����.)

	��ü�� ���� ��ȯ ����� �־����� ������ ī�޶� ��ȯ ����� ������� ���Ѵ�.
	��ü�� ���� ��ȯ ����� �־����� �� ��ǥ���� ��ŷ ������ ���ϰ�, �׷��� ������ ���� ��ǥ���� ��ŷ ������ ���Ѵ�. */

	D3DXMATRIX d3dxmtxInverse;
	D3DXMATRIX d3dxmtxWorldView = *pd3dxmtxView;

	if (pd3dxmtxWorld) D3DXMatrixMultiply(&d3dxmtxWorldView, pd3dxmtxWorld, pd3dxmtxView);
	D3DXMatrixInverse(&d3dxmtxInverse, NULL, &d3dxmtxWorldView);

	/* ī�޶� ��ǥ���� ���� (0, 0, 0)�� ������ ����  ����ķ� ��ȯ�Ѵ�.
	��ȯ�� ����� 'ī�޶� ��ǥ���� ����'�� �����ϴ� �� ��ǥ���� �� �Ǵ� ���� ��ǥ���� ���̴�.*/
	D3DXVECTOR3 d3dxvCameraOrigin(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(pd3dxvPickRayPosition, &d3dxvCameraOrigin, &d3dxmtxInverse);

	/* ī�޶� ��ǥ���� ���� ������ ���� ����ķ� ��ȯ�Ѵ�.
	��ȯ�� ����� '���콺�� Ŭ���� ��'�� �����Ǵ� �� ��ǥ���� �� �Ǵ� ���� ��ǥ���� ���̴�. */
	D3DXVec3TransformCoord(pd3dxvPickRayDirection, pd3dxvPickPosition, &d3dxmtxInverse);

	// ��ŷ ������ ���� ���͸� ���Ѵ�.
	*pd3dxvPickRayDirection = *pd3dxvPickRayDirection - *pd3dxvPickRayPosition;
}


int CGameObject::PickObjectByRayIntersection(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxView, MESHINTERSECTINFO* pd3dxIntersectInfo)
{
	// pd3dxvPickPosition : ī�޶� ��ǥ���� ��(ȭ�� ��ǥ�迡�� ���콺�� Ŭ���� ���� ����ȯ�� ��)
	// pd3dxmtxView : ī�޶� ��ȯ ���
	D3DXVECTOR3 d3dxvPickRayPosition, d3dxvPickRayDirection;
	int nIntersected = 0;

	// Ȱ��ȭ�� ��ü�� ���Ͽ� �޽��� ������ ��ŷ ������ ���ϰ� ��ü�� �޽��� �浹 �˻縦 �Ѵ�.
	if (m_bActive && m_ppMeshes)
	{
		// ��ü�� �� ��ǥ���� ��ŷ ������ ���Ѵ�.
		GenerateRayForPicking(pd3dxvPickPosition, &m_d3dxmtxWorld, pd3dxmtxView, &d3dxvPickRayPosition, &d3dxvPickRayDirection);

		/* �� ��ǥ���� ��ŷ ������ �޽��� �浹�� �˻��Ѵ�. ��ŷ ������ �޽��� �ﰢ������ ���� �� �浹�� �� �ִ�.
		�˻��� ����� �浹�� Ƚ���̴�.*/
		for (int i = 0; i < m_nMeshes; i++)
		{
			nIntersected = m_ppMeshes[i]->CheckRayIntersection(&d3dxvPickRayPosition, &d3dxvPickRayDirection, pd3dxIntersectInfo);
			if (nIntersected > 0) break;
		}
	}
	return (nIntersected);
}


#pragma region __CRotatingObject__
CRotatingObject::CRotatingObject(int nMeshes) : CGameObject(nMeshes)
{
	m_d3dxvRotationAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_fRotationSpeed = 15.0f;
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fTimeElapsed)
{
	CGameObject::Rotate(&m_d3dxvRotationAxis, m_fRotationSpeed * fTimeElapsed);

}

void CRotatingObject::Render(ID3D11DeviceContext *pd3dDeviceContext)
{
	CGameObject::Render(pd3dDeviceContext);
}
#pragma endregion


#pragma region __CRevolvingObject__
CRevolvingObject::CRevolvingObject(int nMeshes) : CGameObject(nMeshes)
{
	m_d3dxvRevolutionAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_fRevolutionSpeed = 1.0f;
}

CRevolvingObject::~CRevolvingObject()
{
}

void CRevolvingObject::Animate(float fTimeElapsed)
{
	//������ ��Ÿ���� ���� ȸ�� ����� �����ʿ� ���Ѵ�.
	D3DXMATRIX mtxRotate;
	D3DXMatrixRotationAxis(&mtxRotate, &m_d3dxvRevolutionAxis, (float)D3DXToRadian(m_fRevolutionSpeed * fTimeElapsed));
	m_d3dxmtxWorld = m_d3dxmtxWorld * mtxRotate;
}
#pragma endregion


#pragma region __CHeightMap__
CHeightMap::CHeightMap(LPCTSTR pFileName, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale)
{
	m_nWidth = nWidth;
	m_nLength = nLength;
	m_d3dxvScale = d3dxvScale;

	BYTE *pHeightMapImage = new BYTE[m_nWidth*m_nLength];

	// ������ ���� �д´�. ���� �� �̹����� ���� ����� ���� RAW �̹����̴�.
	HANDLE hFile = ::CreateFile(pFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY, NULL);

	DWORD dwBytesRead;
	::ReadFile(hFile, pHeightMapImage, (m_nWidth * m_nLength), &dwBytesRead, NULL);
	::CloseHandle(hFile);

	/* �̹����� y-��� ������ z-���� ������ �ݴ��̹Ƿ� �̹����� ���ϴ�Ī ���� �����Ѵ�.
	�׷��� �̹����� ��ǥ��� ������ ��ǥ���� ������ ��ġ�ϰ� �ȴ�.*/
	m_pHeightMapImage = new BYTE[m_nWidth * m_nLength];
	for (int y = 0; y < m_nLength; y++)
	{
		for (int x = 0; x < m_nWidth; x++)
		{
			m_pHeightMapImage[x + ((m_nLength - 1 - y) * m_nWidth)] = pHeightMapImage[x + (y*m_nWidth)];
		}
	}
	if (pHeightMapImage) delete[] pHeightMapImage;
}

CHeightMap::~CHeightMap()
{
	if (m_pHeightMapImage) delete[] m_pHeightMapImage;
	m_pHeightMapImage = NULL;
}

// �� �Լ��� ������ (x, z)��ǥ�� �־��� �� ���� �� (x, z)��ǥ���� ������ ���� ���͸� ����Ѵ�.
D3DXVECTOR3 CHeightMap::GetHeightMapNormal(int x, int z)
{
	// ������ x-��ǥ�� z-��ǥ�� ����(���� ��)�� ������ ����� ������ ���� ���ʹ� y-�� ���� �����̴�.
	if ((x < 0.0f) || (z < 0.0f) || (x >= m_nWidth) || (z >= m_nLength))
		return (D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	/* ���� �ʿ��� (x, z)��ǥ�� �ȼ� ���� ������ �� ���� ��(x+1, z), (z, z+1)�� ���� �ȼ� ���� ����Ͽ� ���� ���͸� ����Ѵ�.*/
	int nHeightMapIndex = x + (z*m_nWidth);
	int xHeightMapAdd = (x < (m_nWidth - 1)) ? 1 : -1;
	int zHeightMapAdd = (z < (m_nLength - 1)) ? m_nWidth : -(signed)m_nWidth;

	// (x, z), (x+1, z), (z, z+1)�� ������ ���� ���� ���Ѵ�.
	float y1 = (float)m_pHeightMapImage[nHeightMapIndex] * m_d3dxvScale.y;
	float y2 = (float)m_pHeightMapImage[nHeightMapIndex + xHeightMapAdd] * m_d3dxvScale.y;
	float y3 = (float)m_pHeightMapImage[nHeightMapIndex + zHeightMapAdd] * m_d3dxvScale.y;

	// vEdge1�� (0, y3, m_vScale.z) - (0, y1, 0) �����̴�.
	D3DXVECTOR3 vEdge1 = D3DXVECTOR3(0.0f, y3 - y1, m_d3dxvScale.z);
	// vEdge2�� (m_vScale.x, y2, 0) - (0, y1, 0) �����̴�.
	D3DXVECTOR3 vEdge2 = D3DXVECTOR3(m_d3dxvScale.x, y2 - y1, 0.0f);
	// ���� ���ʹ� vEdge1�� vEdge2�� ������ ����ȭ�ϸ� �ȴ�.
	D3DXVECTOR3 vNormal;
	D3DXVec3Cross(&vNormal, &vEdge1, &vEdge2);
	D3DXVec3Normalize(&vNormal, &vNormal);
	return(vNormal);
}

float CHeightMap::GetHeight(float fx, float fz, bool bReverseQuad)
{
	// ������ ��ǥ (fx, fz)���� ���� ���� ��ǥ�� ����Ѵ�.
	fx = fx / m_d3dxvScale.x;
	fz = fz / m_d3dxvScale.z;

	// ���� ���� x-��ǥ�� z-��ǥ�� ���� ���� ������ ����� ������ ���̴� 0�̴�.
	if ((fx < 0.0f) || (fz<0.0f) || (fx >-m_nWidth) || (fz >= m_nLength))
		return (0.0f);
	
	// ���� ���� ��ǥ�� ���� �κа� �Ҽ� �κ��� ����Ѵ�.
	int x = (int)fx;
	int z = (int)fz;
	float fxPercent = fx - x;
	float fzPercent = fz - z;

	float fTopLeft = m_pHeightMapImage[x + (z*m_nWidth)];
	float fTopRight = m_pHeightMapImage[(x + 1) + (z*m_nWidth)];
	float fBottomLeft = m_pHeightMapImage[x + ((z + 1) * m_nWidth)];
	float fBottomRight = m_pHeightMapImage[(x + 1) + ((z + 1) * m_nWidth)];

	if (bReverseQuad)
	{
		/* ������ �ﰢ������ �����ʿ��� ���� �������� �����Ǵ� ����̴�.
		fzPercent < fxPercent�� ���. TopLeft�� �ȼ� ���� (fTopLeft = fTopRight + (fBottomLeft - fBottomRight))�� �ٻ��Ѵ�.
		fzPercent > fxPercent�� ���, BottomRight�� �ȼ� ���� (fBottomRight = fBottomLeft +(fTopRight - fTopLeft))�� �ٻ��Ѵ�.*/
		if(fzPercent < (1.0f - fxPercent))
			fTopRight = fTopLeft + (fBottomRight - fBottomLeft);
		else
			fBottomLeft = fTopLeft + (fBottomRight - fTopRight);
	}
	else
	{
		/* ������ �ﰢ������ ���ʿ��� ������ �������� �����Ǵ� ����̴�.
		(fzPercent < (1.0f - fxPercent))�� ��� TopRight�� �ȼ� ���� (fTopRight = fTopLeft + (fBottomRight - fBottomLeft))�� �ٻ��Ѵ�.
		(fzPercent �� (1.0f - fxPercent))�� ��� BottomLeft�� �ȼ� ���� (fBottomLeft = fTopLeft + (fBottomRight - fTopRight))�� �ٻ��Ѵ�.*/ 
		if (fzPercent >= fxPercent)
			fBottomRight = fBottomLeft + (fTopRight - fTopLeft);
		else
			fTopLeft = fTopRight + (fBottomLeft - fBottomRight);
	}
	// �簢���� �� ���� �����Ͽ� ����( �ȼ� ��)�� ����Ѵ�.
	float fTopHeight = fTopLeft*(1 - fxPercent) + fTopRight * fxPercent;
	float fBottomHeight = fBottomLeft * (1 - fxPercent) + fBottomRight * fxPercent;
	 
	float fHeight = fBottomHeight*(1 - fzPercent) + fTopHeight*fzPercent;
	return fHeight;
}

CHeightMapTerrain::CHeightMapTerrain(ID3D11Device* pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale, D3DXCOLOR d3dxColor) : CGameObject(0)
{
	// ������ ����� ���� ���� ����, ������ ũ���̴�.
	m_nWidth = nWidth;
	m_nLength = nLength;

	/* ���� ��ü�� ���� �޽����� �迭�� ���� ���̴�. nBlockWidth, nBlockLength�� ���� �޽� �ϳ��� ���� ���� ũ���̴�.
	cwQuadsPerBlock, czQuadPerBlock�� ���� �޽��� ���� ����� ���� ���� �簢���� ���� �̴�.*/
	int cxQuadsPerBlock = nBlockWidth - 1;
	int czQuadsPerBlock = nBlockLength - 1;

	// d3dxvScale�� ������ ������ �� �� Ȯ���� ���ΰ��� ��Ÿ����.
	m_d3dxvScale = d3dxvScale;

	// ������ ����� ���� ���� �����Ѵ�.
	m_pHeightMap = new CHeightMap(pFileName, nWidth, nLength, d3dxvScale);

	// �������� ����, ���� �������� ���� �޽��� �� ���� �ִ� ���� ��Ÿ����.
	int cxBlocks = (m_nWidth - 1) / cxQuadsPerBlock;
	int czBlocks = (m_nLength - 1) / czQuadsPerBlock;

	// ���� ��ü�� ǥ���ϱ� ���� ���� �޽��� �����̴�.
	m_nMeshes = cxBlocks * czBlocks;

	// ���� ��ü�� ǥ���ϱ� ���� ���� �޽��� ���� ������ �迭�� �����Ѵ�.
	m_ppMeshes = new CMesh*[m_nMeshes];
	for (int i = 0; i < m_nMeshes; i++) m_ppMeshes[i] = NULL;

	CHeightMapGridMesh* pHeightMapGridMesh = NULL;
	for (int z = 0, zStart = 0; z < czBlocks; z++)
	{
		for (int x = 0, xStart = 0; x < cxBlocks; x++)
		{
			// ������ �Ϻκ��� ��Ÿ���� ���� �޽��� ���� ��ġ�̴�.
			xStart = x*(nBlockWidth - 1);
			zStart = z*(nBlockLength - 1);


			// ������ �Ϻκ��� ��Ÿ���� ���� �޽��� �����Ͽ� ���� �޽��� �����Ѵ�.
			pHeightMapGridMesh = new CHeightMapGridMesh(pd3dDevice, xStart, zStart, nBlockWidth, nBlockLength, d3dxvScale, d3dxColor, m_pHeightMap);
			SetMesh(pHeightMapGridMesh, x + (z*cxBlocks));
		}
	}
}

CHeightMapTerrain::~CHeightMapTerrain()
{
	if (m_pHeightMap)
		delete m_pHeightMap;
}