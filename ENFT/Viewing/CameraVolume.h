
#ifndef _CAMERA_VOLUME_H_
#define _CAMERA_VOLUME_H_

#include "Camera.h"
#include <GL/glew.h>
#include <GL/glut.h>

class CameraVolume
{

public:

	inline void Initialize(const float &zPlane, const float &xzRatio, const float &yzRatio, const float &scale)
	{
		m_Xc.Set(zPlane * xzRatio, zPlane * yzRatio, zPlane);
		m_Xc.reserve() = 1.0f;
		m_XcScaled = m_Xc;
	}
	inline void SetScale(const float &scale)
	{
		m_XcScaled.XYZx() = _mm_mul_ps(_mm_set1_ps(scale), m_Xc.XYZx());
		m_XcScaled.reserve() = 1.0f;
	}
	inline void DrawVolume(const Camera &C)
	{
		C.GetCenter(m_Xws[0]);

		// Xw = R^T * (Xc - t) = R^T * Xc + Cc
		__m128 &XcR0 = m_work[0], &YcR1 = m_work[1], &ZcR2 = m_work[2];
		XcR0 = _mm_mul_ps(_mm_set1_ps(m_XcScaled.X()), C.r00_r01_r02_tX());
		YcR1 = _mm_mul_ps(_mm_set1_ps(m_XcScaled.Y()), C.r10_r11_r12_tY());
		ZcR2 = _mm_mul_ps(_mm_set1_ps(m_XcScaled.Z()), C.r20_r21_r22_tZ());


		// 4 plane vertexes' world coordinate are: X[R0] + Y[R1] + Z[R2] + Cc,   X[R0] - Y[R1] + Z[R2] + Cc, 
		//										 - X[R0] + Y[R1] + Z[R2] + Cc, - X[R0] - Y[R1] + Z[R2] + Cc, 
		ZcR2 = _mm_add_ps(ZcR2, m_Xws[0].XYZx());			//   Z[R2] + Cc
		m_Xws[1].XYZx() = XcR0;
		XcR0 = _mm_add_ps(XcR0, YcR1);						//   X[R0] + Y[R1]
		YcR1 = _mm_sub_ps(m_Xws[1].XYZx(), YcR1);			//   X[R0] - Y[R1]
		m_Xws[1].XYZx() = _mm_add_ps(ZcR2, XcR0);			//   X[R0] + Y[R1] + Z[R2] + Cc
		m_Xws[2].XYZx() = _mm_add_ps(ZcR2, YcR1);			//   X[R0] - Y[R1] + Z[R2] + Cc
		m_Xws[3].XYZx() = _mm_sub_ps(ZcR2, YcR1);			// - X[R0] + Y[R1] + Z[R2] + Cc
		m_Xws[4].XYZx() = _mm_sub_ps(ZcR2, XcR0);			// - X[R0] - Y[R1] + Z[R2] + Cc

		// [0;0;0], [X;Y;Z], [X;-Y;Z], [-X;Y;Z], [-X;-Y;Z]
		glBegin(GL_LINE_LOOP);
		glVertex3fv(m_Xws[0]);
		glVertex3fv(m_Xws[1]);
		glVertex3fv(m_Xws[3]);
		glVertex3fv(m_Xws[4]);
		glVertex3fv(m_Xws[2]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3fv(m_Xws[0]);
		glVertex3fv(m_Xws[3]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3fv(m_Xws[0]);
		glVertex3fv(m_Xws[4]);
		glEnd();
		glBegin(GL_LINES);
		glVertex3fv(m_Xws[1]);
		glVertex3fv(m_Xws[2]);
		glEnd();

		//Point axisX(m_Xc.m128_f32[0], 0, 0), axisY(0, m_Yc.m128_f32[0], 0), axisZ(0, 0, m_Zc.m128_f32[0]);
		//C.ApplyInversely(axisX);
		//C.ApplyInversely(axisY);
		//C.ApplyInversely(axisZ);
		//glColor3ub(255, 0, 0);	glBegin(GL_LINES);	glVertex3fv(m_Xws[0]);	glVertex3fv(axisX);	glEnd();
		//glColor3ub(0, 255, 0);	glBegin(GL_LINES);	glVertex3fv(m_Xws[0]);	glVertex3fv(axisY);	glEnd();
		//glColor3ub(0, 0, 255);	glBegin(GL_LINES);	glVertex3fv(m_Xws[0]);	glVertex3fv(axisZ);	glEnd();
	}

	//inline void DrawCenter(const Camera &C)
	//{
	//	C.GetCenter(m_Xws[0]);
	//	glVertex3fv(m_Xws[0]);
	//}

private:

	Point3D m_Xc, m_XcScaled, m_Xws[5];
	__m128 m_work[3];

};

#endif