#ifndef TEXTURE_H
#define TEXTURE_H

#include <unordered_map>

class Texture
{

    static std::unordered_map< GLenum, GLuint > _bindings;

    GLuint     _id;
    GLenum _target;

    Texture( GLenum );

public:

    ~Texture();

    Texture( Texture && );
    Texture( Texture  & ) = delete;

    Texture &operator=( Texture && );
    Texture &operator=( Texture  & ) = delete;

    void   param( GLenum,   GLint ) const;
    void   param( GLenum, GLfloat ) const;

    void   bind()                   const;
    GLenum target()                 const;

};

#endif