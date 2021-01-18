#include <shader_interop.hpp>

namespace baldr::detail {

attribute_format
shader_attribute_format(GLenum type) {
    switch (type) {
        case GL_FLOAT:             return {  1, GL_FLOAT }; break;
        case GL_FLOAT_VEC2:        return {  2, GL_FLOAT }; break;
        case GL_FLOAT_VEC3:        return {  3, GL_FLOAT }; break;
        case GL_FLOAT_VEC4:        return {  4, GL_FLOAT }; break;
        case GL_DOUBLE:            return {  1, GL_DOUBLE }; break;
        case GL_DOUBLE_VEC2:       return {  2, GL_DOUBLE }; break;
        case GL_DOUBLE_VEC3:       return {  3, GL_DOUBLE }; break;
        case GL_DOUBLE_VEC4:       return {  4, GL_DOUBLE }; break;
        case GL_INT:               return {  1, GL_INT }; break;
        case GL_INT_VEC2:          return {  2, GL_INT }; break;
        case GL_INT_VEC3:          return {  3, GL_INT }; break;
        case GL_INT_VEC4:          return {  4, GL_INT }; break;
        case GL_UNSIGNED_INT:      return {  1, GL_UNSIGNED_INT }; break;
        case GL_UNSIGNED_INT_VEC2: return {  2, GL_UNSIGNED_INT }; break;
        case GL_UNSIGNED_INT_VEC3: return {  3, GL_UNSIGNED_INT }; break;
        case GL_UNSIGNED_INT_VEC4: return {  4, GL_UNSIGNED_INT }; break;
        case GL_BOOL:              return {  1, GL_BOOL }; break;
        case GL_BOOL_VEC2:         return {  2, GL_BOOL }; break;
        case GL_BOOL_VEC3:         return {  3, GL_BOOL }; break;
        case GL_BOOL_VEC4:         return {  4, GL_BOOL }; break;
        case GL_FLOAT_MAT2:        return {  4, GL_FLOAT }; break;
        case GL_FLOAT_MAT3:        return {  9, GL_FLOAT }; break;
        case GL_FLOAT_MAT4:        return { 16, GL_FLOAT }; break;
        case GL_FLOAT_MAT2x3:      return {  6, GL_FLOAT }; break;
        case GL_FLOAT_MAT2x4:      return {  8, GL_FLOAT }; break;
        case GL_FLOAT_MAT3x2:      return {  6, GL_FLOAT }; break;
        case GL_FLOAT_MAT3x4:      return { 12, GL_FLOAT }; break;
        case GL_FLOAT_MAT4x2:      return {  8, GL_FLOAT }; break;
        case GL_FLOAT_MAT4x3:      return { 12, GL_FLOAT }; break;
        case GL_DOUBLE_MAT2:       return {  4, GL_DOUBLE }; break;
        case GL_DOUBLE_MAT3:       return {  9, GL_DOUBLE }; break;
        case GL_DOUBLE_MAT4:       return { 16, GL_DOUBLE }; break;
        case GL_DOUBLE_MAT2x3:     return {  6, GL_DOUBLE }; break;
        case GL_DOUBLE_MAT2x4:     return {  8, GL_DOUBLE }; break;
        case GL_DOUBLE_MAT3x2:     return {  6, GL_DOUBLE }; break;
        case GL_DOUBLE_MAT3x4:     return { 12, GL_DOUBLE }; break;
        case GL_DOUBLE_MAT4x2:     return {  8, GL_DOUBLE }; break;
        case GL_DOUBLE_MAT4x3:     return { 12, GL_DOUBLE }; break;
        default:     return { 0, GL_FLOAT }; break;
    }
    return { 0, GL_FLOAT };
}

} // baldr::detail
