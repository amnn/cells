#ifndef TEXTURE3D_H
#define TEXTURE3D_H

#include "GL_includes.h"

#include "texture/Texture.h"

namespace Engine {

class Texture3D : public Texture {

	GLsizei _w, _h, _d;

public:

	Texture3D
	(
		GLenum   target,
		GLint    levels,
		GLint    fmt,
		GLsizei  w,
		GLsizei  h,
		GLsizei  d
	)
		: Texture(target, fmt)
		, _w   	 { w }
		, _h  	 { h }
		, _d     { d }
	{
		glTexStorage3D(target, levels, fmt, w, h, d);
	}

	template <class S>
	Texture3D
	(
		GLenum   target,
		GLint    levels,
		GLint    fmt,
		GLsizei  w,
		GLsizei  h,
		GLsizei  d,
		GLenum   dFmt,
		GLenum   type,
		S *      data
	)
		: Texture3D(target, levels, fmt, w, h, d)
	{
		image(0, dFmt, type, data);
	}

	template <class S>
	void
	image
	(
		GLint   level,
		GLenum  dFmt,
		GLenum  type,
		S *     data
	)
	{ glTexImage3D(_target, level, _fmt, _w, _h, _d, 0, dFmt, type, data); }

	template <class S>
	void
	sub_image
	(
		GLint    level,
		GLint    x,
		GLint    y,
		GLint    z,
		GLsizei  w,
		GLsizei  h,
		GLsizei  d,
		GLenum   dFmt,
		GLenum   type,
		S *      data
	)
	{ glTexSubImage3D(_target, level, x, y, z, w, h, d, dFmt, type, data); }
};

}; // namespace Engine

#endif
