#include <utility>
#include <unordered_map>

#include "GL_includes.h"

#include "Texture.h"

std::unordered_map< GLenum, GLuint > Texture::_bindings {};

Texture::Texture( GLenum target, GLint fmt ) 
: _target { target }, _fmt { fmt }
{ glGenTextures( 1, &_id ); }

Texture::Texture( GLenum target ) : Texture( target, 0 ) {}

Texture::~Texture() { glDeleteTextures( 1, &_id ); }

Texture::Texture( Texture &&that )
{
    std::swap( _id,         that._id );
    std::swap( _target, that._target );  
}

Texture &Texture::operator=( Texture &&that )
{
    std::swap( _id,         that._id );
    std::swap( _target, that._target );  

    return *this;    
}

void Texture::param( GLenum pname, GLint   p ) const { glTexParameteri( _target, pname, p ); }
void Texture::param( GLenum pname, GLfloat p ) const { glTexParameterf( _target, pname, p ); }

void Texture::mip_maps()                       const { glGenerateMipmap(          _target ); }

void Texture::bind() const { 
    
    GLuint & current = _bindings[ _target ];

    if( current != _id ) {
        
        glBindTexture( _target, _id );
        current =               _id  ;

    }


}

GLenum Texture::target() const { return _target; };

/* Texture1D */

Texture1D::Texture1D
(

    GLenum target,
    GLint  levels,
    GLint     fmt,
    GLsizei     w

) 
: Texture( target, fmt ), _w { w } 
{ glTexStorage1D( target, levels, fmt, w ); }

template <class S>
Texture1D::Texture1D
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
void Texture1D::image
(

    GLint level,
    GLenum dFmt, 
    GLenum type,
    S     *data

)
{ glTexImage1D( _target, level, _fmt, _w, 0, dFmt, type, data ); }

template <class S>
void Texture1D::sub_image
(

    GLint level,
    GLint     x,
    GLsizei   w,
    GLenum dFmt,
    GLenum type,
    S     *data

)
{ glTexSubImage1D( _target, level, x, w, dFmt, type, data ); }

/* Texture2D */

Texture2D::Texture2D
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
Texture2D::Texture2D
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
void Texture2D::image
(

    GLint level,
    GLenum dFmt, 
    GLenum type,
    S     *data

)
{ glTexImage2D( _target, level, _fmt, _w, _h, 0, dFmt, type, data ); }

template <class S>
void Texture2D::sub_image
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

/* Texture3D */

Texture3D::Texture3D
(

    GLenum target,
    GLint  levels,
    GLint     fmt,
    GLsizei     w,
    GLsizei     h,
    GLsizei     d

) 
: Texture( target, fmt ), _w { w }, _h { h }, _d { d }
{ glTexStorage3D( target, levels, fmt, w, h, d ); }

template <class S>
Texture3D::Texture3D
(
    GLenum target,
    GLint  levels,
    GLint     fmt,
    GLsizei     w,
    GLsizei     h,
    GLsizei     d,
    GLenum   dFmt,
    GLenum   type,
    S       *data

)
: Texture3D( target, levels, fmt, w, h, d )
{ image( 0, dFmt, type, data ); }

template <class S>
void Texture3D::image
(

    GLint level,
    GLenum dFmt, 
    GLenum type,
    S     *data

)
{ glTexImage3D( _target, level, _fmt, _w, _h, _d, 0, dFmt, type, data ); }

template <class S>
void Texture3D::sub_image
(

    GLint level,
    GLint     x,
    GLint     y,
    GLint     z,
    GLsizei   w,
    GLsizei   h,
    GLsizei   d,
    GLenum dFmt,
    GLenum type,
    S     *data

)
{ glTexSubImage3D( _target, level, x, y, z, w, h, d, dFmt, type, data ); }
