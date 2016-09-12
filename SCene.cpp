#include "stdafx.h"
#include "SCene.h"

CScene::CScene()
{
	m_pShaders = NULL;
	m_nShaders = 0;

	m_pSelectedObject = NULL;

}

CScene::~CScene()
{
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nShaders = 1;
	m_pShaders = new CTerrainShader[m_nShaders];
	m_pShaders[0].CreateShader(pd3dDevice);
	m_pShaders[0].BuildObjects(pd3dDevice);
}
void CScene::ReleaseObjects()
{
	for (int j = 0; j < m_nShaders; j++) m_pShaders[j].ReleaseObjects();
	if (m_pShaders) delete[] m_pShaders;
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		/* 윈도우의 클라이언트 영역에서 왼쪽 마우스 버튼이 눌려지면 마우스의 위치를 사용하여 픽킹을 처리한다.*/
		if (m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam)))
		{
			m_pSelectedObject->SetActive(false); 
		}
		break;
	}
	return (false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWNd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return (false);
}

bool CScene::ProcessInput()
{
	return(false);
}
void CScene::AnimateObjects(float fTimeElapsed)
{
	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].AnimateObjects(fTimeElapsed);
	}
}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera)
{
	for (int i = 0; i < m_nShaders; i++)
	{
		m_pShaders[i].Render(pd3dDeviceContext, pCamera);
	}
}

CGameObject* CScene::PickObjectPointedByCursor(int xClinet, int yClient)
{
	if (!m_pCamera)
		return (NULL);

	D3DXMATRIX d3dxmtxView = m_pCamera->GetViewMatrix();
	D3DXMATRIX d3dxmtxProjection = m_pCamera->GetProjectionMatrix();
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	D3DXVECTOR3 d3dxvPickPosition;
	/* 화면 좌표계의 점 (xClient, yClient)를 화면 좌표 변환의 역변환과 투영 변환의 역변환을 한다.
	그 결과는 카메라 좌표계의 점이다. 투영 평면이 카메라에서 z-축으로 거리가 1이므로 z-좌표는 1로 설정한다.
	*/
	d3dxvPickPosition.x = (((2.0f * xClinet) / d3dViewport.Width) - 1) / d3dxmtxProjection._11;
	d3dxvPickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / d3dxmtxProjection._22;
	d3dxvPickPosition.z = 1.0f;

	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	MESHINTERSECTINFO d3dxIntersectinfo;
	CGameObject* pIntersectedObject = NULL, *pNearestObject = NULL;

	// 씬의 모든 쉐이더 객체에 대하여 픽킹을 처리하여 카메라와 가장 가까운 픽킹된 객체를 찾는다.
	for (int i = 0; i < m_nShaders; i++)
	{
		pIntersectedObject = m_pShaders[i].PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectinfo);

		if (pIntersectedObject && (d3dxIntersectinfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectinfo.m_fDistance;
			pNearestObject = pIntersectedObject;
		}
	}
	
	return(pNearestObject);
}
CHeightMapTerrain *CScene::GetTerrain()
{
	CTerrainShader *pSceneShader = (CTerrainShader*)&m_pShaders[0];
	return(pSceneShader->GetTerrain());
}

