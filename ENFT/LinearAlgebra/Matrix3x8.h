
#ifndef _MATRIX_3x8_H_
#define _MATRIX_3x8_H_

#include "Matrix2x3.h"
#include "Matrix2x8.h"
#include "Matrix3.h"

namespace LA
{

	class AlignedMatrix3x8f : public AlignedMatrix2x8f
	{

	public:

		inline const __m128& M_20_21_22_23() const { return m_20_21_22_23; }		inline __m128& M_20_21_22_23() { return m_20_21_22_23; }
		inline const __m128& M_24_25_26_27() const { return m_24_25_26_27; }		inline __m128& M_24_25_26_27() { return m_24_25_26_27; }
		inline const float& M20() const { return m_20_21_22_23.m128_f32[0]; }		inline float& M20() { return m_20_21_22_23.m128_f32[0]; }
		inline const float& M21() const { return m_20_21_22_23.m128_f32[1]; }		inline float& M21() { return m_20_21_22_23.m128_f32[1]; }
		inline const float& M22() const { return m_20_21_22_23.m128_f32[2]; }		inline float& M22() { return m_20_21_22_23.m128_f32[2]; }
		inline const float& M23() const { return m_20_21_22_23.m128_f32[3]; }		inline float& M23() { return m_20_21_22_23.m128_f32[3]; }
		inline const float& M24() const { return m_24_25_26_27.m128_f32[0]; }		inline float& M24() { return m_24_25_26_27.m128_f32[0]; }
		inline const float& M25() const { return m_24_25_26_27.m128_f32[1]; }		inline float& M25() { return m_24_25_26_27.m128_f32[1]; }
		inline const float& M26() const { return m_24_25_26_27.m128_f32[2]; }		inline float& M26() { return m_24_25_26_27.m128_f32[2]; }
		inline const float& M27() const { return m_24_25_26_27.m128_f32[3]; }		inline float& M27() { return m_24_25_26_27.m128_f32[3]; }

	protected:

		__m128 m_20_21_22_23, m_24_25_26_27;

	};

	inline void AATUpper(const AlignedMatrix3x8f &A, AlignedMatrix3f &AAT)
	{
		AAT.M00() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), A.M_00_01_02_03()), _mm_mul_ps(A.M_04_05_06_07(), A.M_04_05_06_07())));
		AAT.M01() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), A.M_10_11_12_13()), _mm_mul_ps(A.M_04_05_06_07(), A.M_14_15_16_17())));
		AAT.M02() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), A.M_20_21_22_23()), _mm_mul_ps(A.M_04_05_06_07(), A.M_24_25_26_27())));
		AAT.M11() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_10_11_12_13(), A.M_10_11_12_13()), _mm_mul_ps(A.M_14_15_16_17(), A.M_14_15_16_17())));
		AAT.M12() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_10_11_12_13(), A.M_20_21_22_23()), _mm_mul_ps(A.M_14_15_16_17(), A.M_24_25_26_27())));
		AAT.M22() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_20_21_22_23(), A.M_20_21_22_23()), _mm_mul_ps(A.M_24_25_26_27(), A.M_24_25_26_27())));
	}
	inline void AddATBTo(const LA::AlignedMatrix3x8f &A, const LA::Vector3f &B, LA::AlignedVector8f &to, __m128 *work3)
	{
		work3[0] = _mm_set1_ps(B.v0());		work3[1] = _mm_set1_ps(B.v1());		work3[2] = _mm_set1_ps(B.v2());
		to.v0123() = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), work3[0]), _mm_mul_ps(A.M_10_11_12_13(), work3[1])), 
													  _mm_mul_ps(A.M_20_21_22_23(), work3[2])), to.v0123());
		to.v4567() = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(A.M_04_05_06_07(), work3[0]), _mm_mul_ps(A.M_14_15_16_17(), work3[1])), 
													  _mm_mul_ps(A.M_24_25_26_27(), work3[2])), to.v4567());
	}
	inline void ATB(const Matrix2x3f &A, const AlignedMatrix2x8f &B, AlignedMatrix3x8f &ATB, __m128 *work2)
	{
		work2[0] = _mm_set1_ps(A.M00());	work2[1] = _mm_set1_ps(A.M10());
		ATB.M_00_01_02_03() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_00_01_02_03()), _mm_mul_ps(work2[1], B.M_10_11_12_13()));
		ATB.M_04_05_06_07() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_04_05_06_07()), _mm_mul_ps(work2[1], B.M_14_15_16_17()));
		work2[0] = _mm_set1_ps(A.M01());	work2[1] = _mm_set1_ps(A.M11());
		ATB.M_10_11_12_13() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_00_01_02_03()), _mm_mul_ps(work2[1], B.M_10_11_12_13()));
		ATB.M_14_15_16_17() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_04_05_06_07()), _mm_mul_ps(work2[1], B.M_14_15_16_17()));
		work2[0] = _mm_set1_ps(A.M02());	work2[1] = _mm_set1_ps(A.M12());
		ATB.M_20_21_22_23() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_00_01_02_03()), _mm_mul_ps(work2[1], B.M_10_11_12_13()));
		ATB.M_24_25_26_27() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_04_05_06_07()), _mm_mul_ps(work2[1], B.M_14_15_16_17()));
	}
	inline void ATB(const AlignedMatrix2x3f &A, const AlignedMatrix2x8f &B, AlignedMatrix3x8f &ATB, __m128 *work2)
	{
		work2[0] = _mm_set1_ps(A.M00());	work2[1] = _mm_set1_ps(A.M10());
		ATB.M_00_01_02_03() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_00_01_02_03()), _mm_mul_ps(work2[1], B.M_10_11_12_13()));
		ATB.M_04_05_06_07() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_04_05_06_07()), _mm_mul_ps(work2[1], B.M_14_15_16_17()));
		work2[0] = _mm_set1_ps(A.M01());	work2[1] = _mm_set1_ps(A.M11());
		ATB.M_10_11_12_13() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_00_01_02_03()), _mm_mul_ps(work2[1], B.M_10_11_12_13()));
		ATB.M_14_15_16_17() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_04_05_06_07()), _mm_mul_ps(work2[1], B.M_14_15_16_17()));
		work2[0] = _mm_set1_ps(A.M02());	work2[1] = _mm_set1_ps(A.M12());
		ATB.M_20_21_22_23() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_00_01_02_03()), _mm_mul_ps(work2[1], B.M_10_11_12_13()));
		ATB.M_24_25_26_27() = _mm_add_ps(_mm_mul_ps(work2[0], B.M_04_05_06_07()), _mm_mul_ps(work2[1], B.M_14_15_16_17()));
	}
	inline void ATB(const SymmetricMatrix3f &A, const AlignedMatrix3x8f &B, AlignedMatrix3x8f &ATB, __m128 *work4)
	{
		work4[0] = _mm_set1_ps(A.M00());	work4[1] = _mm_set1_ps(A.M10());	work4[2] = _mm_set1_ps(A.M20());
		ATB.M_00_01_02_03() = _mm_add_ps(_mm_add_ps(_mm_mul_ps(work4[0], B.M_00_01_02_03()), _mm_mul_ps(work4[1], B.M_10_11_12_13())), 
													_mm_mul_ps(work4[2], B.M_20_21_22_23()));
		ATB.M_04_05_06_07() = _mm_add_ps(_mm_add_ps(_mm_mul_ps(work4[0], B.M_04_05_06_07()), _mm_mul_ps(work4[1], B.M_14_15_16_17())), 
													_mm_mul_ps(work4[2], B.M_24_25_26_27()));
		work4[0] = _mm_set1_ps(A.M11());	work4[3] = _mm_set1_ps(A.M21());
		ATB.M_10_11_12_13() = _mm_add_ps(_mm_add_ps(_mm_mul_ps(work4[1], B.M_00_01_02_03()), _mm_mul_ps(work4[0], B.M_10_11_12_13())), 
													_mm_mul_ps(work4[3], B.M_20_21_22_23()));
		ATB.M_14_15_16_17() = _mm_add_ps(_mm_add_ps(_mm_mul_ps(work4[1], B.M_04_05_06_07()), _mm_mul_ps(work4[0], B.M_14_15_16_17())), 
													_mm_mul_ps(work4[3], B.M_24_25_26_27()));
		work4[0] = _mm_set1_ps(A.M22());
		ATB.M_20_21_22_23() = _mm_add_ps(_mm_add_ps(_mm_mul_ps(work4[2], B.M_00_01_02_03()), _mm_mul_ps(work4[3], B.M_10_11_12_13())), 
													_mm_mul_ps(work4[0], B.M_20_21_22_23()));
		ATB.M_24_25_26_27() = _mm_add_ps(_mm_add_ps(_mm_mul_ps(work4[2], B.M_04_05_06_07()), _mm_mul_ps(work4[3], B.M_14_15_16_17())), 
													_mm_mul_ps(work4[0], B.M_24_25_26_27()));
	}
	inline void SubtractATBFrom(const AlignedMatrix3x8f &A, const Vector3f &B, AlignedVector8f &from, __m128 *work3)
	{
		work3[0] = _mm_set1_ps(B.v0());		work3[1] = _mm_set1_ps(B.v1());		work3[2] = _mm_set1_ps(B.v2());
		from.v0123() = _mm_sub_ps(from.v0123(), _mm_add_ps(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), work3[0]), _mm_mul_ps(A.M_10_11_12_13(), work3[1])), 
																	  _mm_mul_ps(A.M_20_21_22_23(), work3[2])));
		from.v4567() = _mm_sub_ps(from.v4567(), _mm_add_ps(_mm_add_ps(_mm_mul_ps(A.M_04_05_06_07(), work3[0]), _mm_mul_ps(A.M_14_15_16_17(), work3[1])), 
																	  _mm_mul_ps(A.M_24_25_26_27(), work3[2])));
	}
	inline void AddABTo(const LA::AlignedMatrix3x8f &A, const LA::AlignedVector8f &B, LA::Vector3f &to)
	{
		to.v0() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), B.v0123()), _mm_mul_ps(A.M_04_05_06_07(), B.v4567()))) + to.v0();
		to.v1() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_10_11_12_13(), B.v0123()), _mm_mul_ps(A.M_14_15_16_17(), B.v4567()))) + to.v1();
		to.v2() = SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_20_21_22_23(), B.v0123()), _mm_mul_ps(A.M_24_25_26_27(), B.v4567()))) + to.v2();
	}
	inline void AddABTo(const LA::AlignedMatrix3x8f &A, const LA::AlignedVector8f &B, LA::Vector3f &to, __m128 *work0) { AddABTo(A, B, to); }
	inline void SubtractABFrom(const LA::AlignedMatrix3x8f &A, const LA::AlignedVector8f &B, LA::Vector3f &from)
	{
		from.v0() -= SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_00_01_02_03(), B.v0123()), _mm_mul_ps(A.M_04_05_06_07(), B.v4567())));
		from.v1() -= SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_10_11_12_13(), B.v0123()), _mm_mul_ps(A.M_14_15_16_17(), B.v4567())));
		from.v2() -= SSE::Sum0123(_mm_add_ps(_mm_mul_ps(A.M_20_21_22_23(), B.v0123()), _mm_mul_ps(A.M_24_25_26_27(), B.v4567())));
	}
	inline void SubtractABFrom(const LA::AlignedMatrix3x8f &A, const LA::AlignedVector8f &B, LA::Vector3f &from, __m128 *work0) { SubtractABFrom(A, B, from); }
	inline void SubtractABFrom(const LA::AlignedMatrix2x3f &A, const LA::AlignedMatrix3x8f &B, LA::AlignedMatrix2x8f &from, __m128 *work3)
	{
		work3[0] = _mm_set1_ps(A.M00());
		work3[1] = _mm_set1_ps(A.M01());
		work3[2] = _mm_set1_ps(A.M02());
		from.M_00_01_02_03() = _mm_sub_ps(from.M_00_01_02_03(), _mm_add_ps(_mm_add_ps(_mm_mul_ps(work3[0], B.M_00_01_02_03()), _mm_mul_ps(work3[1], B.M_10_11_12_13())), 
																					  _mm_mul_ps(work3[2], B.M_20_21_22_23())));
		from.M_04_05_06_07() = _mm_sub_ps(from.M_04_05_06_07(), _mm_add_ps(_mm_add_ps(_mm_mul_ps(work3[0], B.M_04_05_06_07()), _mm_mul_ps(work3[1], B.M_14_15_16_17())), 
																					  _mm_mul_ps(work3[2], B.M_24_25_26_27())));
		work3[0] = _mm_set1_ps(A.M10());
		work3[1] = _mm_set1_ps(A.M11());
		work3[2] = _mm_set1_ps(A.M12());
		from.M_10_11_12_13() = _mm_sub_ps(from.M_10_11_12_13(), _mm_add_ps(_mm_add_ps(_mm_mul_ps(work3[0], B.M_00_01_02_03()), _mm_mul_ps(work3[1], B.M_10_11_12_13())), 
																					  _mm_mul_ps(work3[2], B.M_20_21_22_23())));
		from.M_14_15_16_17() = _mm_sub_ps(from.M_14_15_16_17(), _mm_add_ps(_mm_add_ps(_mm_mul_ps(work3[0], B.M_04_05_06_07()), _mm_mul_ps(work3[1], B.M_14_15_16_17())), 
																					  _mm_mul_ps(work3[2], B.M_24_25_26_27())));
	}
	inline void AddATo(const AlignedMatrix3x8f &A, AlignedMatrix3x8f &to)
	{
		to.M_00_01_02_03() = _mm_add_ps(A.M_00_01_02_03(), to.M_00_01_02_03());		to.M_04_05_06_07() = _mm_add_ps(A.M_04_05_06_07(), to.M_04_05_06_07());
		to.M_10_11_12_13() = _mm_add_ps(A.M_10_11_12_13(), to.M_10_11_12_13());		to.M_14_15_16_17() = _mm_add_ps(A.M_14_15_16_17(), to.M_14_15_16_17());
		to.M_24_25_26_27() = _mm_add_ps(A.M_24_25_26_27(), to.M_24_25_26_27());		to.M_24_25_26_27() = _mm_add_ps(A.M_24_25_26_27(), to.M_24_25_26_27());
	}
	inline void s1s2TA(const Vector3f &s1, const AlignedVector8f &s2, AlignedMatrix3x8f &A, __m128 &work)
	{
		work = _mm_set1_ps(s1.v0());
		A.M_00_01_02_03() = _mm_mul_ps(_mm_mul_ps(work, s2.v0123()), A.M_00_01_02_03());
		A.M_04_05_06_07() = _mm_mul_ps(_mm_mul_ps(work, s2.v4567()), A.M_04_05_06_07());
		work = _mm_set1_ps(s1.v1());
		A.M_10_11_12_13() = _mm_mul_ps(_mm_mul_ps(work, s2.v0123()), A.M_10_11_12_13());
		A.M_14_15_16_17() = _mm_mul_ps(_mm_mul_ps(work, s2.v4567()), A.M_14_15_16_17());
		work = _mm_set1_ps(s1.v2());
		A.M_20_21_22_23() = _mm_mul_ps(_mm_mul_ps(work, s2.v0123()), A.M_20_21_22_23());
		A.M_24_25_26_27() = _mm_mul_ps(_mm_mul_ps(work, s2.v4567()), A.M_24_25_26_27());
	}

	template<ushort STAGE> inline void SetReserve(AlignedMatrix3x8f &M) {}
	inline void SetReserve(AlignedMatrix3x8f &M) {}
}

#endif