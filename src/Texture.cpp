#include <utility>
#include <unordered_map>

#include "GL_includes.h"

#include "Texture.h"

std::unordered_map< GLenum, GLuint > Texture::_bindings {};

Texture::Texture( GLenum target ) : _target { target } { glGenTextures( 1, &_id ); }

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

void Texture::param( GLenum pname, GLint   p ) const { glTexParameteri( _target, pname, p ); }
void Texture::param( GLenum pname, GLfloat p ) const { glTexParameterf( _target, pname, p ); }

void Texture::bind() const { 
 	
 	GLuint & current = _bindings[ _target ];

 	if( current != _id ) {
 		
 		glBindTexture( _target, _id );
 		current =               _id  ;

 	}


}

GLenum Texture::target() const { return _target; };