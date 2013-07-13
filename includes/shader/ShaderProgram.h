#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <unordered_map>

#include "GL_includes.h"

#include "shader/Shader.h"

namespace engine {

class ShaderProgram
{

    GLuint    _id;

    std::unordered_map< std::string, GLint > _syms;

public:

    ShaderProgram() : _id { glCreateProgram() } {};

    ShaderProgram( const ShaderProgram  & ) = delete;
    ShaderProgram(       ShaderProgram && );

    ~ShaderProgram();

    ShaderProgram &operator=(  const ShaderProgram  & ) = delete;
    ShaderProgram &operator=(        ShaderProgram && );

    GLint          operator[]( const std::string    & ) const;
    void           attrib(     const std::string    & );
    void           uniform(    const std::string    & );

    void           attach_shader(           Shader && ) const;
    void           attach_shader(           Shader  & ) const;

    void           link()               throw( const char * );

    void           use()                                const;
    const GLuint   id()                                 const;

};

}; // namespace engine

#endif