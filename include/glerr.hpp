#pragma once
#include <cstdio>
#include <axl.glw/gl.hpp>

namespace glerr {

axl::glw::gl::GLenum __gl_error__ = 0;

#ifdef DEBUG
#	define GLE(X) axl::glw::gl::glGetError(); X; if((__gl_error__ = axl::glw::gl::glGetError())) fprintf(stderr, "> GLerr::%s:%d -- %u:%s\n", __FILE__, __LINE__, glerr::__gl_error__, glerr::glErrString(glerr::__gl_error__))
#else
#	define GLE(X) X
#endif

const char* glErrString(axl::glw::gl::GLenum error)
{
	using  namespace axl::glw::gl;
	switch(error)
	{
		case GL_INVALID_ENUM: return "invalid_enum";
		case GL_INVALID_VALUE: return "invalid_value";
		case GL_INVALID_OPERATION: return "invalid_operation";
		case GL_STACK_OVERFLOW: return "stack_overflow";
		case GL_STACK_UNDERFLOW: return "stack_underflow";
		case GL_OUT_OF_MEMORY: return "out_of_memory";
		default: return "other error";
	}
}

} // namespace glerr
