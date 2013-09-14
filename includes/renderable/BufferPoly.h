#ifndef BUFFERPOLY_H
#define BUFFERPOLY_H

#include <memory>
#include <vector>

#include "GL_includes.h"
#include "glm/glm.hpp"

#include "buffer/Buffer.h"
#include "renderable/Renderable.h"
#include "renderable/RenderEngine.h"
#include "shader/ShaderProgram.h"
#include "texture/Texture.h"

namespace engine {

class BufferPoly
{
    static GLuint                   _currentBinding;

    GLuint                                   _vaoID;
    GLenum                                     _fmt;
    std::vector< std::shared_ptr<void> > _tiedBuffs;

    GLsizei                                _indices;
    std::shared_ptr<void>                   _elBuff;
    GLenum                                   _elFmt;
    GLuint                                   _elOff;

public:

    class Instance : public Renderable {

        std::shared_ptr<BufferPoly> _poly;

    public:

        Instance(std::shared_ptr<BufferPoly>  & poly)
        : Renderable(),           _poly { poly } {};

        Instance(              const Instance & that)
        : Renderable(that), _poly { that._poly } {};

        Instance(                   Instance && that)
        : Renderable(that)
        {
            std::swap(_poly, that._poly);
        };

        virtual void
        render(const ShaderProgram & p, const glm::mat4 & m) const
        {
            _poly->bind();
            glm::mat4 trn = m * _local;

            glUniformMatrix4fv(p["MVP"], 1, GL_FALSE, &trn[0][0]);

            if(_poly->_elFmt)
            {
                glDrawElements(
                    _poly->_fmt,
                    _poly->_indices,
                    _poly->_elFmt,
                    reinterpret_cast<void *>(_poly->_elOff)
                );
            } else {
                glDrawArrays(
                    _poly->_fmt,
                    _poly->_elOff,
                    _poly->_indices
                );
            }
        };
    };

    class TextureInstance : public Instance {

        std::shared_ptr<Texture> _tex;

    public:

        TextureInstance(std::shared_ptr<BufferPoly>  & poly,
                        std::shared_ptr<   Texture>  &  tex)
        : Instance(poly),                   _tex { tex } {};

        TextureInstance(       const TextureInstance & that)
        : Instance(that),             _tex { that._tex } {};

        TextureInstance(            TextureInstance && that)
        : Instance(that)
        {
            std::swap(_tex, that._tex);
        };

        virtual void
        render(const ShaderProgram & p, const glm::mat4 & m) const
        {
            _tex->bind(); Instance::render(p, m);
        }

    };

    template <class LFun, class Scr>
    BufferPoly
    (
          const RenderEngine<Scr> & eng,
          std::shared_ptr<Buffer> & v,
          LFun                      layout,
          GLsizei                   num,
          GLenum                    fmt = GL_TRIANGLE_STRIP,
          GLuint                    off = 0
    )
    : _fmt     { fmt },
      _elOff   { off },
      _elFmt   {   0 },
      _indices { num }
    {
        if(v->target() != GL_ARRAY_BUFFER)
            throw("First Buffer must be an Array Buffer!");

        glGenVertexArrays(1, &_vaoID);

        add_array(    eng, v, layout);
    }

    template <class LFun, class Scr>
    BufferPoly
    (
          const RenderEngine<Scr> & eng,
          std::shared_ptr<Buffer> & v,
          LFun                      layout,
          std::shared_ptr<Buffer> & e,
          GLsizei                   num,
          GLenum                    fmt   = GL_TRIANGLE_STRIP,
          GLenum                    elFmt = GL_UNSIGNED_INT,
          GLuint                    off   = 0
    )
    : BufferPoly(eng, v, layout, num, fmt, off)
    {
        if(e->target() != GL_ELEMENT_ARRAY_BUFFER)
            throw("Second Buffer must be Element Array Buffer!");

        add_indices(elFmt, e);
    };

    ~BufferPoly() { glDeleteVertexArrays(1, &_vaoID); };

    BufferPoly(const BufferPoly & that) = delete;

    BufferPoly &
    operator=( const BufferPoly & that) = delete;

    void
    bind() const
    {
        if(_currentBinding == _vaoID) return;

        _currentBinding = _vaoID;
        glBindVertexArray(_vaoID);
    };

    template <class LFun, class Scr>
    void
    add_array
    (
        const RenderEngine<Scr> & eng,
        std::shared_ptr<Buffer> & v,
        LFun                      layout
    )
    {
        _tiedBuffs.emplace_back(v);

        bind(); v->bind();
        layout(    eng.prog(), *v);
    };

    void
    add_indices
    (
        GLenum                    elFmt,
        std::shared_ptr<Buffer> & e
    )
    {
        bind();

        _elFmt   = elFmt;
        _elBuff  =     e;

        e->bind();
    };

    friend class Instance;
};

GLuint BufferPoly::_currentBinding = 0;

}; // namespace engine

#endif
