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
    ibo_ = std::make_unique<data_buffer>(indices, GL_STATIC_DRAW);
    vao_ = std::make_unique<vertex_array>();
    vao_->set_index_buffer(*ibo_);

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

    vs_ = shader_program::from_code(vs_code, GL_VERTEX_SHADER);
    pipeline_ = std::make_shared<shader_pipeline>(vs_, fs_);
    vs_->buffer_binding(*vao_, "pos") = *vbo_;
}

fullscreen_pass::~fullscreen_pass() {
}

void
fullscreen_pass::render(const vec4i_t& viewport) {
    // push
    GLboolean depth_write;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &depth_write);
    GLboolean depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    bool changed_vp = vp[0] != viewport[0]
        || vp[1] != viewport[1]
        || vp[2] != viewport[2]
        || vp[3] != viewport[3];

    // draw
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    if (changed_vp) {
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    }
    pipeline_->bind();
    vao_->bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // pop
    glDepthMask(depth_write);
    if (depth_test) {
        glEnable(GL_DEPTH_TEST);
    }
    if (changed_vp) {
        glViewport(vp[0], vp[1], vp[2], vp[3]);
    }
}

} // baldr
