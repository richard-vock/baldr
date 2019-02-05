#pragma once

#include "common.hpp"
#include "shader_program.hpp"
#include "data_buffer.hpp"
#include "vertex_array.hpp"
#include "render_pass.hpp"

namespace baldr {

class fullscreen_pass
{
public:
    fullscreen_pass(std::shared_ptr<shader_program> fragment_shader);

    virtual ~fullscreen_pass();

    template <typename... Uniforms>
    void
    render(const render_options& opts, Uniforms&&... uniforms);

protected:
    std::shared_ptr<shader_program> fs_;
    std::unique_ptr<data_buffer> vbo_;
    std::shared_ptr<data_buffer> ibo_;
    std::unique_ptr<vertex_array> vao_;
    std::unique_ptr<render_pass> pass_;
};

}  // namespace baldr

#include "fullscreen_pass.ipp"
