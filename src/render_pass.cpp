#include <render_pass.hpp>

namespace baldr {

render_pass::render_pass(std::shared_ptr<shader_program> vertex_shader, std::shared_ptr<shader_program> fragment_shader) : vs_(vertex_shader), fs_(fragment_shader) {
    pipeline_ = std::make_shared<shader_pipeline>(vs_, fs_);
}

render_pass::~render_pass() {
}

} // baldr
