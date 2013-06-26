#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{

    GLuint     _id;
    GLenum _target;

    Texture( GLenum );

public:

    Texture( GLenum, const char * ) throw( const char * );
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