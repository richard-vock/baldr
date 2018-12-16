#pragma once

#include "common.hpp"
#include "shader_program.hpp"

namespace baldr {

class shader_pipeline {
public:
    template <typename... Stages>
    shader_pipeline(Stages&&... stages);

    void
    add_stage(std::shared_ptr<shader_program> shader);

    void
    bind() const;

    static void
    release();

protected:
    GLuint handle_;
};

} // baldr

#include "shader_pipeline.ipp"
