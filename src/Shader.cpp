#include <string>
#include <fstream>
#include <iostream>

#include "GL_includes.h"

#include "shader/Shader.h"

namespace engine {

void
Shader::read(const char * loc, std::string & buff)
throw(const char *)
{
    std::ifstream in(loc, std::ios::in);

    if(in)
    {
        in.seekg(0, std::ios::end);
        buff.resize(   in.tellg());
        in.seekg(0, std::ios::beg);

        in.read(&buff[0], buff.size());

        in.close();

        return;
    } else throw("Could not read Shader!");
}

Shader::Shader(GLenum target)
: _target                 { target },
  _id     { glCreateShader(target) }
                                  {}

Shader::Shader(GLenum target, const char * loc)
: Shader(target)
{ compile(loc); }

Shader::Shader(GLenum target, GLuint prog, const char * loc)
: Shader(target, loc)
{ attach(prog); }

Shader::~Shader() { glDeleteShader(_id); }

Shader &
Shader::operator=(Shader && that)
{
    std::swap(_id,         that._id);
    std::swap(_target, that._target);
    return                     *this;
}

void
Shader::compile(const char * loc) const
throw(const char *)
{
    std::string src; GLint res;
    Shader::read(    loc, src);

    const char * srcRaw = src.c_str();

    glShaderSource(_id,        1, &srcRaw, NULL);

    glCompileShader(_id);

    glGetShaderiv( _id, GL_COMPILE_STATUS, &res);

    if(res != GL_TRUE)
    {
        int logLength;
        glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &logLength);

        char *msg = new char[logLength];
        glGetShaderInfoLog(     _id, logLength, NULL, msg);

        throw(msg);
    }
}

void
Shader::attach(GLuint prog) const { glAttachShader(prog, _id); }

}; // namespace engine
