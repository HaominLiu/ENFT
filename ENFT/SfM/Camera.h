
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "RigidTransformation.h"
#include "Point.h"

class Camera : public RigidTransformation3D
{

public:

	class IntrinsicParameter
	{
	public:
		IntrinsicParameter() : m_f(1.0f), m_d(0.0f) {}
		inline void Set(const float &f, const float &d) { m_f = f; m_d = d; }
		inline const float& f() const { return m_f; }	inline float& f() { return m_f; }
		inline const float& d() const { return m_d; }	inline float& d() { return m_d; }
		inline void Scale(const float &s) { m_f *= s; m_d /= s * s; }
		inline void Undistort(Point2D &x) const { x *= 1 + m_d * x.SquaredLength(); }
		inline void Rectify(Point2D &x) const { x *= (1 + m_d * x.SquaredLength()) / m_f; }
		inline void Rectify(const Point2D &xd, Point2D &xr) const
		{
			xr.y() = (1 + m_d * xd.SquaredLength()) / m_f;
			xr.x() = xr.y() * xd.x();
			xr.y() = xr.y() * xd.x();
		}
		inline void SaveB(FILE *fp) const { fwrite(this, sizeof(IntrinsicParameter), 1, fp); }
		inline void LoadB(FILE *fp) { fread(this, sizeof(IntrinsicParameter), 1, fp); }
		inline void Print() const { printf("f = %f, d = %f\n", m_f, m_d); }
	protected:
		float m_f, m_d;
	};

	Camera() {}
	Camera(const RigidTransformation3D &T) { memcpy(this, &T, 48); }
	inline void operator = (const RigidTransformation3D &T) { memcpy(this, &T, 48); }
	inline void operator = (const RotationTransformation3D &R) { memcpy(this, &R, 48); }
	inline void operator = (const LA::AlignedMatrix3x4f &M) { memcpy(this, &M, 48); }
	
	inline void ProjectToNormalizedPlane(const Point3D &X, Point2D &x) const
	{
		x.y() = 1 / SSE::Sum0123(_mm_mul_ps(r20_r21_r22_tZ(), X.XYZx()));
		x.x() = SSE::Sum0123(_mm_mul_ps(r00_r01_r02_tX(), X.XYZx())) * x.y();
		x.y() = SSE::Sum0123(_mm_mul_ps(r10_r11_r12_tY(), X.XYZx())) * x.y();
	}
	inline void ProjectToNormalizedPlane(const Point3D &X, float &Zc, Point2D &x) const
	{
		Zc = SSE::Sum0123(_mm_mul_ps(r20_r21_r22_tZ(), X.XYZx()));
		x.y() = 1 / Zc;
		x.x() = SSE::Sum0123(_mm_mul_ps(r00_r01_r02_tX(), X.XYZx())) * x.y();
		x.y() = SSE::Sum0123(_mm_mul_ps(r10_r11_r12_tY(), X.XYZx())) * x.y();
	}
	inline void ProjectToNormalizedPlane(const Point3D &X, Point3D &Xc, Point2D &x) const
	{
		x.y() = 1 / (Xc.Z() = SSE::Sum0123(_mm_mul_ps(r20_r21_r22_tZ(), X.XYZx())));
		x.x() = (Xc.X() = SSE::Sum0123(_mm_mul_ps(r00_r01_r02_tX(), X.XYZx()))) * x.y();
		x.y() = (Xc.Y() = SSE::Sum0123(_mm_mul_ps(r10_r11_r12_tY(), X.XYZx()))) * x.y();
	}
	inline void ProjectToNormalizedPlane(const Point3D &X, float &RX_X, float &RX_Y, float &RX_Z, float &ZcI, float &x, float &y) const
	{
		Apply(X, RX_X, RX_Y, RX_Z, x, y, ZcI);
		ZcI = 1 / ZcI;
		x *= ZcI;
		y *= ZcI;
	}
	inline bool ProjectToNormalizedPlane(const Point3D &X, float &ZcI, float &x, float &y) const
	{
		Apply(X, x, y, ZcI);
		if(ZcI < FLT_EPSILON)
			return false;
		ZcI = 1 / ZcI;
		x *= ZcI;
		y *= ZcI;
		return true;
	}
	inline void ProjectToNormalizedPlane(const __m128 &X, float &ZcI, float &x, float &y) const
	{
		Apply(X, x, y, ZcI);
		ZcI = 1 / ZcI;
		x *= ZcI;
		y *= ZcI;
	}
	inline void ProjectToNormalizedPlane(const Point3D &X, const Point3DCovariance &PX, Point2D &x, Point2DCovariance &Px, LA::AlignedMatrix2x3f &ZcH, 
										 __m128 *work4) const
	{
		ProjectToNormalizedPlane(X, Px.cyy(), x.x(), x.y());
		Px.cyy() = Px.cyy() * Px.cyy();
		ZcH.M_00_01_02_x() = _mm_sub_ps(r_00_01_02_x(), _mm_mul_ps(_mm_set1_ps(x.x()), r_20_21_22_x()));
		ZcH.M_10_11_12_x() = _mm_sub_ps(r_10_11_12_x(), _mm_mul_ps(_mm_set1_ps(x.y()), r_20_21_22_x()));

		work4[0] = _mm_setr_ps(PX.M00(), PX.M01(), PX.M02(), 0.0f);
		work4[1] = _mm_setr_ps(PX.M10(), PX.M11(), PX.M12(), 0.0f);
		work4[2] = _mm_setr_ps(PX.M20(), PX.M21(), PX.M22(), 0.0f);

		work4[3].m128_f32[0] = SSE::Sum012(_mm_mul_ps(work4[0], ZcH.M_00_01_02_x()));
		work4[3].m128_f32[1] = SSE::Sum012(_mm_mul_ps(work4[1], ZcH.M_00_01_02_x()));
		work4[3].m128_f32[2] = SSE::Sum012(_mm_mul_ps(work4[2], ZcH.M_00_01_02_x()));
		Px.cxx() = Px.cyy() * SSE::Sum012(_mm_mul_ps(ZcH.M_00_01_02_x(), work4[3]));

		work4[3].m128_f32[0] = SSE::Sum012(_mm_mul_ps(work4[0], ZcH.M_10_11_12_x()));
		work4[3].m128_f32[1] = SSE::Sum012(_mm_mul_ps(work4[1], ZcH.M_10_11_12_x()));
		work4[3].m128_f32[2] = SSE::Sum012(_mm_mul_ps(work4[2], ZcH.M_10_11_12_x()));
		Px.cxy() = Px.cyy() * SSE::Sum012(_mm_mul_ps(ZcH.M_00_01_02_x(), work4[3]));
		Px.cyy() = Px.cyy() * SSE::Sum012(_mm_mul_ps(ZcH.M_10_11_12_x(), work4[3]));
	}
	inline void ProjectToNormalizedPlane2(const Point3D *pX2, __m128 &x2, __m128 &work) const
	{
		Apply(pX2[0], x2.m128_f32[0], x2.m128_f32[1], work.m128_f32[0]);	work.m128_f32[0] = work.m128_f32[1] = 1 / work.m128_f32[0];
		Apply(pX2[1], x2.m128_f32[2], x2.m128_f32[3], work.m128_f32[2]);	work.m128_f32[2] = work.m128_f32[3] = 1 / work.m128_f32[2];
		x2 = _mm_mul_ps(x2, work);
	}
	static inline void ProjectToNormalizedPlane2(const Camera &C0, const Camera &C1, const Point3D &X0, const Point3D &X1, __m128 &x2, __m128 &work)
	{
		C0.Apply(X0, x2.m128_f32[0], x2.m128_f32[1], work.m128_f32[0]);	work.m128_f32[0] = work.m128_f32[1] = 1 / work.m128_f32[0];
		C1.Apply(X1, x2.m128_f32[2], x2.m128_f32[3], work.m128_f32[1]);	work.m128_f32[2] = work.m128_f32[3] = 1 / work.m128_f32[2];
		x2 = _mm_mul_ps(x2, work);
	}
	static inline void ProjectToNormalizedPlane2(const Camera &C0, const Camera &C1, const Point3D &X, float &ZcI0, float &ZcI1, __m128 &x2, __m128 &work)
	{
		C0.Apply(X, x2.m128_f32[0], x2.m128_f32[1], ZcI0);	ZcI0 = work.m128_f32[0] = work.m128_f32[1] = 1 / ZcI0;
		C1.Apply(X, x2.m128_f32[2], x2.m128_f32[3], ZcI1);	ZcI1 = work.m128_f32[2] = work.m128_f32[3] = 1 / ZcI1;
		x2 = _mm_mul_ps(x2, work);
	}
	inline void ComputeProjectionError(const Point3D &X, const Point2D &x, Point2D &e) const
	{
		ProjectToNormalizedPlane(X, e);
		e.x() = x.x() - e.x();
		e.y() = x.y() - e.y();
	}
	inline void ComputeProjectionError(const Point3D &X, const Point2D &x, float &Zc, Point2D &e) const
	{
		Zc = SSE::Sum0123(_mm_mul_ps(r20_r21_r22_tZ(), X.XYZx()));
		e.y() = 1 / Zc;
		e.x() = x.x() - SSE::Sum0123(_mm_mul_ps(r00_r01_r02_tX(), X.XYZx())) * e.y();
		e.y() = x.y() - SSE::Sum0123(_mm_mul_ps(r10_r11_r12_tY(), X.XYZx())) * e.y();
	}
	inline float ComputeProjectionSquaredError(const Point3D &X, const Point2D &x, Point2D &e) const
	{
		ComputeProjectionError(X, x, e);
		return e.SquaredLength();
	}
	inline void ComputeProjectionError2(const Point3D &X0, const Point3D &X1, const __m128 &x2, __m128 &e2, __m128 &work) const
	{
		Apply(X0, e2.m128_f32[0], e2.m128_f32[1], work.m128_f32[0]);		work.m128_f32[0] = work.m128_f32[1] = 1 / work.m128_f32[0];
		Apply(X1, e2.m128_f32[2], e2.m128_f32[3], work.m128_f32[2]);		work.m128_f32[2] = work.m128_f32[3] = 1 / work.m128_f32[2];
		e2 = _mm_sub_ps(x2, _mm_mul_ps(e2, work));
	}
	inline void ComputeProjectionError2(const Point3D &X0, const Point3D &X1, const __m128 &x2, LA::Vector2f &Zc2, __m128 &e2, __m128 &work) const
	{
		Apply(X0, e2.m128_f32[0], e2.m128_f32[1], Zc2.v0());		work.m128_f32[0] = work.m128_f32[1] = 1 / Zc2.v0();
		Apply(X1, e2.m128_f32[2], e2.m128_f32[3], Zc2.v1());		work.m128_f32[2] = work.m128_f32[3] = 1 / Zc2.v1();
		e2 = _mm_sub_ps(x2, _mm_mul_ps(e2, work));
	}
	static inline void ComputeProjectionError2(const Camera &C0, const Camera &C1, const Point3D &X0, const Point3D &X1, const __m128 &x2, __m128 &e2, 
											   __m128 &work)
	{
		C0.Apply(X0, e2.m128_f32[0], e2.m128_f32[1], work.m128_f32[0]);		work.m128_f32[0] = work.m128_f32[1] = 1 / work.m128_f32[0];
		C1.Apply(X1, e2.m128_f32[2], e2.m128_f32[3], work.m128_f32[2]);		work.m128_f32[2] = work.m128_f32[3] = 1 / work.m128_f32[2];
		e2 = _mm_sub_ps(x2, _mm_mul_ps(e2, work));
	}
	static inline void ComputeProjectionError2(const Camera &C0, const Camera &C1, const Point3D &X0, const Point3D &X1, const __m128 &x2, LA::Vector2f &Zc2, 
											   __m128 &e2, __m128 &work)
	{
		C0.Apply(X0, e2.m128_f32[0], e2.m128_f32[1], Zc2.v0());		work.m128_f32[0] = work.m128_f32[1] = 1 / Zc2.v0();
		C1.Apply(X1, e2.m128_f32[2], e2.m128_f32[3], Zc2.v1());		work.m128_f32[2] = work.m128_f32[3] = 1 / Zc2.v1();
		e2 = _mm_sub_ps(x2, _mm_mul_ps(e2, work));
	}
	static inline void ComputeProjectionError2(const Camera &C0, const Camera &C1, const Point3D &X0, const Point3D &X1, const Point2D &x0, const Point2D &x1, 
											   LA::Vector2f &Zc2, __m128 &e2, __m128 &work)
	{
		C0.Apply(X0, e2.m128_f32[0], e2.m128_f32[1], Zc2.v0());		work.m128_f32[0] = work.m128_f32[1] = 1 / Zc2.v0();
		C1.Apply(X1, e2.m128_f32[2], e2.m128_f32[3], Zc2.v1());		work.m128_f32[2] = work.m128_f32[3] = 1 / Zc2.v1();
		e2 = _mm_mul_ps(e2, work);
		e2 = _mm_setr_ps(x0.x() - e2.m128_f32[0], x0.y() - e2.m128_f32[1], x1.x() - e2.m128_f32[2], x1.y() - e2.m128_f32[3]);
	}
	inline float ComputeDepth(const Point3D &X) const {	return SSE::Sum0123(_mm_mul_ps(r20_r21_r22_tZ(), X.XYZx())); }
	inline bool CheckCheirality(const Point3D &X) const { return ComputeDepth(X) > 0; }
	inline bool ComputeProjectionError_CheckCheirality(const Point3D &X, const Point2D &x, Point2D &e) const
	{
		e.y() = SSE::Sum0123(_mm_mul_ps(r20_r21_r22_tZ(), X.v012x()));
		if(e.y() < 0)
			return false;
		e.y() = 1 / e.y();
		e.x() = x.x() - SSE::Sum0123(_mm_mul_ps(r00_r01_r02_tX(), X.v012x())) * e.y();
		e.y() = x.y() - SSE::Sum0123(_mm_mul_ps(r10_r11_r12_tY(), X.v012x())) * e.y();
		return true;
	}
	inline void ComputeRayDirection(const Point2D &x, Point3D &rayDir) const
	{
		ApplyRotationInversely(x, rayDir);
		rayDir.Normalize();
		rayDir.reserve() = 1.0f;
	}

	inline void Print() const
	{
		if(IsInvalid())
			printf("Invalid!\n");
		else
		{
			printf("%f %f %f %f\n", r00(), r01(), r02(), tX());
			printf("%f %f %f %f\n", r10(), r11(), r12(), tY());
			printf("%f %f %f %f\n", r20(), r21(), r22(), tZ());
			//float w[3];
			//ToRodrigues(w);
			//printf("w = [ %f %f %f ]\n", w[0], w[1], w[2]);
			//Point3D Cc;
			//GetCenter(Cc);
			//printf("c = [ %f %f %f ]\n", Cc.X(), Cc.Y(), Cc.Z());
		}
	}

	static inline float DotOrientation(const Camera &C1, const Camera &C2) { return SSE::Sum012(_mm_mul_ps(C1.r_20_21_22_x(), C2.r_20_21_22_x())); }
};

#endif