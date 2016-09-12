#pragma once
const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer
{
public:
	CGameTimer();
	~CGameTimer();

	// 타이머의 시간을 갱신한다.
	void Tick(float flockFPS = 0.0f);

	// 프레임 레이트를 반환한다.
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	
	// 프레임의 평균 경과 시간을 반환한다.
	float GetTimeElapsed();

private:
	// 컴퓨터가 performance counter를 가지고 있는가
	bool m_bHardwareHasPerformanceCounter;
	
	// scale counter의 양
	float m_fTimeScale;
	
	// 마지막 프레임 이후 지나간 시간
	float m_fTimeElapsed;
	
	// 현재의 시간
	__int64 m_nCurrentTime;

	// 마지막 프레임의 시간
	__int64 m_nLastTime;

	// 컴퓨터의 performance frequency
	__int64 m_PerformanceFrequency;

	// 프레임 시간을 누적하기 위한 배열
	float m_fFrameTime[MAX_SAMPLE_COUNT];

	// 누적된 프레임 횟수
	ULONG m_nSampleCount;

	// 현재의 프레임 레이트
	unsigned long m_nCurrentFrameRate;

	// 초당 프레임 수
	unsigned long m_FramePerSecond;

	// 프레임 레이트 계산 소요 시간
	float m_fFPSTimeElapsed;
};

