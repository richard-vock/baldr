#pragma once

#include "common.hpp"

namespace baldr {

struct texture_specification
{
    GLuint format;
    GLuint internal_format;
    std::tuple<GLint, GLint> filter = {GL_NEAREST, GL_NEAREST};
    std::tuple<GLint, GLint, GLint> wrap_mode = {
        GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
    GLuint levels = 1;
    GLuint border = 0;
    bool cubemap = false;
};

class texture
{
public:
    texture(int32_t width, texture_specification specs);

    texture(int32_t width, int32_t height, texture_specification specs);

    texture(int32_t width, int32_t height, int32_t depth,
            texture_specification specs);

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
    rgb8ui(Is... dimensions);

    template <typename... Is>
    static std::shared_ptr<texture>
    rgba8ui(Is... dimensions);

    template <typename... Is>
    static std::shared_ptr<texture>
    rgb8(Is... dimensions);

    template <typename... Is>
    static std::shared_ptr<texture>
    rgba8(Is... dimensions);

    template <typename... Is>
    static std::shared_ptr<texture>
    depth32f(Is... dimensions);

    virtual ~texture();

    GLuint
    handle() const;

    [[ nodiscard ]]
    int width() const noexcept;

    [[ nodiscard ]]
    int height() const noexcept;

    [[ nodiscard ]]
    int depth() const noexcept;

    [[ nodiscard ]]
    int pixel_count() const noexcept;

    void
    set_filter(std::tuple<GLint, GLint> filter);

    void
    set_wrap_mode(std::tuple<GLint, GLint, GLint> wrap_mode);

    void
    set_max_level(int max_level);

    void
    reset_max_level();

    template <typename T>
    void
    clear(T value, int level = 0);

    template <typename T, int Channel>
    void
    clear(Eigen::Matrix<T, Channel, 1> values, int level = 0);

    template <typename T>
    void
    set(const T* data);

    template <typename T>
    void
    set(const T* data, int cubemap_face, int level=0);

    template <typename T>
    void
    set_cubemap_faces(const T* data, int level=0);

    template <typename T>
    void
    get(GLint level, T* data);

    void
    generate_mipmap();

    uint32_t
    channel_count() const;

    const texture_specification&
    specification() const;

protected:
    GLuint handle_;
    int width_;
    int height_;
    int depth_;
    texture_specification specs_;
};

}  // namespace baldr

#include "texture.ipp"
