#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GL_includes.h"

#include "Shader.h"

class ShaderProgram
{

    GLuint    _id;
    GLint  _matID;

public:

    ShaderProgram() : _id { glCreateProgram() }, _matID { -1 } {};

    ShaderProgram( const ShaderProgram  & ) = delete;
    ShaderProgram(       ShaderProgram && );

    ~ShaderProgram();

    ShaderProgram &operator=( const ShaderProgram  & ) = delete;
    ShaderProgram &operator=( ShaderProgram       && );

    void         attach_shader( Shader && ) const;
    void         attach_shader( Shader  & ) const;

    void         link()  throw( const char * );

    void         use()   const;
    const GLuint id()    const;
    const GLuint matID() const;

};

#endif