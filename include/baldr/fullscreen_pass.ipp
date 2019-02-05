namespace baldr {

namespace detail {

inline void
set_uniforms(std::shared_ptr<shader_program>) {
}

template <typename T, typename... Uniforms>
inline void
set_uniforms(std::shared_ptr<shader_program> fs, const std::string& name, T&& value, Uniforms&&... uniforms) {
    fs->uniform(name) = value;
    set_uniforms(fs, std::forward<Uniforms>(uniforms)...);
}

}  // namespace detail

template <typename... Uniforms>
inline void
fullscreen_pass::render(const render_options& opts, Uniforms&&... uniforms)
{
    detail::set_uniforms(fs_, std::forward<Uniforms>(uniforms)...);
    render_options opt = opts;
    opt.depth_test = false;
    pass_->render(opt, [&](auto, auto) {
        vao_->bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    });
}

}  // namespace baldr
