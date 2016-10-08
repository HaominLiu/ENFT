
#ifndef _CAMERA_ARRAY_H_
#define _CAMERA_ARRAY_H_

#include "Camera.h"
#include "CameraArrayCalibrationParameter.h"

class CameraArray
{

public:

	inline const Camera& operator[] (const CameraIndex &iCam) const { return m_Cs[iCam]; }
	inline		 Camera& operator[] (const CameraIndex &iCam)		{ return m_Cs[iCam]; }
	//inline operator		Camera* const& ()		{ return m_Cs; }
	//inline operator const Camera* const& () const { return m_Cs; }
	inline const Camera* Data() const { return m_Cs; }
	inline		 Camera* Data()		  { return m_Cs; }
	inline const Camera& C(const CameraIndex &iCam) const { return m_Cs[iCam]; }
	inline		 Camera& C(const CameraIndex &iCam)		  { return m_Cs[iCam]; }

	inline void operator = (const CameraArray &C)
	{
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
			m_Cs[iCam] = C[iCam];
	}
	inline void ChangeReference(const RigidTransformation3D &T, __m128* const &work2)
	{
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
			m_Cs[iCam].ChangeReference(T, work2);
	}
	inline void MakeIdentity(const CameraArrayCalibrationParameter &calibParam)
	{
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
			m_Cs[iCam] = calibParam.GetRelativePose(iCam);
	}
	inline void Scale(const float &s)
	{
		for(CameraIndex iCam = 0; iCam < CAMERAS_NUMBER; ++iCam)
			m_Cs[iCam].Scale(s);
	}
	inline void FromFirstCamera(const CameraArrayCalibrationParameter &calibParam, __m128 &work)
	{
		for(CameraIndex iCam = 1; iCam < CAMERAS_NUMBER; ++iCam)
			RigidTransformation3D::AccumulateTransformation(m_Cs[0], calibParam.GetRelativePose(iCam), m_Cs[iCam], work);
	}
	inline void FromFirstCameraScaled(const CameraArrayCalibrationParameter &calibParam, __m128 &work)
	{
		for(CameraIndex iCam = 1; iCam < CAMERAS_NUMBER; ++iCam)
			RigidTransformation3D::AccumulateTransformation(m_Cs[0], calibParam.GetRelativePoseScaled(iCam), m_Cs[iCam], work);
	}
	inline void Print() const { m_Cs[0].Print(); }

private:

	Camera m_Cs[CAMERAS_NUMBER];

};

#endif