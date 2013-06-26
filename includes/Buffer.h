#ifndef BUFFER_H
#define BUFFER_H

#include "GL_includes.h"

enum BufferAttrib {
    ATTRIB_VERT  = 0,
    ATTRIB_COLOR = 1,
    ATTRIB_NORM  = 2,
    ATTRIB_TEXUV = 3
};

template <class S>
class Buffer {

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

    void           bind()   const      {  glBindBuffer( _target, _id ); }
    const GLenum & target() const      { return                _target; }

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