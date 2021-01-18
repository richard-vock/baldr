#include <shader_pipeline.hpp>

namespace baldr {

void
shader_pipeline::add_stage(std::shared_ptr<shader_program> shader) {
    GLenum stage;
    switch (shader->shader_type()) {
        case GL_VERTEX_SHADER: stage = GL_VERTEX_SHADER_BIT; break;
        case GL_FRAGMENT_SHADER: stage = GL_FRAGMENT_SHADER_BIT; break;
        case GL_GEOMETRY_SHADER: stage = GL_GEOMETRY_SHADER_BIT; break;
        case GL_COMPUTE_SHADER: stage = GL_COMPUTE_SHADER_BIT; break;
        case GL_TESS_CONTROL_SHADER: stage = GL_TESS_CONTROL_SHADER_BIT; break;
        default: stage = GL_TESS_EVALUATION_SHADER_BIT; break;
    }
    glUseProgramStages(handle_, stage, shader->program());
}

void
shader_pipeline::bind() const {
    glBindProgramPipeline(handle_);
}

void
shader_pipeline::release() {
    glBindProgramPipeline(0);
}

} // baldr
