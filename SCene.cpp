#include "stdafx.h"
#include "SCene.h"

CScene::CScene()
{
	m_ppShaders = NULL;
	m_nShaders = 0;

	m_pCamera = NULL;
	m_pSelectedObject = NULL;

}

CScene::~CScene()
{
}
void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	m_nShaders = 2;
	m_ppShaders = new CShader*[m_nShaders];

	m_ppShaders[0] = new CTerrainShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);

	CInstancingShader *pInstancingShader = new CInstancingShader();
	pInstancingShader->CreateShader(pd3dDevice);
	pInstancingShader->BuildObjects(pd3dDevice, GetTerrain());
	m_ppShaders[1] = pInstancingShader;
}

void CScene::ReleaseObjects()
{
	for (int j = 0; j < m_nShaders; j++)
	{
		m_ppShaders[j]->ReleaseObjects();
		delete m_ppShaders[j];
	}
	if (m_ppShaders) delete[] m_ppShaders;
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		/*윈도우의 클라이언트 영역에서 왼쪽 마우스 버튼이 눌려지면 마우스의 위치를 사용하여 픽킹을 처리한다. 마우스 픽킹으로 선택된 객체가 있으면 그 객체를 비활성화한다.*/
		m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam)); 
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
		m_ppShaders[i]->AnimateObjects(fTimeElapsed);
	}
}

void CScene::Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera)
{
	for (int i = 0; i < m_nShaders; i++)
	{
		m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);
	}
}

CGameObject *CScene::PickObjectPointedByCursor(int xClient, int yClient)
{
	if (!m_pCamera) return(NULL);

	D3DXMATRIX d3dxmtxView = m_pCamera->GetViewMatrix();
	D3DXMATRIX d3dxmtxProjection = m_pCamera->GetProjectionMatrix();
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	D3DXVECTOR3 d3dxvPickPosition;
	/*화면 좌표계의 점 (xClient, yClient)를 화면 좌표 변환의 역변환과 투영 변환의 역변환을 한다. 
	그 결과는 카메라 좌표계의 점이다. 투영 평면이 카메라에서 z-축으로 거리가 1이므로 z-좌표는 1로 설정한다.*/
	d3dxvPickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / d3dxmtxProjection._11;
	d3dxvPickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / d3dxmtxProjection._22;
	d3dxvPickPosition.z = 1.0f;

	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	MESHINTERSECTINFO d3dxIntersectInfo;
	CGameObject *pIntersectedObject = NULL, *pNearestObject = NULL;
	//씬의 모든 쉐이더 객체에 대하여 픽킹을 처리하여 카메라와 가장 가까운 픽킹된 객체를 찾는다.
	for (int i = 0; i < m_nShaders; i++)
	{
		pIntersectedObject = m_ppShaders[i]->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = pIntersectedObject;
		}
	}
	return(pNearestObject);
}
CHeightMapTerrain *CScene::GetTerrain()
{
	CTerrainShader *pSceneShader = (CTerrainShader *)m_ppShaders[0];
	return(pSceneShader->GetTerrain());
}
