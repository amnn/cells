#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "GL_includes.h"

#include "texture/Texture.h"

class Texture2D : public Texture {

	GLsizei _w, _h;

public:
	
	Texture2D
	(

	    GLenum target,
	    GLint  levels,
	    GLint     fmt,
	    GLsizei     w,
	    GLsizei     h

	) 
	: Texture( target, fmt ), _w { w }, _h { h }
	{ glTexStorage2D( target, levels, fmt, w, h ); }

	template <class S>
	Texture2D
	(
	    GLenum target,
	    GLint  levels,
	    GLint     fmt,
	    GLsizei     w,
	    GLsizei     h,
	    GLenum   dFmt,
	    GLenum   type,
	    S       *data

	)
	: Texture2D( target, levels, fmt, w, h )
	{ image( 0, dFmt, type, data ); }

	template <class S>
	void image
	(

	    GLint level,
	    GLenum dFmt, 
	    GLenum type,
	    S     *data

	)
	{ glTexImage2D( _target, level, _fmt, _w, _h, 0, dFmt, type, data ); }

	template <class S>
	void sub_image
	(

	    GLint level,
	    GLint     x,
	    GLint     y,
	    GLsizei   w,
	    GLsizei   h,
	    GLenum dFmt,
	    GLenum type,
	    S     *data

	)
	{ glTexSubImage2D( _target, level, x, y, w, h, dFmt, type, data ); }

};

#endif
