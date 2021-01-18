#include <compute_pass.hpp>

namespace baldr {

compute_pass::compute_pass(std::shared_ptr<shader_program> compute_shader) : shader_(compute_shader) {
    pipeline_ = std::make_shared<shader_pipeline>(shader_);
}

compute_pass::~compute_pass() {
}

void
compute_pass::execute(GLuint groups_x, GLuint groups_y, GLuint groups_z) {
    pipeline_->bind();
    glDispatchCompute(groups_x, groups_y, groups_z);
}

} // baldr
