#ifndef SHADER_H
#define SHADER_H

#include <iostream>

#include "GL_includes.h"

namespace engine {

class Shader {

    GLuint     _id;
    GLenum _target;

protected:

    static void read( const char *, std::string & ) throw ( const char * );

public:

    Shader( GLenum );
    Shader( GLenum,         const char * );
    Shader( GLenum, GLuint, const char * );

    Shader( const Shader  & ) = delete;
    Shader(       Shader && );

    ~Shader();

    Shader &operator=( const Shader  & ) = delete;
    Shader &operator=(       Shader && );

    void compile( const char * ) const              throw ( const char * );
    void attach(        GLuint ) const;

};

}; // namespace engine

#endif