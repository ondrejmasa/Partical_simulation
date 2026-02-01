# pragma once
# include <SDL3/SDL.h>

class FpsCounter {
private:
	Uint64 mLastTime;
	int mFrameCount = 0;
	float mElapsedTime = 0.f;
	float mDeltaTime = 0.f;
	float mFps = 0.f;
public:
	FpsCounter();
	bool Update();
	const float GetFps() const;
	const float GetDeltaTime() const;
};