namespace baldr {

template <typename... Is>
inline std::shared_ptr<texture>
texture::r32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_RED,
            GL_R32F
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rg32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_RG,
            GL_RG32F
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rgb32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_RGB,
            GL_RGB32F
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rgba32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_RGBA,
            GL_RGBA32F
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rgb8ui(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_RGB_INTEGER,
            GL_RGB8UI
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rgba8ui(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_RGBA_INTEGER,
            GL_RGBA8UI
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rgb8(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_RGB,
            GL_RGB8
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rgba8(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_RGBA,
            GL_RGBA8
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::depth32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            GL_DEPTH_COMPONENT,
            GL_DEPTH_COMPONENT32F
        }
    ));
}

template <typename T, int Channels>
inline void
texture::clear(Eigen::Matrix<T, Channels, 1> values, int level) {
    terminate_unless(static_cast<int>(channel_count()) == Channels, "Vector clear() function used for texture with wrong channel count (texture: {}, values: {})", channel_count(), Channels);
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

    glClearTexImage(handle_, level, specs_.format, pixel_type, values.data());
}

} // baldr
