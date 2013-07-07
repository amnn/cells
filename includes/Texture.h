#ifndef TEXTURE_H
#define TEXTURE_H

#include <unordered_map>

class Texture
{

    static std::unordered_map< GLenum, GLuint > _bindings;

    GLuint     _id;

    Texture( GLenum );

public:

    ~Texture();

    Texture( Texture && );
    Texture( Texture  & ) = delete;

    Texture &operator=( Texture && );
    Texture &operator=( Texture  & ) = delete;

    void   param( GLenum,   GLint ) const;
    void   param( GLenum, GLfloat ) const;

    void   mip_maps()               const;

    void   bind()                   const;
    GLenum target()                 const;

protected:

    GLenum _target;
    GLint     _fmt;

    Texture( GLenum, GLint );
    
};

class Texture1D : public Texture {

    GLsizei _w;

public:

    Texture1D
    ( 
    
        GLenum, 
        GLint, 
        GLint, 
        GLsizei 
    
    );

    template <class S>
    Texture1D
    ( 
    
        GLenum, 
        GLint, 
        GLint, 
        GLsizei, 
        GLenum, 
        GLenum, 
        S * 
    
    );

    template <class S>
    void image
    ( 

        GLint, 
        GLenum, 
        GLenum, 
        S * 

    );

    template <class S>
    void sub_image
    ( 
        
        GLint, 
        GLint, 
        GLsizei, 
        GLenum, 
        GLenum, 
        S * 

    );

};

class Texture2D : public Texture {

    GLsizei _w, _h;

public:

    Texture2D
    ( 
    
        GLenum, 
        GLint, 
        GLint, 
        GLsizei, 
        GLsizei 

    );

    template <class S>
    Texture2D
    ( 
        GLenum, 
        GLint, 
        GLint, 
        GLsizei, 
        GLsizei, 
        GLenum, 
        GLenum, 
        S * 

    );

    template <class S>
    void image
    ( 
    
        GLint, 
        GLenum, 
        GLenum, 
        S * 

    );

    template <class S>
    void sub_image
    ( 

        GLint, 
        GLint, 
        GLint, 
        GLsizei, 
        GLsizei, 
        GLenum, 
        GLenum, 
        S * 

    );

};

class Texture3D : public Texture {

    GLsizei _w, _h, _d;

public:

    Texture3D
    ( 
   
        GLenum, 
        GLint, 
        GLint, 
        GLsizei, 
        GLsizei, 
        GLsizei 

    );

    template <class S>
    Texture3D
    ( 

        GLenum, 
        GLint, 
        GLint, 
        GLsizei, 
        GLsizei, 
        GLsizei, 
        GLenum, 
        GLenum, 
        S * 

    );

    template <class S>
    void image
    ( 

        GLint, 
        GLenum, 
        GLenum, 
        S * 

    );

    template <class S>
    void sub_image
    ( 

        GLint, 
        GLint, 
        GLint, 
        GLint, 
        GLsizei, 
        GLsizei, 
        GLsizei, 
        GLenum, 
        GLenum, 
        S * 

    );

};

#endif