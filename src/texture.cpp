#include <texture.hpp>

namespace baldr {

texture::texture(int32_t width, texture_specification specs) : width_(width), height_(0), depth_(0), specs_(specs) {
    glCreateTextures(GL_TEXTURE_1D, 1, &handle_);
    glTextureStorage1D(handle_, specs.levels, specs.internal_format, width);
    set_filter(specs.filter);
    set_wrap_mode(specs.wrap_mode);
}

texture::texture(int32_t width, int32_t height, texture_specification specs) : width_(width), height_(height), depth_(0), specs_(specs) {
    glCreateTextures(specs.cubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D, 1, &handle_);
    glTextureStorage2D(handle_, specs.levels, specs.internal_format, width, height);
    set_filter(specs.filter);
    set_wrap_mode(specs.wrap_mode);
    set_max_level(specs.levels-1);
}

texture::texture(int32_t width, int32_t height, int32_t depth, texture_specification specs) : width_(width), height_(height), depth_(depth), specs_(specs) {
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

int
texture::width() const noexcept {
    return width_;
}

int
texture::height() const noexcept {
    return height_;
}

int
texture::depth() const noexcept {
    return depth_;
}

int
texture::pixel_count() const noexcept {
    int count = width_;
    if (height_) count *= height_;
    if (depth_) count *= depth_;
    return count;
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
texture::clear(T value, int level) {
    terminate_unless(channel_count() != 1, "Scalar clear() function used for texture with more than one channel. Use vector variants instead.");
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

    glClearTexImage(handle_, level, specs_.format, pixel_type, &value);
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
texture::set(const T* data, int cubemap_face, int level) {
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

    glTextureSubImage3D(handle_,
                        level,
                        0, // xoffset
                        0, // yoffset
                        cubemap_face, // zoffset
                        width_ >> level, // width
                        height_ >> level, // height
                        1, // depth
                        specs_.format,
                        pixel_type,
                        data);
}

template <typename T>
void
texture::set_cubemap_faces(const T* data, int level) {
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

    glTextureSubImage3D(handle_,
                        level,
                        0, // xoffset
                        0, // yoffset
                        0, // zoffset
                        width_ >> level, // width
                        height_ >> level, // height
                        6, // depth
                        specs_.format,
                        pixel_type, data);
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
    int w = width_ / divisor;
    int h = height_ ? (height_ / divisor) : 1;
    int d = depth_ ? (depth_ / divisor) : 1;

    GLsizei buf_size = 4 * w * h * d * sizeof(T);
    glGetTextureImage(handle_, level, specs_.format, pixel_type, buf_size, (void*)data);
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
        case GL_RED_INTEGER:
            return 1u;
        case GL_RG:
            return 2u;
        case GL_RG_INTEGER:
            return 2u;
        case GL_RGB:
            return 3u;
        case GL_RGB_INTEGER:
            return 3u;
        case GL_BGR:
            return 3u;
        case GL_BGR_INTEGER:
            return 3u;
        case GL_RGBA:
            return 4u;
        case GL_RGBA_INTEGER:
            return 4u;
        case GL_BGRA:
            return 4u;
        case GL_BGRA_INTEGER:
            return 4u;
        default:
            return 1u;
    }
}

const texture_specification&
texture::specification() const {
    return specs_;
}

template void texture::clear<float>(float, int);
template void texture::clear<uint8_t>(uint8_t, int);
template void texture::clear<int8_t>(int8_t, int);
template void texture::clear<uint16_t>(uint16_t, int);
template void texture::clear<int16_t>(int16_t, int);
template void texture::clear<uint32_t>(uint32_t, int);
template void texture::clear<int32_t>(int32_t, int);

template void texture::set<float>(const float*);
template void texture::set<uint8_t>(const uint8_t*);
template void texture::set<int8_t>(const int8_t*);
template void texture::set<uint16_t>(const uint16_t*);
template void texture::set<int16_t>(const int16_t*);
template void texture::set<uint32_t>(const uint32_t*);
template void texture::set<int32_t>(const int32_t*);

template void texture::set<float>(const float*, int, int);
template void texture::set<uint8_t>(const uint8_t*, int, int);
template void texture::set<int8_t>(const int8_t*, int, int);
template void texture::set<uint16_t>(const uint16_t*, int, int);
template void texture::set<int16_t>(const int16_t*, int, int);
template void texture::set<uint32_t>(const uint32_t*, int, int);
template void texture::set<int32_t>(const int32_t*, int, int);

template void texture::set_cubemap_faces<float>(const float*, int);
template void texture::set_cubemap_faces<uint8_t>(const uint8_t*, int);
template void texture::set_cubemap_faces<int8_t>(const int8_t*, int);
template void texture::set_cubemap_faces<uint16_t>(const uint16_t*, int);
template void texture::set_cubemap_faces<int16_t>(const int16_t*, int);
template void texture::set_cubemap_faces<uint32_t>(const uint32_t*, int);
template void texture::set_cubemap_faces<int32_t>(const int32_t*, int);

template void texture::get<float>(GLint, float*);
template void texture::get<uint8_t>(GLint, uint8_t*);
template void texture::get<int8_t>(GLint, int8_t*);
template void texture::get<uint16_t>(GLint, uint16_t*);
template void texture::get<int16_t>(GLint, int16_t*);
template void texture::get<uint32_t>(GLint, uint32_t*);
template void texture::get<int32_t>(GLint, int32_t*);

} // baldr
