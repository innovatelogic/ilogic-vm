#include "RenderContext.h"
#include "RenderTargetNode.h"

namespace RenderDriver
{
	RenderTargetNode::RenderTargetNode(size_t width, size_t height, SRenderContext *const pCtxt)
	: m_pContextOwner(pCtxt)
	{
		const size_t bpp = 4;
		const size_t size = (width * height) * bpp * sizeof(unsigned int);

		// Create Storage Space For Texture Data (128x128x4)
		unsigned int *data = (unsigned int*)new GLuint[size];

		ZeroMemory(data, size);   // Clear Storage Memory

		m_target.width = width;
		m_target.height = height;

		glGenTextures(1, &m_target.texture);                // Create 1 Texture
		glBindTexture(GL_TEXTURE_2D, m_target.texture);     // Bind The Texture
		glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); // Build Texture Using Information In data
			           
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		delete [] data; // Release data
	}

	//----------------------------------------------------------------------------------------------
	RenderTargetNode::~RenderTargetNode()
	{

	}

	//----------------------------------------------------------------------------------------------
	void RenderTargetNode::DoRelease()
	{
		glDeleteTextures(1, &m_target.texture);

		Refcount::DoRelease();
	}
}