
#ifndef _MATRIX_2x4_H_
#define _MATRIX_2x4_H_

#include "Vector2.h"
#include "Vector4.h"

namespace LA
{

	class AlignedMatrix2x4f
	{

	public:

		inline const __m128& M_00_01_02_03() const { return m_00_01_02_03; }	inline __m128& M_00_01_02_03() { return m_00_01_02_03; }
		inline const __m128& M_10_11_12_13() const { return m_10_11_12_13; }	inline __m128& M_10_11_12_13() { return m_10_11_12_13; }
		inline const float& M00() const { return m_00_01_02_03.m128_f32[0]; }	inline float& M00()	{ return m_00_01_02_03.m128_f32[0]; }
		inline const float& M01() const { return m_00_01_02_03.m128_f32[1]; }	inline float& M01()	{ return m_00_01_02_03.m128_f32[1]; }
		inline const float& M02() const { return m_00_01_02_03.m128_f32[2]; }	inline float& M02()	{ return m_00_01_02_03.m128_f32[2]; }
		inline const float& M03() const { return m_00_01_02_03.m128_f32[3]; }	inline float& M03() { return m_00_01_02_03.m128_f32[3]; }
		inline const float& M10() const { return m_10_11_12_13.m128_f32[0]; }	inline float& M10()	{ return m_10_11_12_13.m128_f32[0]; }
		inline const float& M11() const { return m_10_11_12_13.m128_f32[1]; }	inline float& M11()	{ return m_10_11_12_13.m128_f32[1]; }
		inline const float& M12() const { return m_10_11_12_13.m128_f32[2]; }	inline float& M12()	{ return m_10_11_12_13.m128_f32[2]; }
		inline const float& M13() const { return m_10_11_12_13.m128_f32[3]; }	inline float& M13() { return m_10_11_12_13.m128_f32[3]; }
		inline operator const float* () const { return (const float *) this; }
		inline operator		  float* ()		  { return (	  float *) this; }
		inline void SetZero() { memset(this, 0, sizeof(AlignedMatrix2x4f)); }

	protected:

		__m128 m_00_01_02_03, m_10_11_12_13;

	};

	inline void AddATBTo(const AlignedMatrix2x4f &A, const Vector2f &B, AlignedVector4f &to)
	{
		to.v0123() = _mm_add_ps(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), _mm_set1_ps(B.v0())), _mm_mul_ps(A.M_10_11_12_13(), _mm_set1_ps(B.v1()))), to.v0123());
	}
	inline void AddAij2To(const AlignedMatrix2x4f &A, AlignedVector4f &to)
	{
		to.v0123() = _mm_add_ps(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), A.M_00_01_02_03()), _mm_mul_ps(A.M_10_11_12_13(), A.M_10_11_12_13())), to.v0123());
	}
}

#endif