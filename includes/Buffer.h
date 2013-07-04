#ifndef BUFFER_H
#define BUFFER_H

#include <unordered_map>

#include "GL_includes.h"

enum BufferAttrib {
    ATTRIB_VERT  = 0,
    ATTRIB_COLOR = 1,
    ATTRIB_NORM  = 2,
    ATTRIB_TEXUV = 3
};

class _BufferBindings {
protected:

    static std::unordered_map< GLenum, GLuint > _bindings;

};

std::unordered_map< GLenum, GLuint > _BufferBindings::_bindings {};

template <class S>
class Buffer : public _BufferBindings {

    GLuint _id, _target;

public:

    Buffer()                           {     glGenBuffers( 1, &_id ); }
    ~Buffer()                          {  glDeleteBuffers( 1, &_id ); }

    Buffer( 
    
        GLenum     target, 
        GLsizei      size, 
        S       *contents, 
        GLenum      usage 
    
    ) : Buffer()     { _target = target; data( size, contents, usage ); }

    Buffer( Buffer &&that )            {    std::swap( _id, that._id ); }
    Buffer( Buffer & )                                        = delete;

    Buffer &operator=( Buffer &&that ) {    std::swap( _id, that._id ); 
                                                          return *this; }
    Buffer &operator=( Buffer & )                             = delete;

    const GLenum & target() const      { return                _target; }

    void bind( GLenum target ) {

        _bindings[ _target ] =      0;
        _target              = target;

        bind();

    }

    void bind() const { 
        
        GLuint & current = _bindings[ _target ];

        if( _target != GL_ELEMENT_ARRAY_BUFFER )
            if( current == _id )        return; 
            else                 current = _id;


        glBindBuffer( _target, _id ); 
    }

    void register_attrib
    (
        BufferAttrib   attr,
        GLuint         size,
        GLenum         type,
        GLboolean      norm,
        GLsizei      stride,
        GLuint          off

    ) const
    {

        if( _target != GL_ARRAY_BUFFER ) return;

        bind();

        glEnableVertexAttribArray( (GLuint)attr );
        glVertexAttribPointer( (GLuint)attr, size, type, norm, stride, reinterpret_cast<void *>( off ) );

    }

    void data
    ( 
 
        GLsizei      size, 
        S       *contents, 
        GLenum      usage 

    ) const
    {
        bind();
        glBufferData( _target, size * sizeof( S ), contents, usage );
    }

};

#endif