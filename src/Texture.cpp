#include <utility>
#include <unordered_map>

#include "GL_includes.h"

#include "texture/Texture.h"

namespace engine {

std::unordered_map< GLenum, GLuint > Texture::_bindings {};

Texture::Texture( GLenum target, GLint fmt ) 
: _target { target }, _fmt { fmt }
{ glGenTextures( 1, &_id ); }

Texture::Texture( GLenum target ) : Texture( target, 0 ) {}

Texture::~Texture() { glDeleteTextures( 1, &_id ); }

Texture::Texture( Texture &&that )
{
    std::swap( _id,         that._id );
    std::swap( _target, that._target );  
}

Texture &Texture::operator=( Texture &&that )
{
    std::swap( _id,         that._id );
    std::swap( _target, that._target );  

    return *this;    
}

void Texture::param( GLenum pname, GLint   p ) const 
{ bind(); glTexParameteri( _target, pname, p ); }

void Texture::param( GLenum pname, GLfloat p ) const 
{ bind(); glTexParameterf( _target, pname, p ); }

void Texture::mip_maps() const 
{ bind(); glGenerateMipmap(          _target ); }

void Texture::bind() const { 
    
    GLuint & current = _bindings[ _target ];

    if( current != _id ) {
        
        glBindTexture( _target, _id );
        current =               _id  ;

    }


}

GLenum Texture::target() const { return _target; };
GLuint Texture::id()     const { return     _id; };

}; // namespace engine
