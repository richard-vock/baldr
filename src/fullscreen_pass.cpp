#include <baldr/fullscreen_pass.hpp>

namespace baldr {

fullscreen_pass::fullscreen_pass(std::shared_ptr<shader_program> fragment_shader) : fs_(fragment_shader) {
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> vertex_data(4, 2);
    vertex_data <<
         -1.0f, -1.0f,
          1.0f, -1.0f,
          1.0f,  1.0f,
         -1.0f,  1.0f;
    std::vector<uint32_t> indices = {0, 1, 2, 2, 3, 0};
    vbo_ = std::make_unique<data_buffer>(vertex_data, GL_STATIC_DRAW);
    ibo_ = std::make_shared<data_buffer>(indices, GL_STATIC_DRAW);
    vao_ = std::make_unique<vertex_array>();
    vao_->set_index_buffer(ibo_);

    std::string vs_code = R"shader(
        #version 460

        layout(location=0) in vec2 pos;

        layout(location=0) out gl_PerVertex
        {
            vec4 gl_Position;
            float gl_PointSize;
            float gl_ClipDistance[];
        };

        layout(location=0) out vec2 uv;

        void main() {
            gl_Position = vec4(pos, 0, 1);
            uv = 0.5 * (pos.xy + vec2(1.0));
        }
    )shader";

    auto vs = shader_program::from_code(vs_code, GL_VERTEX_SHADER);
    vs->buffer_binding(*vao_, "pos") = *vbo_;
    pass_ = std::make_unique<render_pass>(vs, fs_);
}

fullscreen_pass::~fullscreen_pass() {
}

} // baldr
