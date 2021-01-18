#pragma once

#include "common.hpp"
#include "shader_pipeline.hpp"
#include "texture.hpp"

namespace baldr {

struct texture_image {
    std::shared_ptr<texture> tex;
    int level;
};

enum class blend_mode : int {
    zero,
    one,
    src_color,
    dst_color,
    src_alpha,
    dst_alpha,
    constant_color,
    constant_alpha,
    one_minus_src_color,
    one_minus_dst_color,
    one_minus_src_alpha,
    one_minus_dst_alpha,
    one_minus_constant_color,
    one_minus_constant_alpha
};

struct render_options {
    std::vector<std::pair<std::string, std::shared_ptr<texture>>> input = {};
    std::vector<std::pair<std::string, std::variant<std::shared_ptr<texture>, texture_image>>> output = {};
    std::shared_ptr<texture> depth_attachment = nullptr;
    bool depth_test = true;
    bool depth_write = true;
    bool cull_face = false;
    std::tuple<bool, bool, bool, bool> color_write = std::tuple(true, true, true, true);
    std::optional<float> clear_depth = std::nullopt;
    std::optional<vec4f_t> clear_color = std::nullopt;
    std::optional<std::pair<blend_mode, blend_mode>> blend = std::nullopt;
};

class render_pass
{
public:
    render_pass(std::shared_ptr<shader_program> vertex_shader, std::shared_ptr<shader_program> fragment_shader);

    virtual ~render_pass();

    template <typename Func>
    void
    render(const render_options& opts, Func&& func);

protected:
    std::shared_ptr<shader_program> vs_;
    std::shared_ptr<shader_program> fs_;
    std::shared_ptr<shader_pipeline> pipeline_;
};

}  // namespace baldr

#include "render_pass.ipp"
