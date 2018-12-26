#pragma once

#include "common.hpp"

namespace baldr::detail {

template <typename T>
void
bind_uniform(T&& value, GLint location, GLenum type, GLuint program);

struct attribute_format {
    GLint size;
    GLenum type;
};

attribute_format
shader_attribute_format(GLenum type);

} // baldr::detail

#include "shader_interop.ipp"
