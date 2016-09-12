// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <d3d11.h>
#include <d3dx11.h>
#include <mmsystem.h>
#include <math.h>

#include <d3dcompiler.h>  	//쉐이더 컴파일 함수를 사용하기 위한 헤더 파일
#include <D3DX10Math.h>	//Direct3D 수학 함수를 사용하기 위한 헤더 파일
#include <d3dx11.h> 
#include <D3D9Types.h>

#define FRAME_BUFFER_WIDTH 640
#define FRAME_BUFFER_HEIGHT 480

#define VS_SLOT_CAMERA				0x00
#define VS_SLOT_WORLD_MATRIX			0x01