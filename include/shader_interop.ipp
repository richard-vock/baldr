namespace baldr::detail {

inline void
bind_uniform_impl(bool value, GLuint loc, GLenum type, GLuint program) {
    terminate_unless(type == GL_BOOL, "Trying to bind non-bool to bool uniform");
    glProgramUniform1i(program, loc, value);
}

inline void
bind_uniform_impl(int value, GLuint loc, GLenum type, GLuint program) {
    terminate_unless(type == GL_INT, "Trying to bind non-int to int uniform");
    glProgramUniform1i(program, loc, value);
}

inline void
bind_uniform_impl(unsigned int value, GLuint loc, GLenum type, GLuint program) {
    terminate_unless(type == GL_UNSIGNED_INT, "Trying to bind non-uint to uint uniform");
    glProgramUniform1ui(program, loc, value);
}

inline void
bind_uniform_impl(float value, GLuint loc, GLenum type, GLuint program) {
    terminate_unless(type == GL_FLOAT, "Trying to bind non-float to float uniform");
    glProgramUniform1f(program, loc, value);
}

inline void
bind_uniform_impl(double value, GLuint loc, GLenum type, GLuint program) {
    terminate_unless(type == GL_DOUBLE, "Trying to bind non-double to double uniform");
    glProgramUniform1d(program, loc, value);
}

template <typename T, int N>
inline void
bind_uniform_impl(const std::array<T, N>& value, GLuint loc, GLenum type, GLuint program) {
    if constexpr (N == 1) {
        bind_uniform_impl(std::get<0>(value), loc);
    }
    if constexpr (std::is_same_v<T, float> && N == 2) {
        terminate_unless(type == GL_FLOAT_VEC2, "Trying to bind non-vec2 to vec2 uniform");
        glProgramUniform2f(program, loc, std::get<0>(value), std::get<1>(value));
    }
    if constexpr (std::is_same_v<T, float> && N == 3) {
        terminate_unless(type == GL_FLOAT_VEC3, "Trying to bind non-vec3 to vec3 uniform");
        glProgramUniform3f(program, loc, std::get<0>(value), std::get<1>(value), std::get<2>(value));
    }
    if constexpr (std::is_same_v<T, float> && N == 4) {
        terminate_unless(type == GL_FLOAT_VEC4, "Trying to bind non-vec4 to vec4 uniform");
        glProgramUniform4f(program, loc, std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
    }
    if constexpr (std::is_same_v<T, double> && N == 2) {
        terminate_unless(type == GL_DOUBLE_VEC2, "Trying to bind non-dvec2 to dvec2 uniform");
        glProgramUniform2d(program, loc, std::get<0>(value), std::get<1>(value));
    }
    if constexpr (std::is_same_v<T, double> && N == 3) {
        terminate_unless(type == GL_DOUBLE_VEC3, "Trying to bind non-dvec3 to dvec3 uniform");
        glProgramUniform3d(program, loc, std::get<0>(value), std::get<1>(value), std::get<2>(value));
    }
    if constexpr (std::is_same_v<T, double> && N == 4) {
        terminate_unless(type == GL_DOUBLE_VEC4, "Trying to bind non-dvec4 to dvec4 uniform");
        glProgramUniform4d(program, loc, std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
    }
    if constexpr (std::is_same_v<T, int> && N == 2) {
        terminate_unless(type == GL_INT_VEC2, "Trying to bind non-ivec2 to ivec2 uniform");
        glProgramUniform2i(program, loc, std::get<0>(value), std::get<1>(value));
    }
    if constexpr (std::is_same_v<T, int> && N == 3) {
        terminate_unless(type == GL_INT_VEC3, "Trying to bind non-ivec3 to ivec3 uniform");
        glProgramUniform3i(program, loc, std::get<0>(value), std::get<1>(value), std::get<2>(value));
    }
    if constexpr (std::is_same_v<T, int> && N == 4) {
        terminate_unless(type == GL_INT_VEC4, "Trying to bind non-ivec4 to ivec4 uniform");
        glProgramUniform4i(program, loc, std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
    }
    if constexpr (std::is_same_v<T, unsigned int> && N == 2) {
        terminate_unless(type == GL_UNSIGNED_INT_VEC2, "Trying to bind non-uvec2 to uvec2 uniform");
        glProgramUniform2ui(program, loc, std::get<0>(value), std::get<1>(value));
    }
    if constexpr (std::is_same_v<T, unsigned int> && N == 3) {
        terminate_unless(type == GL_UNSIGNED_INT_VEC3, "Trying to bind non-uvec3 to uvec3 uniform");
        glProgramUniform3ui(program, loc, std::get<0>(value), std::get<1>(value), std::get<2>(value));
    }
    if constexpr (std::is_same_v<T, unsigned int> && N == 4) {
        terminate_unless(type == GL_UNSIGNED_INT_VEC4, "Trying to bind non-uvec4 to uvec4 uniform");
        glProgramUniform4ui(program, loc, std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
    }
}

template <typename Derived>
inline void
bind_uniform_impl(const Eigen::DenseBase<Derived>& value, GLuint loc, GLenum type, GLuint program) {
    constexpr int rows = Eigen::DenseBase<Derived>::RowsAtCompileTime;
    constexpr int cols = Eigen::DenseBase<Derived>::ColsAtCompileTime;
    using scalar_t = typename Eigen::DenseBase<Derived>::Scalar;
    if constexpr (rows <= 0 || cols <= 0) {
        fail("Invalid uniform matrix/vector object");
    }

    if constexpr (rows == 1 || cols == 1) {
        // vector case
        constexpr int major = std::max(rows, cols);
        if constexpr (major == 1) {
            // single value case
            bind_uniform_impl(value.value(), loc, type, program);
        } else {
            if constexpr (std::is_same<scalar_t, float>::value && major == 2) {
                terminate_unless(type == GL_FLOAT_VEC2, "Trying to bind non-vec2 to vec2 uniform");
                glProgramUniform2f(program, loc, value[0], value[1]);
            }
            if constexpr (std::is_same<scalar_t, float>::value && major == 3) {
                terminate_unless(type == GL_FLOAT_VEC3, "Trying to bind non-vec3 to vec3 uniform");
                glProgramUniform3f(program, loc, value[0], value[1], value[2]);
            }
            if constexpr (std::is_same<scalar_t, float>::value && major == 4) {
                terminate_unless(type == GL_FLOAT_VEC4, "Trying to bind non-vec4 to vec4 uniform");
                glProgramUniform4f(program, loc, value[0], value[1], value[2], value[3]);
            }
            if constexpr (std::is_same<scalar_t, double>::value && major == 2) {
                terminate_unless(type == GL_DOUBLE_VEC2, "Trying to bind non-dvec2 to dvec2 uniform");
                glProgramUniform2d(program, loc, value[0], value[1]);
            }
            if constexpr (std::is_same<scalar_t, double>::value && major == 3) {
                terminate_unless(type == GL_DOUBLE_VEC3, "Trying to bind non-dvec3 to dvec3 uniform");
                glProgramUniform3d(program, loc, value[0], value[1], value[2]);
            }
            if constexpr (std::is_same<scalar_t, double>::value && major == 4) {
                terminate_unless(type == GL_DOUBLE_VEC4, "Trying to bind non-dvec4 to dvec4 uniform");
                glProgramUniform4d(program, loc, value[0], value[1], value[2], value[3]);
            }
            if constexpr (std::is_same<scalar_t, int>::value && major == 2) {
                terminate_unless(type == GL_INT_VEC2, "Trying to bind non-ivec2 to ivec2 uniform");
                glProgramUniform2i(program, loc, value[0], value[1]);
            }
            if constexpr (std::is_same<scalar_t, int>::value && major == 3) {
                terminate_unless(type == GL_INT_VEC3, "Trying to bind non-ivec3 to ivec3 uniform");
                glProgramUniform3i(program, loc, value[0], value[1], value[2]);
            }
            if constexpr (std::is_same<scalar_t, int>::value && major == 4) {
                terminate_unless(type == GL_INT_VEC4, "Trying to bind non-ivec4 to ivec4 uniform");
                glProgramUniform4i(program, loc, value[0], value[1], value[2], value[3]);
            }
            if constexpr (std::is_same<scalar_t, unsigned int>::value && major == 2) {
                terminate_unless(type == GL_UNSIGNED_INT_VEC2, "Trying to bind non-uvec2 to uvec2 uniform");
                glProgramUniform2ui(program, loc, value[0], value[1]);
            }
            if constexpr (std::is_same<scalar_t, unsigned int>::value && major == 3) {
                terminate_unless(type == GL_UNSIGNED_INT_VEC3, "Trying to bind non-uvec3 to uvec3 uniform");
                glProgramUniform3ui(program, loc, value[0], value[1], value[2]);
            }
            if constexpr (std::is_same<scalar_t, unsigned int>::value && major == 4) {
                terminate_unless(type == GL_UNSIGNED_INT_VEC4, "Trying to bind non-uvec4 to uvec4 uniform");
                glProgramUniform4ui(program, loc, value[0], value[1], value[2], value[3]);
            }
        }
    } else {
        Eigen::Matrix<scalar_t, rows, cols> copy = value;
        Eigen::Matrix<float, rows, cols> typed_copy = copy.template cast<float>();
        if constexpr (cols == 2) {
            if constexpr (rows == 2) {
                terminate_unless(type == GL_FLOAT_MAT2 || type == GL_DOUBLE_MAT2, "Trying to bind non-(d)mat2 to (d)mat2 uniform");
                glProgramUniformMatrix2fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
            if constexpr (rows == 3) {
                terminate_unless(type == GL_FLOAT_MAT2x3 || type == GL_DOUBLE_MAT2x3, "Trying to bind non-(d)mat2x3 to (d)mat2x3 uniform");
                glProgramUniformMatrix2x3fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
            if constexpr (rows == 4) {
                terminate_unless(type == GL_FLOAT_MAT2x4 || type == GL_DOUBLE_MAT2x4, "Trying to bind non-wat2x4 to (d)mat2x4 uniform");
                glProgramUniformMatrix2x4fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
        }
        if constexpr (cols == 3) {
            if constexpr (rows == 2) {
                terminate_unless(type == GL_FLOAT_MAT3x2 || type == GL_DOUBLE_MAT3x2, "Trying to bind non-(d)mat3x2 to (d)mat3x2 uniform");
                glProgramUniformMatrix3x2fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
            if constexpr (rows == 3) {
                terminate_unless(type == GL_FLOAT_MAT3 || type == GL_DOUBLE_MAT3, "Trying to bind non-(d)mat3 to (d)mat3 uniform");
                glProgramUniformMatrix3fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
            if constexpr (rows == 4) {
                terminate_unless(type == GL_FLOAT_MAT3x4 || type == GL_DOUBLE_MAT3x4, "Trying to bind non-(d)mat3x4 to (d)mat3x4 uniform");
                glProgramUniformMatrix3x4fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
        }
        if constexpr (cols == 4) {
            if constexpr (rows == 2) {
                terminate_unless(type == GL_FLOAT_MAT4x2 || type == GL_DOUBLE_MAT4x2, "Trying to bind non-(d)mat4x2 to (d)mat4x2 uniform");
                glProgramUniformMatrix4x2fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
            if constexpr (rows == 3) {
                terminate_unless(type == GL_FLOAT_MAT4x3 || type == GL_DOUBLE_MAT4x3, "Trying to bind non-(d)mat4x3 to (d)mat4x3 uniform");
                glProgramUniformMatrix4x3fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
            if constexpr (rows == 4) {
                terminate_unless(type == GL_FLOAT_MAT4 || type == GL_DOUBLE_MAT4, "Trying to bind non-(d)mat4 to (d)mat4 uniform");
                glProgramUniformMatrix4fv(program, loc, 1, GL_FALSE, typed_copy.data());
            }
        }
    }
}

template <typename T>
inline void
bind_uniform(T&& value, GLint location, GLenum type, GLuint program) {
    bind_uniform_impl(std::forward<T>(value), location, type, program);
}

} // baldr::detail
