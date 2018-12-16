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

    void
    set_data(const void* data);

    template <typename T>
    void
    set_data(const std::vector<T>& data);


protected:
    bool allocated_;
    GLuint byte_count_;
    GLenum usage_hint_;
    GLuint handle_;
};

}  // namespace baldr

#include "data_buffer.ipp"
