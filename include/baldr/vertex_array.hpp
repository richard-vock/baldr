#pragma once

#include "common.hpp"
#include "data_buffer.hpp"

namespace baldr {

struct vertex_attribute
{
    GLuint index;
    GLint size;
    GLenum type;
    GLuint offset;
    GLboolean normalized;
};

struct binding_point {
    GLuint binding;
    GLuint vao;
    GLuint stride;

    const binding_point&
    operator=(const data_buffer& buffer) const;
};

class vertex_array
{
public:
    vertex_array();

    virtual ~vertex_array();

    [[nodiscard]] GLuint handle() const;

    void bind() const;

    static void release();

    void set_index_buffer(const data_buffer& ibo);

    [[nodiscard]] binding_point
    vertex_buffer_binding(const std::vector<vertex_attribute>& attributes, GLuint vertex_stride);

protected:
    GLuint handle_;
    GLuint next_binding_point_;
};

}  // namespace baldr
