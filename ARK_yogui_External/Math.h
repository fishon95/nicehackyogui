#pragma once
#include "DirectXIncludes.h"
#include <Windows.h>
class Math
{
public:
	static inline float GetDistance(Vec3& src, Vec3& dst)
	{
		Vec3 Delta;
		D3DXVec3Subtract(&Delta, &src, &dst);

		return D3DXVec3Length(&Delta);
	}
	static inline Vec3 CalculateAngle(Vec3& src, Vec3& dst)
	{
		Vec3 angles;
		Vec3 delta = src - dst;
		angles.x = -(asinf(delta.z / D3DXVec3Length(&delta)) * 180.f / D3DX_PI);
		angles.y = (atanf(delta.y / delta.x) * 180.0f / D3DX_PI);
		angles.z = 0.0f;

		if (delta.x >= 0.0) { angles.y += 180.0f; }

		if (isnan(angles.x) || isnan(angles.y))
			return Vec3(0, 0, 0);

		Math::ClampAngle(angles);

		return angles;
	}
	static inline void ClampAngle(Vec3& Angle)
	{
		if (Angle.x < -87.0f)
			Angle.x = -87.0f;
		else if (Angle.x > 87.0f)
			Angle.x = 87.0f;

		if (Angle.y < -180.0f)
			Angle.y = 180.f - (Angle.y - 180.f);
		else if (Angle.y > 180.0f)
			Angle.y = -180.f + (Angle.y - 180.f);

	}
};