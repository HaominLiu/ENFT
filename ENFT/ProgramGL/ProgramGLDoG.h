
#ifndef _PROGRAM_GL_DOG_H_
#define _PROGRAM_GL_DOG_H_

#include "ProgramGL.h"

class ProgramGLDoG : public ProgramGL
{

public:

	inline void Initialize()
	{
		std::vector<std::string> inpTexNames(2), inpParamNames;
		inpTexNames[0] = "g_gaussTex0";
		inpTexNames[1] = "g_gaussTex1";
		LoadSource(
			"uniform sampler2DRect g_gaussTex0;\n"
			"uniform sampler2DRect g_gaussTex1;\n"
			"void main()\n"
			"{\n"
			"	vec2 coord = gl_FragCoord.st;\n"
			"	gl_FragColor = texture2DRect(g_gaussTex1, coord) - texture2DRect(g_gaussTex0, coord);\n"
			"	gl_FragDepth = 1;\n"
			"}\n", inpTexNames, inpParamNames);
	}
	inline void Run(const TextureGL4 &gaussTex0, const TextureGL4 &gaussTex1, const TextureGL4 &dogTex) const
	{
#if _DEBUG
		assert(gaussTex0.GetWidth() == gaussTex1.GetWidth() && gaussTex0.GetHeight() == gaussTex1.GetHeight());
		assert(gaussTex0.GetWidth() <= dogTex.GetWidth() && gaussTex0.GetHeight() <= dogTex.GetHeight());
#endif
		Activate();
		SetInputTextures(gaussTex0, gaussTex1);
		SetOutputTexture(dogTex);
		DrawQuad(gaussTex0.GetWidth(), gaussTex0.GetHeight());
		Deactivate();
		UnbindInputTextures(gaussTex0, gaussTex1);
	}

};

#endif