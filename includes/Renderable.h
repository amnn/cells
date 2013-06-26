#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "glm/glm.hpp"

#include "ShaderProgram.h"

class Renderable 
{

public:

    typedef void (*tick_callback)(Renderable &,const double &);

    Renderable() : _local( 1.0 ), _cb { 0 } {};

    Renderable( const Renderable &that )
    {
        _local = that._local;
        _cb    =    that._cb;
    }

    Renderable( Renderable &&that )
    {
        std::swap( _local, that._local );
        std::swap( _cb,       that._cb );
    }

    Renderable &operator=( const Renderable &that )
    {
        _local = that._local;
        _cb    =    that._cb;

        return *this;
    }

    Renderable &operator=( Renderable &&that )
    {
        std::swap( _local, that._local );
        std::swap( _cb,       that._cb );

        return *this;
    }
    
    glm::mat4     & transform()              { return                 _local; }
    tick_callback & callback()               { return                    _cb; }

    virtual void tick( const double &delta ) { if( _cb ) _cb( *this, delta ); }

    virtual void render( const ShaderProgram &, const glm::mat4 & ) const = 0;


protected:

    glm::mat4     _local;
    tick_callback    _cb;

};

#endif