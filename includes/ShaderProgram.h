#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "GL_includes.h"

class ShaderProgram
{

    GLuint    _id;
    GLint  _matID;

protected:

    static void read( const char *, std::string & )    throw( const char * );
    static void compile_shader( GLuint, const char * ) throw( const char * );

public:

    ShaderProgram() : _id { 0 }, _matID { -1 } {};
    ShaderProgram( const char *, const char * ) throw( char const * );

    ShaderProgram( const ShaderProgram  & ) = delete;
    ShaderProgram(       ShaderProgram && );

    ~ShaderProgram();

    ShaderProgram &operator=( const ShaderProgram  & ) = delete;
    ShaderProgram &operator=( ShaderProgram       && );

    void         use()   const;
    const GLuint id()    const;
    const GLuint matID() const;

};

#endif