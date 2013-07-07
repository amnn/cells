#ifndef TEXTURE_H
#define TEXTURE_H

#include <unordered_map>

class Texture
{

    static std::unordered_map< GLenum, GLuint > _bindings;

    GLuint     _id;

    Texture( GLenum );

public:

    ~Texture();

    Texture( Texture && );
    Texture( Texture  & ) = delete;

    Texture &operator=( Texture && );
    Texture &operator=( Texture  & ) = delete;

    void   param( GLenum,   GLint ) const;
    void   param( GLenum, GLfloat ) const;

    void   mip_maps()               const;

    void   bind()                   const;
    GLenum target()                 const;
    GLuint id()                     const;

protected:

    GLenum _target;
    GLint     _fmt;

    Texture( GLenum, GLint );

};

#include "Texture1D.h"
#include "Texture2D.h"
#include "Texture3D.h"

#endif