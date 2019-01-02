#pragma once

#include "common.hpp"

namespace baldr {

struct texture_specification
{
    GLuint format;
    GLuint internal_format;
    std::tuple<GLint, GLint> filter = {GL_LINEAR, GL_LINEAR};
    std::tuple<GLint, GLint, GLint> wrap_mode = {
        GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    GLuint levels = 1;
    GLuint border = 0;
};

class texture
{
public:
    texture(int32_t width, const texture_specification& specs);

    texture(int32_t width, int32_t height, const texture_specification& specs);

    texture(int32_t width, int32_t height, int32_t depth,
            const texture_specification& specs);

    texture(const texture& other) = delete;

    texture&
    operator=(const texture& other) = delete;

    template <typename... Is>
    static std::shared_ptr<texture>
    r32f(Is... dimensions);

    template <typename... Is>
    static std::shared_ptr<texture>
    rg32f(Is... dimensions);

    template <typename... Is>
    static std::shared_ptr<texture>
    rgb32f(Is... dimensions);

    template <typename... Is>
    static std::shared_ptr<texture>
    rgba32f(Is... dimensions);

    template <typename... Is>
    static std::shared_ptr<texture>
    depth32f(Is... dimensions);

    virtual ~texture();

    GLuint
    handle() const;

    void
    set_filter(std::tuple<GLint, GLint> filter);

    void
    set_wrap_mode(std::tuple<GLint, GLint, GLint> wrap_mode);

    template <typename T>
    void
    set(const T* data);

    template <typename T>
    void
    get(GLint level, T* data);

    void
    generate_mipmap();

    uint32_t
    channel_count() const;

protected:
    GLuint handle_;
    int width_;
    int height_;
    int depth_;
    texture_specification specs_;
};

}  // namespace baldr

#include "texture.ipp"
