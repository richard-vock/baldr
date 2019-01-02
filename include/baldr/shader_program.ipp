#include "shader_interop.hpp"


namespace baldr {

inline void
add_binding_format_input(GLuint& offset, std::vector<vertex_attribute>& vinfo, shader_input inp, GLboolean normalized) {
    detail::attribute_format fmt = detail::shader_attribute_format(inp.type);
    vinfo.push_back({
        .index = static_cast<GLuint>(inp.location),
        .size = fmt.size,
        .type = fmt.type,
        .offset = offset,
        .normalized = normalized
    });
    offset += fmt.size * sizeof(fmt.type);
}

inline void
add_binding_format(const std::map<std::string, shader_input>& inputs, GLuint& offset, std::vector<vertex_attribute>& vinfo, std::string inp) {
    auto find_it = inputs.find(inp);
    terminate_unless(find_it != inputs.end(), "Input \"{}\" not an active shader input", inp);
    add_binding_format_input(offset, vinfo, find_it->second, GL_FALSE);
}

inline void
add_binding_format(const std::map<std::string, shader_input>& inputs, GLuint& offset, std::vector<vertex_attribute>& vinfo, normalized nrm) {
    auto find_it = inputs.find(nrm.name);
    terminate_unless(find_it != inputs.end(), "Input \"{}\" not an active shader input", nrm.name);
    add_binding_format_input(offset, vinfo, find_it->second, GL_TRUE);
}

inline void
add_binding_format(const std::map<std::string, shader_input>&, GLuint& offset, std::vector<vertex_attribute>&, padding pad) {
    offset += pad.bytes;
}

inline bool
is_atomic_counter(GLenum type)
{
    return type == GL_UNSIGNED_INT_ATOMIC_COUNTER;
}

template <typename T>
inline const shader_uniform&
shader_uniform::operator=(T&& value) const {
    auto prog = program.lock();
    detail::bind_uniform(std::forward<T>(value), location, type, prog->program());

    return *this;
}

inline bool
shader_program::valid() const
{
    return shader_ && program_;
}

inline GLuint
shader_program::shader_type() const {
    return shader_type_;
}

inline GLuint
shader_program::shader() const
{
    return shader_;
}

inline GLuint
shader_program::program() const
{
    return program_;
}

inline const shader_input&
shader_program::input(const std::string& name) const {
    auto find_it = input_.find(name);
    if (find_it == input_.end()) {
        fail("Input \"{}\" not an active shader input", name);
    }
    return find_it->second;
}

inline const shader_output&
shader_program::output(const std::string& name) const {
    auto find_it = output_.find(name);
    if (find_it == output_.end()) {
        fail("Output \"{}\" not an active shader output", name);
    }
    return find_it->second;
}

inline const shader_uniform&
shader_program::uniform(const std::string& name) const {
    auto find_it = uniforms_.find(name);
    if (find_it == uniforms_.end()) {
        fail("Uniform \"{}\" not an active shader uniform", name);
    }
    return find_it->second;
}

inline const sampler_unit&
shader_program::sampler(const std::string& name) const {
    auto find_it = samplers_.find(name);
    if (find_it == samplers_.end()) {
        fail("Texture unit \"{}\" not an active shader sampler", name);
    }
    return find_it->second;
}

inline framebuffer_depth_attachment
shader_program::depth_attachment() {
    return { weak_from_this() };
}

inline std::shared_ptr<const framebuffer>
shader_program::current_framebuffer() const {
    return fbo_;
}

inline std::shared_ptr<framebuffer>
shader_program::current_framebuffer() {
    return fbo_;
}

template <typename... Attrs>
inline binding_point
shader_program::buffer_binding(vertex_array& vao, Attrs&&... attrs) {
    std::vector<vertex_attribute> vinfo;
    GLuint offset = 0;
    std::apply([&](auto&&... attr) { (add_binding_format(input_, offset, vinfo, attr), ...); },
               std::make_tuple(std::forward<Attrs>(attrs)...));
    return vao.vertex_buffer_binding(vinfo, offset);
}

} // baldr
