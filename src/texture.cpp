#include <baldr/texture.hpp>

namespace baldr {

texture::texture(int32_t width, const texture_specification& specs) : width_(width), height_(0), depth_(0), specs_(specs) {
    glCreateTextures(GL_TEXTURE_1D, 1, &handle_);
    glTextureStorage1D(handle_, specs.levels, specs.internal_format, width);
    set_filter(specs.filter);
    set_wrap_mode(specs.wrap_mode);
}

texture::texture(int32_t width, int32_t height, const texture_specification& specs) : width_(width), height_(height), depth_(0), specs_(specs) {
    glCreateTextures(GL_TEXTURE_2D, 1, &handle_);
    glTextureStorage2D(handle_, specs.levels, specs.internal_format, width, height);
    glTextureParameteri(handle_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(handle_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    set_filter(specs.filter);
    set_wrap_mode(specs.wrap_mode);
}

texture::texture(int32_t width, int32_t height, int32_t depth, const texture_specification& specs) : width_(width), height_(height), depth_(depth), specs_(specs) {
    glCreateTextures(GL_TEXTURE_3D, 1, &handle_);
    glTextureStorage3D(handle_, specs.levels, specs.internal_format, width, height, depth);
    set_filter(specs.filter);
    set_wrap_mode(specs.wrap_mode);
}

texture::~texture() {
    glDeleteTextures(1, &handle_);
}

GLuint
texture::handle() const {
    return handle_;
}

void
texture::set_filter(std::tuple<GLint, GLint> filter) {
    glTextureParameteri(handle_, GL_TEXTURE_MIN_FILTER, std::get<0>(filter));
    glTextureParameteri(handle_, GL_TEXTURE_MAG_FILTER, std::get<1>(filter));
}

void
texture::set_wrap_mode(std::tuple<GLint, GLint, GLint> wrap_mode) {
    glTextureParameteri(handle_, GL_TEXTURE_WRAP_S, std::get<0>(wrap_mode));
    glTextureParameteri(handle_, GL_TEXTURE_WRAP_T, std::get<1>(wrap_mode));
    glTextureParameteri(handle_, GL_TEXTURE_WRAP_R, std::get<2>(wrap_mode));
}

void
texture::set_max_level(int max_level) {
    glTextureParameteri(handle_, GL_TEXTURE_MAX_LEVEL, max_level);
}

void
texture::reset_max_level() {
    glTextureParameteri(handle_, GL_TEXTURE_MAX_LEVEL, specs_.levels - 1);
}

template <typename T>
void
texture::set(const T* data) {
    if (!data) return;

    GLuint pixel_type = GL_INT;
    if constexpr (std::is_same_v<T, float>) {
        pixel_type = GL_FLOAT;
    } else if (std::is_same_v<T, uint8_t>) {
        pixel_type = GL_UNSIGNED_BYTE;
    } else if (std::is_same_v<T, int8_t>) {
        pixel_type = GL_BYTE;
    } else if (std::is_same_v<T, uint16_t>) {
        pixel_type = GL_UNSIGNED_SHORT;
    } else if (std::is_same_v<T, int16_t>) {
        pixel_type = GL_SHORT;
    } else if (std::is_same_v<T, uint32_t>) {
        pixel_type = GL_UNSIGNED_INT;
    }

    if (depth_) {
        glTextureSubImage3D(handle_, 0, 0, 0, 0, width_, height_, depth_, specs_.format, pixel_type, data);
    } else if (height_) {
        glTextureSubImage2D(handle_, 0, 0, 0, width_, height_, specs_.format, pixel_type, data);
    } else {
        glTextureSubImage1D(handle_, 0, 0, width_, specs_.format, pixel_type, data);
    }
}

template <typename T>
void
texture::get(GLint level, T* data) {
    if (!data) return;

    GLuint pixel_type = GL_INT;
    if constexpr (std::is_same_v<T, float>) {
        pixel_type = GL_FLOAT;
    } else if (std::is_same_v<T, uint8_t>) {
        pixel_type = GL_UNSIGNED_BYTE;
    } else if (std::is_same_v<T, int8_t>) {
        pixel_type = GL_BYTE;
    } else if (std::is_same_v<T, uint16_t>) {
        pixel_type = GL_UNSIGNED_SHORT;
    } else if (std::is_same_v<T, int16_t>) {
        pixel_type = GL_SHORT;
    } else if (std::is_same_v<T, uint32_t>) {
        pixel_type = GL_UNSIGNED_INT;
    }

    GLsizei divisor = std::pow(2, level);
    GLsizei buf_size = (width_ / divisor) * sizeof(T);
    if (height_) buf_size *= (height_ / divisor);
    if (depth_) buf_size *= (depth_ / divisor);
    buf_size *= channel_count();

    glGetTextureImage(handle_, level, specs_.format, pixel_type, buf_size, data);
}

void
texture::generate_mipmap() {
    glGenerateTextureMipmap(handle_);
}

uint32_t
texture::channel_count() const {
    switch (specs_.format) {
        case GL_RED:
            return 1u;
        case GL_RG:
            return 2u;
        case GL_RGB:
            return 3u;
        case GL_BGR:
            return 3u;
        case GL_RGBA:
            return 4u;
        case GL_BGRA:
            return 4u;
        default:
            return 1u;
    }
}

template void texture::set<float>(const float*);
template void texture::set<uint8_t>(const uint8_t*);
template void texture::set<int8_t>(const int8_t*);
template void texture::set<uint16_t>(const uint16_t*);
template void texture::set<int16_t>(const int16_t*);
template void texture::set<uint32_t>(const uint32_t*);
template void texture::set<int32_t>(const int32_t*);

template void texture::get<float>(GLint, float*);
template void texture::get<uint8_t>(GLint, uint8_t*);
template void texture::get<int8_t>(GLint, int8_t*);
template void texture::get<uint16_t>(GLint, uint16_t*);
template void texture::get<int16_t>(GLint, int16_t*);
template void texture::get<uint32_t>(GLint, uint32_t*);
template void texture::get<int32_t>(GLint, int32_t*);

} // baldr
