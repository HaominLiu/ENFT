

#ifndef PROGRAM_GL_DESCRIPTOR_COPY_H_
#define PROGRAM_GL_DESCRIPTOR_COPY_H_

#include "ProgramGL.h"

class ProgramGLDescriptorCopy : public ProgramGL
{

public:

	inline void Initialize(const ushort &descTexWidth)
	{
		m_descTexWidth = descTexWidth;
		m_descTexWidthLog = ushort(log(float(descTexWidth)) / log(2.0f));
#if _DEBUG
		assert((1 << m_descTexWidthLog) == descTexWidth);
#endif

		char source[MAX_SOURCE_LENGTH];
		std::ostrstream out(source, MAX_SOURCE_LENGTH);
		out <<
			"uniform sampler2DRect g_srcDescTex;\n"
			"void main()\n"
			"{\n"
			"	gl_FragColor = texture2DRect(g_srcDescTex, gl_FragCoord.st);\n"
			"}\n" << '\0';
		std::vector<std::string> inpTexNames(1), inpParamNames;
		inpTexNames[0] = "g_srcDescTex";
		LoadSource(source, inpTexNames, inpParamNames);
	}
	inline void Run(const TextureGL4 &srcDescTex, const TextureGL4 &dstDescTex, const ushort &nFtrs) const
	{
		if(srcDescTex.GetTexture() == dstDescTex.GetTexture())
			return;

#if _DEBUG
		assert(srcDescTex.GetWidth() == m_descTexWidth && dstDescTex.GetWidth() == m_descTexWidth);
#endif

		Activate();
		SetInputTexture(srcDescTex);
		SetOutputTexture(dstDescTex);
		DrawQuad(uint(nFtrs) << 4, m_descTexWidth, m_descTexWidthLog);
		Deactivate();
	}

private:

	ushort m_descTexWidth, m_descTexWidthLog;

};

#endif