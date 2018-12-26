#include <baldr/vertex_array.hpp>

namespace baldr {

const binding_point&
binding_point::operator=(const data_buffer& buffer) const {
    glVertexArrayVertexBuffer(vao, binding, buffer.handle(), 0, stride);
    return *this;
}

vertex_array::vertex_array() : handle_(0), next_binding_point_(0) {
    glCreateVertexArrays(1, &handle_);
}

vertex_array::~vertex_array() {
    glDeleteVertexArrays(1, &handle_);
}

GLuint
vertex_array::handle() const {
    return handle_;
}

void
vertex_array::bind() const {
    glBindVertexArray(handle_);
}

void
vertex_array::release() {
    glBindVertexArray(0);
}

void
vertex_array::set_index_buffer(const data_buffer& ibo) {
    glVertexArrayElementBuffer(handle_, ibo.handle());
}

binding_point
vertex_array::vertex_buffer_binding(const std::vector<vertex_attribute>& attributes, GLuint vertex_stride) {
    GLuint bp = next_binding_point_++;

    for (const auto& attr : attributes) {
        auto && [index, size, type, offset, normalized] = attr;
        glEnableVertexArrayAttrib(handle_, index);
        glVertexArrayAttribFormat(handle_, index, size, type, normalized, offset);
        glVertexArrayAttribBinding(handle_, index, bp);
    }

    return {
        .binding = bp,
        .vao = handle_,
        .stride = vertex_stride
    };
}

} // baldr
