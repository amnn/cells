#ifndef BUFFERPOLY_H
#define BUFFERPOLY_H

#include <memory>
#include <vector>

#include "GL_includes.h"
#include "glm/glm.hpp"

#include "Buffer.h"
#include "Texture.h"
#include "Renderable.h"
#include "ShaderProgram.h"

class BufferPoly 
{

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

        Instance( std::shared_ptr<BufferPoly>  &poly ) 
        : Renderable(),       _poly       { poly } {};

        Instance( const Instance               &that ) 
        : Renderable( that ), _poly { that._poly } {};

        Instance( Instance                    &&that ) 
        : Renderable( that ) 
        { 
            std::swap( _poly, that._poly ); 
        };

        void render( const ShaderProgram &p, const glm::mat4 &m ) const
        {
            _poly->bind(); glm::mat4 trn = m * _local;

            glUniformMatrix4fv( p.matID(), 1, GL_FALSE, &trn[0][0] );
            glDrawElements(

                _poly->_fmt, 
                _poly->_indices, 
                _poly->_elFmt, 
                reinterpret_cast<void *>( _poly->_elOff ) 

            );
        };

    };

    class TextureInstance : public Instance {

        std::shared_ptr<Texture> _tex;

    public:

        TextureInstance( std::shared_ptr< BufferPoly >  & poly,
                         std::shared_ptr<    Texture >  &  tex )
        : Instance( poly ), _tex                     { tex } {};

        TextureInstance( const TextureInstance          & that )
        : Instance( that ), _tex               { that._tex } {};

        TextureInstance( TextureInstance               && that )
        : Instance( that )
        {
            std::swap( _tex, that._tex );
        };

        void render( const ShaderProgram &p, const glm::mat4 &m ) const
        {
            _tex->bind(); Instance::render( p, m );
        }

    };

    template <class S, class T>
    BufferPoly
    (

        std::shared_ptr< Buffer<S> > &v, 
        std::shared_ptr< Buffer<T> > &e,
        GLsizei                     num,
        GLenum                      fmt = GL_TRIANGLE_STRIP, 
        GLenum                    elFmt =   GL_UNSIGNED_INT,    
        GLuint                      off =                 0

    )
    {

        if( v->target() !=         GL_ARRAY_BUFFER ) throw(          "First Buffer must be Array Buffer!" );
        if( e->target() != GL_ELEMENT_ARRAY_BUFFER ) throw( "Second Buffer must be Element Array Buffer!" );

        glGenVertexArrays( 1, &_vaoID );

        add_array(                         v );
        add_indices( fmt, num, elFmt, off, e );

    };

    ~BufferPoly() { glDeleteVertexArrays( 1, &_vaoID ); };

    BufferPoly            ( const BufferPoly &that ) = delete;
    BufferPoly &operator= ( const BufferPoly &that ) = delete;

    void bind() const { glBindVertexArray( _vaoID ); };

    template <class S>
    void add_array( std::shared_ptr< Buffer<S> > &v )
    {
        _tiedBuffs.emplace_back(   v );
        bind(); S::layout(        *v );
    };

    template <class T>
    void add_indices
    ( 
    
        GLenum                         fmt,
        GLsizei                        num,
        GLenum                       elFmt,
        GLuint                         off,
        std::shared_ptr< Buffer<T> >    &e 
    
    )
    {
        bind();

        _fmt     =   fmt;
        _indices =   num;
        _elFmt   = elFmt;
        _elOff   =   off;
        _elBuff  =     e;

        e->bind();
    };

    friend class Instance;

};

#endif