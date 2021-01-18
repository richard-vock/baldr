#include <shader_interop.hpp>

namespace baldr::detail {

attribute_format
shader_attribute_format(GLenum type) {
    switch (type) {
        case GL_FLOAT:             return { .size =  1, .type = GL_FLOAT }; break;
        case GL_FLOAT_VEC2:        return { .size =  2, .type = GL_FLOAT }; break;
        case GL_FLOAT_VEC3:        return { .size =  3, .type = GL_FLOAT }; break;
        case GL_FLOAT_VEC4:        return { .size =  4, .type = GL_FLOAT }; break;
        case GL_DOUBLE:            return { .size =  1, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_VEC2:       return { .size =  2, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_VEC3:       return { .size =  3, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_VEC4:       return { .size =  4, .type = GL_DOUBLE }; break;
        case GL_INT:               return { .size =  1, .type = GL_INT }; break;
        case GL_INT_VEC2:          return { .size =  2, .type = GL_INT }; break;
        case GL_INT_VEC3:          return { .size =  3, .type = GL_INT }; break;
        case GL_INT_VEC4:          return { .size =  4, .type = GL_INT }; break;
        case GL_UNSIGNED_INT:      return { .size =  1, .type = GL_UNSIGNED_INT }; break;
        case GL_UNSIGNED_INT_VEC2: return { .size =  2, .type = GL_UNSIGNED_INT }; break;
        case GL_UNSIGNED_INT_VEC3: return { .size =  3, .type = GL_UNSIGNED_INT }; break;
        case GL_UNSIGNED_INT_VEC4: return { .size =  4, .type = GL_UNSIGNED_INT }; break;
        case GL_BOOL:              return { .size =  1, .type = GL_BOOL }; break;
        case GL_BOOL_VEC2:         return { .size =  2, .type = GL_BOOL }; break;
        case GL_BOOL_VEC3:         return { .size =  3, .type = GL_BOOL }; break;
        case GL_BOOL_VEC4:         return { .size =  4, .type = GL_BOOL }; break;
        case GL_FLOAT_MAT2:        return { .size =  4, .type = GL_FLOAT }; break;
        case GL_FLOAT_MAT3:        return { .size =  9, .type = GL_FLOAT }; break;
        case GL_FLOAT_MAT4:        return { .size = 16, .type = GL_FLOAT }; break;
        case GL_FLOAT_MAT2x3:      return { .size =  6, .type = GL_FLOAT }; break;
        case GL_FLOAT_MAT2x4:      return { .size =  8, .type = GL_FLOAT }; break;
        case GL_FLOAT_MAT3x2:      return { .size =  6, .type = GL_FLOAT }; break;
        case GL_FLOAT_MAT3x4:      return { .size = 12, .type = GL_FLOAT }; break;
        case GL_FLOAT_MAT4x2:      return { .size =  8, .type = GL_FLOAT }; break;
        case GL_FLOAT_MAT4x3:      return { .size = 12, .type = GL_FLOAT }; break;
        case GL_DOUBLE_MAT2:       return { .size =  4, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_MAT3:       return { .size =  9, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_MAT4:       return { .size = 16, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_MAT2x3:     return { .size =  6, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_MAT2x4:     return { .size =  8, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_MAT3x2:     return { .size =  6, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_MAT3x4:     return { .size = 12, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_MAT4x2:     return { .size =  8, .type = GL_DOUBLE }; break;
        case GL_DOUBLE_MAT4x3:     return { .size = 12, .type = GL_DOUBLE }; break;
        default:     return { .size = 0, .type = GL_FLOAT }; break;
    }
    return { .size = 0, .type = GL_FLOAT };
}

} // baldr::detail
