// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include <d3d11.h>
#include <d3dx11.h>
#include <mmsystem.h>
#include <math.h>

#include <d3dcompiler.h>  	//���̴� ������ �Լ��� ����ϱ� ���� ��� ����
#include <D3DX10Math.h>	//Direct3D ���� �Լ��� ����ϱ� ���� ��� ����
#include <d3dx11.h> 
#include <D3D9Types.h>

#define FRAME_BUFFER_WIDTH 640
#define FRAME_BUFFER_HEIGHT 480

#define VS_SLOT_CAMERA				0x00
#define VS_SLOT_WORLD_MATRIX			0x01