////////////////////////////////
// 
// GameTimer.cpp
// 
// 役割：時間
// 
// 作成開始日：2025/12/29
// 
// 作成日：2025/12/28
// 　　作業内容：#1
// 　　　　　追加：
// 
// 
// 
// 
// 
////////////////////////////////

////////////////////----- 単語帳 -----////////////////////////////////////////////////////////////////////////////////
//
// 
// 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Core/GameTimer.h"

GameTimer::GameTimer()
	:mSecondsPerCount(0.0)
	,mDeltaTime(-1.0)
	,mBaseTime(0)
	,mPausedTime(0)
	,mPrevTime(0)
	,mCurrTime(0)
	,mStopTime(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mStopped = false;
}

void GameTimer::Tick()
{
	if (mStopped)
	{
		mDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// 今回の時間 - 前回の時間 = 経過時間(カウント数)
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;

	mPrevTime = mCurrTime;

	// 負の値にならないように補正
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}


float GameTimer::DeltaTime() const
{
	return (float)mDeltaTime;
}
