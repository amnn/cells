#ifndef TEXTURE1D_H
#define TEXTURE1D_H

#include "GL_includes.h"

#include "Texture.h"

class Texture1D : public Texture {

    GLsizei _w;

public:

    Texture1D
    (

        GLenum target,
        GLint  levels,
        GLint     fmt,
        GLsizei     w

    ) 
    : Texture( target, fmt ), _w { w } 
    { bind(); glTexStorage1D( target, levels, fmt, w ); }

    template <class S>
    Texture1D
    (
        GLenum target,
        GLint  levels,
        GLint     fmt,
        GLsizei     w,
        GLenum   dFmt,
        GLenum   type,
        S       *data

    )
    : Texture1D( target, levels, fmt, w )
    { image( 0, dFmt, type, data ); }

    template <class S>
    void image
    (

        GLint level,
        GLenum dFmt, 
        GLenum type,
        S     *data

    )
    { glTexImage1D( _target, level, _fmt, _w, 0, dFmt, type, data ); }

    template <class S>
    void sub_image
    (

        GLint level,
        GLint     x,
        GLsizei   w,
        GLenum dFmt,
        GLenum type,
        S     *data

    )
    { glTexSubImage1D( _target, level, x, w, dFmt, type, data ); }

};

#endif