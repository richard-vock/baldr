#pragma once

#include "common.hpp"
#include "shader_program.hpp"
#include "shader_pipeline.hpp"
#include "data_buffer.hpp"
#include "vertex_array.hpp"

namespace baldr {

class fullscreen_pass
{
public:
    fullscreen_pass(std::shared_ptr<shader_program> fragment_shader);

    virtual ~fullscreen_pass();

    void
    render(const vec4i_t& viewport);

protected:
    std::shared_ptr<shader_program> fs_;
    std::shared_ptr<shader_program> vs_;
    std::shared_ptr<shader_pipeline> pipeline_;

    std::unique_ptr<data_buffer> vbo_;
    std::unique_ptr<data_buffer> ibo_;
    std::unique_ptr<vertex_array> vao_;
};

}  // namespace baldr
