
#include "Vector4.h"
#include "Matrix3.h"

template<> inline void ProgramGL::SetInputParameter<int>(const int &param, const GLint &loc) const
{
#if _DEBUG
	assert(loc != -1);
#endif
	glUniform1i(loc, param);
}

template<> inline void ProgramGL::SetInputParameter<float>(const float &param, const GLint &loc) const
{
#if _DEBUG
	assert(loc != -1);
#endif
	glUniform1f(loc, param);
}

template<> inline void ProgramGL::SetInputParameter<LA::Vector2f>(const LA::Vector2f &param, const GLint &loc) const
{
#if _DEBUG
	assert(loc != -1);
#endif
	glUniform2fv(loc, 1, param);
}

template<> inline void ProgramGL::SetInputParameter<LA::Vector4f>(const LA::Vector4f &param, const GLint &loc) const
{
#if _DEBUG
	assert(loc != -1);
#endif
	glUniform4fv(loc, 1, param);
}

template<> inline void ProgramGL::SetInputParameter<LA::Matrix3f>(const LA::Matrix3f &param, const GLint &loc) const
{
#if _DEBUG
	assert(loc != -1);
#endif
	glUniformMatrix3fv(loc, 1, GL_TRUE, param);
}