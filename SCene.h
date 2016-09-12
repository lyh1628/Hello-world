#include "Shader.h"

#pragma once

class CScene
{

public:
	CScene();
	~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void BuildObjects(ID3D11Device *pd3dDevice);
	void ReleaseObjects();

	bool ProcessInput();
	void AnimateObjects(float fTimeElapsed); void Render(ID3D11DeviceContext*pd3dDeviceContext, CCamera *pCamera);
 
private:
	//씬은 쉐이더들의 리스트(배열)이다.
	CShader *m_pShaders;
	int m_nShaders;

	CCamera* m_pCamera;
	CGameObject* m_pSelectedObject;

public:
	CGameObject* PickObjectPointedByCursor(int xClient, int yClinet);
	void SetCamera(CCamera* pCamera) { m_pCamera = pCamera; }

	CHeightMapTerrain *GetTerrain();
};

