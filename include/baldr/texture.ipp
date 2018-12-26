namespace baldr {

template <typename... Is>
inline std::shared_ptr<texture>
texture::r32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            .format = GL_RED,
            .internal_format = GL_R32F,
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rgb32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            .format = GL_RGB,
            .internal_format = GL_RGB32F,
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::rgba32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            .format = GL_RGBA,
            .internal_format = GL_RGBA32F,
        }
    ));
}

template <typename... Is>
inline std::shared_ptr<texture>
texture::depth32f(Is... dimensions) {
    return std::shared_ptr<texture>(new texture(
        dimensions...,
        texture_specification{
            .format = GL_DEPTH_COMPONENT,
            .internal_format = GL_DEPTH_COMPONENT32F,
        }
    ));
}

} // baldr
