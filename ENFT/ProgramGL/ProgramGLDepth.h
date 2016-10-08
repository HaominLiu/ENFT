
#ifndef _PROGRAM_GL_DEPTH_H_
#define _PROGRAM_GL_DEPTH_H_

#include "IntrinsicMatrix.h"

class ProgramGLComputeLocalCoordinate : public ProgramGL
{

public:

	inline void Initialize(const IntrinsicMatrix &K)
	{
		char source[MAX_SOURCE_LENGTH];
		std::ostrstream out(source, MAX_SOURCE_LENGTH);
		out <<
			"uniform sampler2DRect g_depTex;\n"
			"void main()\n"
			"{\n"
			"	vec2 x = floor(gl_FragCoord.st);\n"
			"	vec2 xn = (x - vec2(" << K.cx() << ", " << K.cy() << ")) * vec2(" << K.one_over_fx() << ", " << K.one_over_fy() << ");\n"
			"	float d = texture2DRect(g_depTex, gl_FragCoord.st).r;\n"
			"	gl_FragColor.rgb = vec3(xn * d, d);\n"
			"}\n" << '\0';

		std::vector<std::string> inpTexNames(1), inpParamNames;
		inpTexNames[0] = "g_depTex";
		LoadSource(source, inpTexNames, inpParamNames);
	}

	inline void Run(const TextureGL1 &depTex, const TextureGL3 &localCoordTex) const
	{
#if _DEBUG
		assert(depTex.GetWidth() == localCoordTex.GetWidth() && depTex.GetHeight() == localCoordTex.GetHeight());
#endif
		Activate();
		SetInputTexture(depTex);
		SetOutputTexture(localCoordTex);
		DrawQuad(localCoordTex.GetWidth(), localCoordTex.GetHeight());
		Deactivate();
	}
};

#endif