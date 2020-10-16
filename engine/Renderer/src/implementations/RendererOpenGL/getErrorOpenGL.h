
#ifndef GETERROROPENGL_H
#define GETERROROPENGL_H

inline void getErrorOpenGL()
{
	GLenum error = glGetError();
	switch (error)
	{
	case GL_NO_ERROR:
		break;
	case GL_INVALID_ENUM:
		std::cout << "OpenGL ERROR: " << "GL_INVALID_ENUM" << std::endl;
		break;
	case GL_INVALID_VALUE:
		std::cout << "OpenGL ERROR: " << "GL_INVALID_VALUE" << std::endl;
		break;
	case GL_INVALID_OPERATION:
		std::cout << "OpenGL ERROR: " << "GL_INVALID_OPERATION" << std::endl;
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "OpenGL ERROR: " << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
		break;
	case GL_OUT_OF_MEMORY:
		std::cout << "OpenGL ERROR: " << "GL_OUT_OF_MEMORY" << std::endl;
		break;
	}

}

#endif