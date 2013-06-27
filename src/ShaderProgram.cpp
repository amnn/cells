#include <string>
#include <fstream>

#include "GL_includes.h"

#include "Shader.h"

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram( const char * vs_file, const char * fs_file ) throw( char const * )
{

    _id = glCreateProgram();

    Shader( GL_VERTEX_SHADER,   _id, vs_file );
    Shader( GL_FRAGMENT_SHADER, _id, fs_file );

    glLinkProgram(    _id );
    
    GLint res;
    glGetProgramiv( _id, GL_LINK_STATUS, &res );

    if( res != GL_TRUE )
    {
        int logLength;

        glGetProgramiv(      _id, GL_INFO_LOG_LENGTH, &logLength );

        char *msg = new char[logLength];
        
        glGetProgramInfoLog( _id, logLength, NULL,           msg );
        
        throw( msg );

    }

    _matID = glGetUniformLocation( _id, "MVP" );

    if( _matID == -1 ) throw( "No MVP Uniform in shader!" );

}

ShaderProgram::ShaderProgram( ShaderProgram &&that )
{

    std::swap( _id,       that._id );
    std::swap( _matID, that._matID );

}

ShaderProgram &ShaderProgram::operator=( ShaderProgram &&that )
{

    std::swap( _id,       that._id );
    std::swap( _matID, that._matID );
    
    return                     *this;
}

ShaderProgram::~ShaderProgram()           { glDeleteProgram( _id ); }

void ShaderProgram::use()           const { glUseProgram(    _id ); }

const GLuint ShaderProgram::id()    const {             return _id; }

const GLuint ShaderProgram::matID() const {          return _matID; }