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

void
data_buffer::set_data(const void* data) {
    if (!allocated_) {
        glNamedBufferData(handle_, byte_count_, data, usage_hint_);
        allocated_ = true;
        return;
    }

    glNamedBufferSubData(handle_, 0, byte_count_, data);
}


} // baldr
