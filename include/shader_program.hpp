#pragma once

#include <fstream>

#include "common.hpp"
#include "vertex_array.hpp"
#include "texture.hpp"
#include "framebuffer.hpp"

namespace baldr {

class shader_program;

struct shader_uniform
{
    template <typename T>
    const shader_uniform& operator=(T&& value) const;

    GLenum type;
    GLuint count;
    GLint location;
    GLuint binding_point;
    std::weak_ptr<shader_program> program;
};

struct shader_input
{
    GLenum type;
    GLint location;
};

struct shader_output
{
    const shader_output&
    operator=(const texture& tex) const;

    void
    bind_image(const texture& tex, int level) const;

    GLenum type;
    GLint location;
    std::weak_ptr<shader_program> program;
};

struct shader_ssbo
{
    const shader_ssbo& operator=(const data_buffer& buffer) const;

    GLuint binding_point;
};

struct sampler_unit
{
    const sampler_unit& operator=(const texture& tex) const;

    GLuint unit;
    GLint location;
    std::weak_ptr<shader_program> program;
};

struct image_unit
{
    // access is defaulted since it is assumed to be restricted inside the shader
    void
    bind(const texture& tex, int level = 0, GLenum access = GL_READ_WRITE) const;

    void
    release(const texture& tex, int level = 0) const;

    // same as above with default level/access
    const image_unit&
    operator=(const texture& tex) const;

    GLint unit;
};

struct framebuffer_depth_attachment {
    const framebuffer_depth_attachment& operator=(const texture& tex) const;
    std::weak_ptr<shader_program> program;
};

struct padding
{
    padding(GLuint bt) : bytes(bt) {}
    GLuint bytes;
};

struct normalized
{
    normalized(std::string n) : name(std::move(n)) {}
    std::string name;
};

class shader_program : public std::enable_shared_from_this<shader_program>
{
protected:
    friend struct shader_output;
    friend struct framebuffer_depth_attachment;

public:
    static std::shared_ptr<shader_program>
    load(const std::string& shader_file,
         GLuint shader_type,
         std::vector<std::string> const& include_dirs = {},
         const std::string& entry_point = "main");

    static std::shared_ptr<shader_program>
    load_binary(const std::string& shader_file, GLuint shader_type,
                const std::string& entry_point = "main");

    static std::shared_ptr<shader_program>
    load_code(const std::string& shader_file,
              GLuint shader_type,
              std::vector<std::string> const& include_dirs);

    static std::shared_ptr<shader_program>
    from_code(const std::string& code, GLuint shader_type);

    ~shader_program();

    bool valid() const;

    GLuint shader_type() const;

    GLuint shader() const;

    GLuint program() const;

    const shader_input&
    input(const std::string& name) const;

    const shader_output&
    output(const std::string& name) const;

    const shader_uniform&
    uniform(const std::string& name) const;

    const sampler_unit&
    sampler(const std::string& name) const;

    const image_unit&
    image(const std::string& name) const;

    const shader_ssbo&
    ssbo(const std::string& name) const;

    framebuffer_depth_attachment
    depth_attachment();

    template <typename... Attrs>
    binding_point
    buffer_binding(vertex_array& vao, Attrs&&... attrs);

    std::shared_ptr<const framebuffer>
    current_framebuffer() const;

    std::shared_ptr<framebuffer>
    current_framebuffer();

protected:
    shader_program();

    void
    query_output_();

    void
    query_input_();

    void
    query_uniforms_();

    void
    query_ssbo_();

    void
    attach_texture_(GLenum attachment, const texture& tex, GLint level = 0);

protected:
    GLuint shader_;
    GLuint program_;
    GLuint shader_type_;
    std::shared_ptr<framebuffer> fbo_;
    std::map<std::string, shader_uniform> uniforms_;
    std::map<std::string, sampler_unit> samplers_;
    std::map<std::string, image_unit> images_;
    std::map<std::string, shader_input> input_;
    std::map<std::string, shader_output> output_;
    std::map<std::string, shader_ssbo> ssbo_;
};

}  // namespace baldr

#include "shader_program.ipp"
