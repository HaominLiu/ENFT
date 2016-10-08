
#ifndef _TRANSLATION_H_
#define _TRANSLATION_H_

#include "Point.h"

class Translation : public Point3D
{

public:

	inline const float& tX() const { return v0(); }			inline float& tX() { return v0(); }
	inline const float& tY() const { return v1(); }			inline float& tY() { return v1(); }
	inline const float& tZ() const { return v2(); }			inline float& tZ() { return v2(); }
	inline const __m128& tXYZ() const { return XYZx(); }	inline __m128& tXYZ() { return XYZx(); }

	inline void Apply(const Point3D &RX, __m128 &Xc) const { Xc = _mm_add_ps(RX.XYZx(), tXYZ()); }
	inline void ProjectToNormalizedPlane(const Point3D &RX, float &ZcI, Point2D &x) const
	{
		ZcI = 1 / (RX.Z() + tZ());
		x.x() = (RX.X() + tX()) * ZcI;
		x.y() = (RX.Y() + tY()) * ZcI;
	}
	inline void ComputeProjectionError2(const Point3D &RX0, const Point3D &RX1, const __m128 &x2, Point2D &Zc2, __m128 &e2, __m128 *work2) const
	{
		Apply(RX0, work2[0]);	memcpy(&e2.m128_f32[0], work2, 8);	work2[1].m128_f32[0] = work2[1].m128_f32[1] = 1 / (Zc2.v0() = work2[0].m128_f32[2]);
		Apply(RX1, work2[0]);	memcpy(&e2.m128_f32[2], work2, 8);	work2[1].m128_f32[2] = work2[1].m128_f32[3] = 1 / (Zc2.v1() = work2[0].m128_f32[2]);
		e2 = _mm_sub_ps(x2, _mm_mul_ps(e2, work2[1]));
	}
	inline void ComputeProjectionError2(const Point3D &RX0, const Point3D &RX1, const __m128 &x2, __m128 &e2, __m128 *work2) const
	{
		Apply(RX0, work2[0]);	memcpy(&e2.m128_f32[0], work2, 8);	work2[1].m128_f32[0] = work2[1].m128_f32[1] = 1 / work2[0].m128_f32[2];
		Apply(RX1, work2[0]);	memcpy(&e2.m128_f32[2], work2, 8);	work2[1].m128_f32[2] = work2[1].m128_f32[3] = 1 / work2[0].m128_f32[2];
		e2 = _mm_sub_ps(x2, _mm_mul_ps(e2, work2[1]));
	}
	inline void ComputeProjectionError(const Point3D &RX, const Point2D &x, Point2D &e) const
	{
		e.y() = 1 / (RX.Z() + tZ());
		e.x() = x.x() - (RX.X() + tX()) * e.y();
		e.y() = x.y() - (RX.Y() + tY()) * e.y();
	}
	inline bool ComputeProjectionError_CheckCheirality(const Point3D &RX, const Point2D &x, Point2D &e) const
	{
		e.y() = RX.Z() + tZ();
		if(e.y() < 0)
			return false;
		e.y() = 1 / e.y();
		e.x() = x.x() - (RX.X() + tX()) * e.y();
		e.y() = x.y() - (RX.Y() + tY()) * e.y();
		return true;
	}
	inline float ComputeProjectionSquaredError(const Point3D &RX, const Point2D &x, Point2D &e) const
	{
		ComputeProjectionError(RX, x, e);
		return e.SquaredLength();
	}
};

#endif