#pragma once
const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer
{
public:
	CGameTimer();
	~CGameTimer();

	// Ÿ�̸��� �ð��� �����Ѵ�.
	void Tick(float flockFPS = 0.0f);

	// ������ ����Ʈ�� ��ȯ�Ѵ�.
	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	
	// �������� ��� ��� �ð��� ��ȯ�Ѵ�.
	float GetTimeElapsed();

private:
	// ��ǻ�Ͱ� performance counter�� ������ �ִ°�
	bool m_bHardwareHasPerformanceCounter;
	
	// scale counter�� ��
	float m_fTimeScale;
	
	// ������ ������ ���� ������ �ð�
	float m_fTimeElapsed;
	
	// ������ �ð�
	__int64 m_nCurrentTime;

	// ������ �������� �ð�
	__int64 m_nLastTime;

	// ��ǻ���� performance frequency
	__int64 m_PerformanceFrequency;

	// ������ �ð��� �����ϱ� ���� �迭
	float m_fFrameTime[MAX_SAMPLE_COUNT];

	// ������ ������ Ƚ��
	ULONG m_nSampleCount;

	// ������ ������ ����Ʈ
	unsigned long m_nCurrentFrameRate;

	// �ʴ� ������ ��
	unsigned long m_FramePerSecond;

	// ������ ����Ʈ ��� �ҿ� �ð�
	float m_fFPSTimeElapsed;
};

