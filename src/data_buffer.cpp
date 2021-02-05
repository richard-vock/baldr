#include <data_buffer.hpp>

namespace baldr {

data_buffer::data_buffer(GLuint byte_count, GLenum usage_hint, const void* data) : allocated_(false), byte_count_(byte_count), usage_hint_(usage_hint) {
    glCreateBuffers(1, &handle_);
    set_data(data);
}

data_buffer::~data_buffer() {
    glDeleteBuffers(1, &handle_);
}

GLuint
data_buffer::handle() const {
    return handle_;
}

GLuint
data_buffer::byte_count() const {
    return byte_count_;
}

void
data_buffer::set_data(const void* data) {
    if (!allocated_) {
        glNamedBufferData(handle_, byte_count_, data, usage_hint_);
        allocated_ = true;
        return;
    }

    glNamedBufferSubData(handle_, 0, byte_count_, data);
}

void
data_buffer::get_data(void* data) const {
    get_data(data, byte_count_);
}

void
data_buffer::get_data(void* data, GLuint byte_count) const {
    if (!allocated_) {
        return;
    }

    memcpy(data, map(GL_MAP_READ_BIT), byte_count);
    unmap();
}

void*
data_buffer::map(GLbitfield access) {
    return glMapNamedBufferRange(handle_, 0, byte_count_, access);
}

const void*
data_buffer::map(GLbitfield access) const {
    return glMapNamedBufferRange(handle_, 0, byte_count_, access);
}

void
data_buffer::unmap() const {
    glUnmapNamedBuffer(handle_);
}

void
data_buffer::clear_to_zero() {
    glClearNamedBufferData(handle_, GL_R32UI, GL_RED, GL_UNSIGNED_INT, nullptr);
}

} // baldr
