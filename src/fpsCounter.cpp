#include "fpsCounter.h"


FpsCounter::FpsCounter()
	: mLastTime{ SDL_GetTicks() }
{
}

bool FpsCounter::Update()
{
	Uint64 currentTime = SDL_GetTicks();
	mDeltaTime = (currentTime - mLastTime) / 1000.f;
	mLastTime = currentTime;
	mElapsedTime += mDeltaTime;
	mFrameCount++;
	if (mElapsedTime >= 1.f) {
		mFps = mFrameCount / mElapsedTime;
		mElapsedTime = 0.f;
		mFrameCount = 0;
		return true;
	}
	return false;
}

const float FpsCounter::GetFps() const
{
	return mFps;
}

const float FpsCounter::GetDeltaTime() const
{
	return mDeltaTime;
}
