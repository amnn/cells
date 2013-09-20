#include <string>
#include <fstream>
#include <unordered_map>
#include <stdexcept>

#include "GL_includes.h"

#include "shader/Shader.h"

#include "shader/ShaderProgram.h"

namespace Engine {

ShaderProgram::ShaderProgram(ShaderProgram &&that)
{
    std::swap(_id,     that._id);
    std::swap(_syms, that._syms);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(_id); }

ShaderProgram &
ShaderProgram::operator=(ShaderProgram &&that)
{

    std::swap(_id,     that._id);
    std::swap(_syms, that._syms);

    return                 *this;
}

GLint
ShaderProgram::operator[](const std::string & key) const
{
    try                               { return _syms.at(key); }
    catch(const std::out_of_range &e) { return            -1; }
}

void
ShaderProgram::attrib(const std::string &sym)
{
    _syms[sym] = glGetAttribLocation(_id, sym.c_str() );
}

void ShaderProgram::uniform(const std::string &sym)
{
    _syms[sym] = glGetUniformLocation(_id, sym.c_str() );
}

void ShaderProgram::attach_shader(Shader &&s) const { s.attach(_id); }
void ShaderProgram::attach_shader(Shader  &s) const { s.attach(_id); }

void
ShaderProgram::link()
throw(const char *)
{
    glLinkProgram(_id);

    GLint res;
    glGetProgramiv(_id, GL_LINK_STATUS, &res);

    if(res != GL_TRUE)
    {
        int logLength;
        glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logLength);

        char *msg = new char[logLength];
        glGetProgramInfoLog(_id, logLength, NULL, msg);

        throw(msg);
    }
}

void
ShaderProgram::use() const
{
    if(_usedProg == _id) return;

    _usedProg =  _id;
    glUseProgram(_id);
}

const GLuint
ShaderProgram::id()  const {        return _id; }

GLuint ShaderProgram::_usedProg { 0 };

}; // namespace Engine
