#include <unordered_map>

#include "GL_includes.h"

#include "buffer/Buffer.h"

namespace Engine {
    std::unordered_map<GLenum, GLuint> Buffer::_bindings {};
}; // namespace Engine
