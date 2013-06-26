#include <initializer_list>
#include <utility>
#include <memory>

#include "glm/glm.hpp"

#include "RenderGroup.h"

RenderGroup::RenderGroup( std::initializer_list< std::shared_ptr<Renderable> > &&_children )
: Renderable()
{

    children.reserve( _children.size() );

    for( auto c : _children ) this->add_child( c );

}

RenderGroup::RenderGroup( RenderGroup &&that )
{
    std::swap( children, that.children );
}

RenderGroup &RenderGroup::operator=( RenderGroup &&that )
{
    std::swap( children, that.children );
}

void RenderGroup::add_child( std::shared_ptr<Renderable> &pChild )
{
    children.emplace_back( pChild );
}

void RenderGroup::tick( const double &delta )
{

    Renderable::tick( delta );
    for( auto c : children ) c->tick( delta );

}

void RenderGroup::render( const ShaderProgram &p, const glm::mat4 &m ) const
{
    glm::mat4 transform = m * _local;
    for( auto c : children ) c->render( p, transform );

}