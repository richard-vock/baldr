#pragma once

#include "common.hpp"

namespace baldr {

class data_buffer
{
public:
    data_buffer(GLuint byte_count, GLenum usage_hint, const void* data = nullptr);

    template <typename T>
    data_buffer(const std::vector<T>& data, GLenum usage_hint);

    template <typename Derived>
    data_buffer(const Eigen::PlainObjectBase<Derived>& dense_array, GLenum usage_hint);

    virtual ~data_buffer();

    GLuint handle() const;

    GLuint
    byte_count() const;

    template <typename T>
    uint32_t
    value_count() const;

    void
    set_data(const void* data);

    template <typename T>
    void
    set_data(const std::vector<T>& data);

    void
    get_data(void* data) const;

    void
    get_data(void* data, GLuint byte_count) const;

    template <typename T>
    void
    get_data(std::vector<T>& data) const;

    void*
    map(GLbitfield access);

    const void*
    map(GLbitfield access) const;

    void
    unmap() const;

    void
    clear_to_zero();

protected:
    bool allocated_;
    GLuint byte_count_;
    GLenum usage_hint_;
    GLuint handle_;
};

}  // namespace baldr

#include "data_buffer.ipp"
