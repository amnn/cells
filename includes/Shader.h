#ifndef SHADER_H
#define SHADER_H

#include "GL_includes.h"

class Shader {

    GLuint     _id;
    GLenum _target;

protected:

    static void read( const char *, std::string & ) throw ( const char * );

public:

    Shader( GLenum );
    Shader( GLenum, GLuint, const char * )          throw ( const char * );
    Shader( GLenum, const char * )                  throw ( const char * );

    Shader( const Shader  & ) = delete;
    Shader(       Shader && );

    ~Shader();

    Shader &operator=( const Shader  & ) = delete;
    Shader &operator=(       Shader && );

    void compile( const char * ) const              throw ( const char * );
    void attach(        GLuint ) const;

};

#endif