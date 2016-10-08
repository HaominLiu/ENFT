
#include "stdafx.h"
#include "HomographySolver.h"

bool HomographySolver::Run(const FourMatches2D &data, Homography &H, AlignedVector<__m128> &work)
{
#if 1
	m_ATA.SetZero();
	m_ATb.SetZero();
	m_ATA.M22() = m_ATA.M55() = 4.0f;

	const __m128 &u1 = data.u1(), &v1 = data.v1(), &u2 = data.u2(), &v2 = data.v2();
	work.Resize(4);
	__m128 &u1u1 = work[0], &u1v1 = work[1], &v1v1 = work[2], &u2u2pv2v2 = work[3];
	u1u1 = _mm_mul_ps(u1, u1);
	u1v1 = _mm_mul_ps(u1, v1);
	v1v1 = _mm_mul_ps(v1, v1);
	u2u2pv2v2 = _mm_add_ps(_mm_mul_ps(u2, u2), _mm_mul_ps(v2, v2));
	m_ATA.M00() = SSE::Sum0123(u1u1) + m_ATA.M00();
	m_ATA.M01() = SSE::Sum0123(u1v1) + m_ATA.M01();
	m_ATA.M02() = SSE::Sum0123(u1) + m_ATA.M02();
	m_ATA.M06() -= SSE::Sum0123(_mm_mul_ps(u1u1, u2));
	m_ATA.M07() -= SSE::Sum0123(_mm_mul_ps(u1v1, u2));
	m_ATA.M11() = SSE::Sum0123(v1v1) + m_ATA.M11();
	m_ATA.M12() = SSE::Sum0123(v1) + m_ATA.M12();
	m_ATA.M17() -= SSE::Sum0123(_mm_mul_ps(v1v1, u2));
	m_ATA.M26() -= SSE::Sum0123(_mm_mul_ps(u1, u2));
	m_ATA.M27() -= SSE::Sum0123(_mm_mul_ps(v1, u2));
	m_ATA.M36() -= SSE::Sum0123(_mm_mul_ps(u1u1, v2));
	m_ATA.M37() -= SSE::Sum0123(_mm_mul_ps(u1v1, v2));
	m_ATA.M46() -= SSE::Sum0123(_mm_mul_ps(u1v1, v2));
	m_ATA.M47() -= SSE::Sum0123(_mm_mul_ps(v1v1, v2));
	m_ATA.M56() -= SSE::Sum0123(_mm_mul_ps(u1, v2));
	m_ATA.M57() -= SSE::Sum0123(_mm_mul_ps(v1, v2));
	m_ATA.M66() = SSE::Sum0123(_mm_mul_ps(u1u1, u2u2pv2v2)) + m_ATA.M66();
	m_ATA.M67() = SSE::Sum0123(_mm_mul_ps(u1v1, u2u2pv2v2)) + m_ATA.M67();
	m_ATA.M77() = SSE::Sum0123(_mm_mul_ps(v1v1, u2u2pv2v2)) + m_ATA.M77();
	m_ATA.M16() = m_ATA.M07();
	m_ATA.M33() = m_ATA.M00();
	m_ATA.M34() = m_ATA.M01();
	m_ATA.M35() = m_ATA.M02();
	m_ATA.M44() = m_ATA.M11();
	m_ATA.M45() = m_ATA.M12();
	m_ATb.v0() = SSE::Sum0123(_mm_mul_ps(u1, u2)) + m_ATb.v0();
	m_ATb.v1() = SSE::Sum0123(_mm_mul_ps(v1, u2)) + m_ATb.v1();
	m_ATb.v2() = SSE::Sum0123(u2) + m_ATb.v2();
	m_ATb.v3() = SSE::Sum0123(_mm_mul_ps(u1, v2)) + m_ATb.v3();
	m_ATb.v4() = SSE::Sum0123(_mm_mul_ps(v1, v2)) + m_ATb.v4();
	m_ATb.v5() = SSE::Sum0123(v2) + m_ATb.v5();
	m_ATb.v6() -= SSE::Sum0123(_mm_mul_ps(u1, u2u2pv2v2));
	m_ATb.v7() -= SSE::Sum0123(_mm_mul_ps(v1, u2u2pv2v2));
	//printf("----------------------------------------------------------------\n");
	//m_ATA.Print();
	//printf("----------------------------------------------------------------\n");
	//m_ATb.Print();
	if(!LA::SolveLinearSystemSymmetricUpper(m_ATA, m_ATb))
		return false;
	//printf("----------------------------------------------------------------\n");
	//printf("x = \n");
	//m_ATb.Print();
	memcpy(&H.M00(), &m_ATb.v0(), 12);
	memcpy(&H.M10(), &m_ATb.v3(), 12);
	memcpy(&H.M20(), &m_ATb.v6(), 8);
	H.M22() = 1;
	return true;
#else
	const __m128 &u1 = data.u1(), &v1 = data.v1(), &u2 = data.u2(), &v2 = data.v2();
	work.Resize(6);
	__m128 &u1u2 = work[0], &v1u2 = work[1], &u1v2 = work[2], &v1v2 = work[3];
	u1u2 = _mm_mul_ps(u1, u2);
	v1u2 = _mm_mul_ps(v1, u2);
	u1v2 = _mm_mul_ps(u1, v2);
	v1v2 = _mm_mul_ps(v1, v2);
	m_A.SetZero();
	m_A.M00() = m_A.M13() = u1.m128_f32[0];
	m_A.M01() = m_A.M14() = v1.m128_f32[0];
	m_A.M02() = m_A.M15() = 1.0f;
	m_A.M06() = -u1u2.m128_f32[0];
	m_A.M07() = -v1u2.m128_f32[0];
	m_A.M16() = -u1v2.m128_f32[0];
	m_A.M17() = -v1v2.m128_f32[0];
	m_b.v0() = u2.m128_f32[0];
	m_b.v1() = v2.m128_f32[0];
	m_A.M20() = m_A.M33() = u1.m128_f32[1];
	m_A.M21() = m_A.M34() = v1.m128_f32[1];
	m_A.M22() = m_A.M35() = 1.0f;
	m_A.M26() = -u1u2.m128_f32[1];
	m_A.M27() = -v1u2.m128_f32[1];
	m_A.M36() = -u1v2.m128_f32[1];
	m_A.M37() = -v1v2.m128_f32[1];
	m_b.v2() = u2.m128_f32[1];
	m_b.v3() = v2.m128_f32[1];
	m_A.M40() = m_A.M53() = u1.m128_f32[2];
	m_A.M41() = m_A.M54() = v1.m128_f32[2];
	m_A.M42() = m_A.M55() = 1.0f;
	m_A.M46() = -u1u2.m128_f32[2];
	m_A.M47() = -v1u2.m128_f32[2];
	m_A.M56() = -u1v2.m128_f32[2];
	m_A.M57() = -v1v2.m128_f32[2];
	m_b.v4() = u2.m128_f32[2];
	m_b.v5() = v2.m128_f32[2];
	m_A.M60() = m_A.M73() = u1.m128_f32[3];
	m_A.M61() = m_A.M74() = v1.m128_f32[3];
	m_A.M62() = m_A.M75() = 1.0f;
	m_A.M66() = -u1u2.m128_f32[3];
	m_A.M67() = -v1u2.m128_f32[3];
	m_A.M76() = -u1v2.m128_f32[3];
	m_A.M77() = -v1v2.m128_f32[3];
	m_b.v6() = u2.m128_f32[3];
	m_b.v7() = v2.m128_f32[3];
	//printf("----------------------------------------------------------------\n");
	//printf("A = \n");
	//m_A.Print();
	//printf("----------------------------------------------------------------\n");
	//printf("b = \n");
	//m_b.Print();
	//m_ATA.SetZero();
	//m_ATb.SetZero();
	//for(int i = 0; i < 8; ++i)
	//{
	//	for(int j = i; j < 8; ++j)
	//	for(int k = 0; k < 8; ++k)
	//		m_ATA[i * 8 + j] += m_A[k * 8 + i] * m_A[k * 8 + j];
	//	for(int k = 0; k < 8; ++k)
	//		m_ATb[i] += m_A[k * 8 + i] * m_b[k];
	//}
	//printf("----------------------------------------------------------------\n");
	//m_ATA.Print();
	//printf("----------------------------------------------------------------\n");
	//m_ATb.Print();
	if(!LA::SolveLinearSystem(m_A, m_b, (float *) work.Data()))
		return false;
	//printf("----------------------------------------------------------------\n");
	//printf("x = \n");
	//m_b.Print();
	memcpy(&H.M00(), &m_b.v0(), 12);
	memcpy(&H.M10(), &m_b.v3(), 12);
	memcpy(&H.M20(), &m_b.v6(), 8);
	H.M22() = 1;
	return true;
#endif
}

bool HomographySolver::Run(const MatchSet2D &data, Homography &H, AlignedVector<__m128> &work)
{
	m_ATA.SetZero();
	m_ATb.SetZero();
	m_ATA.M22() = m_ATA.M55() = float(data.Size());

	work.Resize(4);
	__m128 &u1u1 = work[0], &u1v1 = work[1], &v1v1 = work[2], &u2u2pv2v2 = work[3];
	const ushort nPacks = data.GetPacksNumber();
	for(ushort iu1 = 0, iv1 = 1, iu2 = 2, iv2 = 3; iu1 < nPacks; iu1 += 4, iv1 += 4, iu2 += 4, iv2 += 4)
	{
		const __m128 &u1 = data.GetPack(iu1), &v1 = data.GetPack(iv1), &u2 = data.GetPack(iu2), &v2 = data.GetPack(iv2);
		u1u1 = _mm_mul_ps(u1, u1);
		u1v1 = _mm_mul_ps(u1, v1);
		v1v1 = _mm_mul_ps(v1, v1);
		u2u2pv2v2 = _mm_add_ps(_mm_mul_ps(u2, u2), _mm_mul_ps(v2, v2));
		m_ATA.M00() = SSE::Sum0123(u1u1) + m_ATA.M00();
		m_ATA.M01() = SSE::Sum0123(u1v1) + m_ATA.M01();
		m_ATA.M02() = SSE::Sum0123(u1) + m_ATA.M02();
		m_ATA.M06() -= SSE::Sum0123(_mm_mul_ps(u1u1, u2));
		m_ATA.M07() -= SSE::Sum0123(_mm_mul_ps(u1v1, u2));
		m_ATA.M11() = SSE::Sum0123(v1v1) + m_ATA.M11();
		m_ATA.M12() = SSE::Sum0123(v1) + m_ATA.M12();
		m_ATA.M17() -= SSE::Sum0123(_mm_mul_ps(v1v1, u2));
		m_ATA.M26() -= SSE::Sum0123(_mm_mul_ps(u1, u2));
		m_ATA.M27() -= SSE::Sum0123(_mm_mul_ps(v1, u2));
		m_ATA.M36() -= SSE::Sum0123(_mm_mul_ps(u1u1, v2));
		m_ATA.M37() -= SSE::Sum0123(_mm_mul_ps(u1v1, v2));
		m_ATA.M46() -= SSE::Sum0123(_mm_mul_ps(u1v1, v2));
		m_ATA.M47() -= SSE::Sum0123(_mm_mul_ps(v1v1, v2));
		m_ATA.M56() -= SSE::Sum0123(_mm_mul_ps(u1, v2));
		m_ATA.M57() -= SSE::Sum0123(_mm_mul_ps(v1, v2));
		m_ATA.M66() = SSE::Sum0123(_mm_mul_ps(u1u1, u2u2pv2v2)) + m_ATA.M66();
		m_ATA.M67() = SSE::Sum0123(_mm_mul_ps(u1v1, u2u2pv2v2)) + m_ATA.M67();
		m_ATA.M77() = SSE::Sum0123(_mm_mul_ps(v1v1, u2u2pv2v2)) + m_ATA.M77();
		m_ATb.v0() = SSE::Sum0123(_mm_mul_ps(u1, u2)) + m_ATb.v0();
		m_ATb.v1() = SSE::Sum0123(_mm_mul_ps(v1, u2)) + m_ATb.v1();
		m_ATb.v2() = SSE::Sum0123(u2) + m_ATb.v2();
		m_ATb.v3() = SSE::Sum0123(_mm_mul_ps(u1, v2)) + m_ATb.v3();
		m_ATb.v4() = SSE::Sum0123(_mm_mul_ps(v1, v2)) + m_ATb.v4();
		m_ATb.v5() = SSE::Sum0123(v2) + m_ATb.v5();
		m_ATb.v6() -= SSE::Sum0123(_mm_mul_ps(u1, u2u2pv2v2));
		m_ATb.v7() -= SSE::Sum0123(_mm_mul_ps(v1, u2u2pv2v2));
	}
	for(ushort iRem = 0, i0 = (nPacks << 1), i1 = i0 + 1; iRem < data.GetRemindersNumber(); ++iRem, i0 += 2, i1 += 2)
	{
		const Point2D &x1 = data.GetReminder1(iRem), &x2 = data.GetReminder2(iRem);
		const float u1u1 = x1.x() * x1.x();
		const float u1v1 = x1.x() * x1.y();
		const float v1v1 = x1.y() * x1.y();
		const float u2u2pv2v2 = x2.x() * x2.x() + x2.y() * x2.y();
		m_ATA.M00() = u1u1 + m_ATA.M00();
		m_ATA.M01() = u1v1 + m_ATA.M01();
		m_ATA.M02() = x1.x() + m_ATA.M02();
		m_ATA.M06() -= u1u1 * x2.x();
		m_ATA.M07() -= u1v1 * x2.x();
		m_ATA.M11() = v1v1 + m_ATA.M11();
		m_ATA.M12() = x1.y() + m_ATA.M12();
		m_ATA.M17() -= v1v1 * x2.x();
		m_ATA.M26() -= x1.x() * x2.x();
		m_ATA.M27() -= x1.y() * x2.x();
		m_ATA.M36() -= u1u1 * x2.y();
		m_ATA.M37() -= u1v1 * x2.y();
		m_ATA.M46() -= u1v1 * x2.y();
		m_ATA.M47() -= v1v1 * x2.y();
		m_ATA.M56() -= x1.x() * x2.y();
		m_ATA.M57() -= x1.y() * x2.y();
		m_ATA.M66() = u1u1 * u2u2pv2v2 + m_ATA.M66();
		m_ATA.M67() = u1v1 * u2u2pv2v2 + m_ATA.M67();
		m_ATA.M77() = v1v1 * u2u2pv2v2 + m_ATA.M77();
		m_ATb.v0() = x1.x() * x2.x() + m_ATb.v0();
		m_ATb.v1() = x1.y() * x2.x() + m_ATb.v1();
		m_ATb.v2() = x2.x() + m_ATb.v2();
		m_ATb.v3() = x1.x() * x2.y() + m_ATb.v3();
		m_ATb.v4() = x1.y() * x2.y() + m_ATb.v4();
		m_ATb.v5() = x2.y() + m_ATb.v5();
		m_ATb.v6() -= x1.x() * u2u2pv2v2;
		m_ATb.v7() -= x1.y() * u2u2pv2v2;
	}
	m_ATA.M16() = m_ATA.M07();
	m_ATA.M33() = m_ATA.M00();
	m_ATA.M34() = m_ATA.M01();
	m_ATA.M35() = m_ATA.M02();
	m_ATA.M44() = m_ATA.M11();
	m_ATA.M45() = m_ATA.M12();
	if(!LA::SolveLinearSystemSymmetricUpper(m_ATA, m_ATb))
		return false;
	memcpy(&H.M00(), &m_ATb.v0(), 12);
	memcpy(&H.M10(), &m_ATb.v3(), 12);
	memcpy(&H.M20(), &m_ATb.v6(), 8);
	H.M22() = 1;
	return true;
}