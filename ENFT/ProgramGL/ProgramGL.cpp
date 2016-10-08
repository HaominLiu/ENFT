
#include "stdafx.h"
#include "ProgramGL.h"

const GLenum ProgramGL::g_attachments[8] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, 
											GL_COLOR_ATTACHMENT4_EXT, GL_COLOR_ATTACHMENT5_EXT, GL_COLOR_ATTACHMENT6_EXT, GL_COLOR_ATTACHMENT7_EXT};
CVD::GLWindow* ProgramGL::g_pWnd = NULL;
GLuint ProgramGL::g_frameBuffer = 0;
GLint ProgramGL::g_drawBuffer = 0;
GLint ProgramGL::g_readBuffer = 0;

void ProgramGL::Initialize(const ushort &width, const ushort &height)
{
	if(g_pWnd)
	{
		if(g_pWnd->size().x == 0 || g_pWnd->size().y == 0)
			g_pWnd->set_size(CVD::ImageRef(int(width), int(height)));
		//printf("(%d, %d)\n", g_pWnd->size().x, g_pWnd->size().y);
		return;
	}
	g_pWnd = new CVD::GLWindow(CVD::ImageRef(int(width), int(height)));
	g_pWnd->set_position(CVD::ImageRef((GetSystemMetrics(SM_CXSCREEN) - width) / 2, (GetSystemMetrics(SM_CYSCREEN) - height) / 2));
	
	glewInit();
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	//glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if(g_frameBuffer == 0)
		glGenFramebuffersEXT(1, &g_frameBuffer);
	g_drawBuffer = 0;
	g_readBuffer = 0;

	BindFrameBuffer();
}

void ProgramGL::BindFrameBuffer()
{
#if _DEBUG
	assert(g_frameBuffer != 0);
	AssertFrameBufferUnbound();
#endif

	glGetIntegerv(GL_DRAW_BUFFER, &g_drawBuffer);
	glGetIntegerv(GL_READ_BUFFER, &g_readBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_frameBuffer);
}

void ProgramGL::UnbindFrameBuffer()
{
#if _DEBUG
	assert(g_frameBuffer != 0);
	AssertFrameBufferBound();
	assert(g_drawBuffer != 0 && g_readBuffer != 0);
#endif

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	GLint currentDrawBuffer, currentReadBuffer;
	glGetIntegerv(GL_DRAW_BUFFER, &currentDrawBuffer);
	if(currentDrawBuffer != g_drawBuffer)
		glDrawBuffer(g_drawBuffer);
	glGetIntegerv(GL_READ_BUFFER, &currentReadBuffer);
	if(currentReadBuffer != g_readBuffer)
		glReadBuffer(g_readBuffer);
}

#if _DEBUG
void ProgramGL::AssertFrameBufferBound()
{
	assert(g_frameBuffer != 0);
	GLint boundFrameBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFrameBuffer);
	assert(boundFrameBuffer == g_frameBuffer);
}

void ProgramGL::AssertFrameBufferUnbound()
{
	GLint boundFrameBuffer;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFrameBuffer);
	assert(boundFrameBuffer == 0);
}

void ProgramGL::AssertAttachmentAvailable(const GLenum &attachment)
{
	AssertFrameBufferBound();
	GLint attachedTexture;
	glGetFramebufferAttachmentParameterivEXT(GL_FRAMEBUFFER_EXT, attachment, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &attachedTexture);
	assert(attachedTexture == 0);
}
#endif