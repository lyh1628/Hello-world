#include "Shader.h"

#pragma once

class CScene
{
public:
	CHeightMapTerrain *GetTerrain();
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
	int m_nShaders; 
	CShader **m_ppShaders;	// private���� public����..?
	// ���� ���� ���� �ٸ� ���̴� ��ü���� �����Ѵ�.
public:
	CGameObject *PickObjectPointedByCursor(int xClient, int yClient);
	void SetCamera(CCamera *pCamera) { m_pCamera = pCamera; }

private:
	CCamera *m_pCamera;
	CGameObject *m_pSelectedObject;

};

