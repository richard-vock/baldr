#pragma once

#include "common.hpp"
#include "shader_pipeline.hpp"

namespace baldr {

class compute_pass
{
public:
    compute_pass(std::shared_ptr<shader_program> compute_shader);

    virtual ~compute_pass();

    void
    execute(GLuint groups_x, GLuint groups_y, GLuint groups_z);

protected:
    std::shared_ptr<shader_program> shader_;
    std::shared_ptr<shader_pipeline> pipeline_;
};

}  // namespace baldr
