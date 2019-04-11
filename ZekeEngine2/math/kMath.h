#pragma once

class CMath {
public:
	static const float PI;

	static float DegToRad(float deg)
	{
		return deg * (PI / 180.0f);
	}
	static float RadToDeg(float rad)
	{
		return rad / (PI / 180.0f);
	}

	static inline float Lerp(float rate, float t0, float t1)
	{
		return t0 + (t1 - t0)*rate;
	}
};
