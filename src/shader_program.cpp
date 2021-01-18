#include <shader_program.hpp>

#include <filesystem>
#include <string_view>
#include <fstream>
#include <set>
#include <streambuf>
#include <ctre.hpp>

namespace fs = std::filesystem;

namespace {

bool
is_sampler(GLenum type)
{
    switch (type) {
        case GL_SAMPLER_1D:
        case GL_SAMPLER_2D:
        case GL_SAMPLER_3D:
        case GL_SAMPLER_CUBE:
        case GL_SAMPLER_1D_SHADOW:
        case GL_SAMPLER_2D_SHADOW:
        case GL_SAMPLER_CUBE_SHADOW:
        case GL_SAMPLER_1D_ARRAY:
        case GL_SAMPLER_2D_ARRAY:
        case GL_SAMPLER_1D_ARRAY_SHADOW:
        case GL_SAMPLER_2D_ARRAY_SHADOW:
        case GL_SAMPLER_2D_MULTISAMPLE:
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_SAMPLER_BUFFER:
        case GL_SAMPLER_2D_RECT:
        case GL_SAMPLER_2D_RECT_SHADOW:
        case GL_INT_SAMPLER_1D:
        case GL_INT_SAMPLER_2D:
        case GL_INT_SAMPLER_3D:
        case GL_INT_SAMPLER_CUBE:
        case GL_INT_SAMPLER_1D_ARRAY:
        case GL_INT_SAMPLER_2D_ARRAY:
        case GL_INT_SAMPLER_2D_MULTISAMPLE:
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_INT_SAMPLER_BUFFER:
        case GL_INT_SAMPLER_2D_RECT:
        case GL_UNSIGNED_INT_SAMPLER_1D:
        case GL_UNSIGNED_INT_SAMPLER_2D:
        case GL_UNSIGNED_INT_SAMPLER_3D:
        case GL_UNSIGNED_INT_SAMPLER_CUBE:
        case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE:
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY:
        case GL_UNSIGNED_INT_SAMPLER_BUFFER:
        case GL_UNSIGNED_INT_SAMPLER_2D_RECT:
            return true;
        default:
            return false;
    }
}

bool
is_image(GLenum type)
{
    switch (type) {
        case GL_IMAGE_1D:
        case GL_IMAGE_2D:
        case GL_IMAGE_3D:
        case GL_IMAGE_2D_RECT:
        case GL_IMAGE_CUBE:
        case GL_IMAGE_BUFFER:
        case GL_IMAGE_1D_ARRAY:
        case GL_IMAGE_2D_ARRAY:
        case GL_IMAGE_2D_MULTISAMPLE:
        case GL_IMAGE_2D_MULTISAMPLE_ARRAY:
        case GL_INT_IMAGE_1D:
        case GL_INT_IMAGE_2D:
        case GL_INT_IMAGE_3D:
        case GL_INT_IMAGE_2D_RECT:
        case GL_INT_IMAGE_CUBE:
        case GL_INT_IMAGE_BUFFER:
        case GL_INT_IMAGE_1D_ARRAY:
        case GL_INT_IMAGE_2D_ARRAY:
        case GL_INT_IMAGE_2D_MULTISAMPLE:
        case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
        case GL_UNSIGNED_INT_IMAGE_1D:
        case GL_UNSIGNED_INT_IMAGE_2D:
        case GL_UNSIGNED_INT_IMAGE_3D:
        case GL_UNSIGNED_INT_IMAGE_2D_RECT:
        case GL_UNSIGNED_INT_IMAGE_CUBE:
        case GL_UNSIGNED_INT_IMAGE_BUFFER:
        case GL_UNSIGNED_INT_IMAGE_1D_ARRAY:
        case GL_UNSIGNED_INT_IMAGE_2D_ARRAY:
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE:
        case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY:
            return true;
        default:
            return false;
    }
}

std::string
preprocess_shaders(std::string entry_file, std::set<std::string> & already_included, std::vector<std::string> const& include_dirs) {
    std::string accum;
    if (std::ifstream in(entry_file); in.good()) {
        std::string line;
        while (std::getline(in, line)) {
            if (auto m = ctre::match<"\\s*[#]\\s*version\\s*(?<version>.+)">(line)) {
                std::string_view version = m.get<"version">();
                fmt::print("found version {} in file {}\n", version, entry_file);
            }
            if (auto m = ctre::match<"\\s*[#]\\s*include\\s*[<\"](?<path>.+)[>\"].*">(line)) {
                std::string_view filename = m.get<"path">();
                if (already_included.contains(std::string(filename))) {
                    continue;
                }
                bool found = false;
                for (auto dir : include_dirs) {
                    fs::path candidate = fs::path(dir) / std::string(filename);
                    if (fs::exists(candidate)) {
                        accum += preprocess_shaders(candidate, already_included, include_dirs);
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    baldr::fail("Unable to find included shader file: {}", filename);
                }
            } else {
                accum += line + "\n";
            }
        }
    } else {
        baldr::fail("Cannot open shader file for reading: {}", entry_file);
    }

    return accum;
}

std::string
preprocess_shaders(std::string entry_file, std::vector<std::string> const& include_dirs) {
    std::set<std::string> already_included;
    return preprocess_shaders(entry_file, already_included, include_dirs);
}

}  // namespace

namespace baldr {

const sampler_unit&
sampler_unit::operator=(const texture& tex) const
{
    auto prog = program.lock();
    glBindTextureUnit(unit, tex.handle());
    glProgramUniform1i(prog->program(), location, unit);
    return *this;
}

void
image_unit::bind(const texture& tex, int level, GLenum access) const
{
    glBindImageTexture(unit, tex.handle(), level, GL_FALSE, 0, access, tex.specification().internal_format);
}

void
image_unit::release(const texture& tex, int level) const
{
    glBindImageTexture(unit, 0, level, GL_FALSE, 0, GL_READ_WRITE, tex.specification().internal_format);
}

const image_unit&
image_unit::operator=(const texture& tex) const
{
    glBindImageTexture(unit, tex.handle(), 0, GL_FALSE, 0, GL_READ_WRITE, tex.specification().internal_format);
    return *this;
}

const shader_output&
shader_output::operator=(const texture& tex) const {
    auto prog = program.lock();
    prog->attach_texture_(GL_COLOR_ATTACHMENT0 + location, tex);
    return *this;
}

void
shader_output::bind_image(const texture& tex, int level) const {
    auto prog = program.lock();
    prog->attach_texture_(GL_COLOR_ATTACHMENT0 + location, tex, level);
}

const framebuffer_depth_attachment&
framebuffer_depth_attachment::operator=(const texture& tex) const {
    auto prog = program.lock();
    prog->attach_texture_(GL_DEPTH_ATTACHMENT, tex);
    return *this;
}

const shader_ssbo&
shader_ssbo::operator=(const data_buffer& buffer) const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, buffer.handle());
    return *this;
}

std::shared_ptr<shader_program>
shader_program::load(const std::string& shader_file,
                     GLuint shader_type,
                     std::vector<std::string> const& include_dirs,
                     const std::string& entry_point)
{
    fs::path shader_path(shader_file);
    if (!fs::exists(shader_path)) {
        fail("Shader file does not exist: {}", shader_path);
        return nullptr;
    }

    if (shader_path.extension() == ".spv") {
        return shader_program::load_binary(shader_file, shader_type, entry_point);
    } else {
        return shader_program::load_code(shader_file, shader_type, include_dirs);
    }
}

std::shared_ptr<shader_program>
shader_program::from_code(const std::string& code, GLuint shader_type)
{
    std::shared_ptr<shader_program> self(new shader_program());
    self->shader_type_ = shader_type;

    self->shader_ = glCreateShader(shader_type);
    const GLchar* source = (const GLchar*)code.c_str();
    glShaderSource(self->shader_, 1, &source, 0);
    glCompileShader(self->shader_);

    GLint compiled = 0;
    glGetShaderiv(self->shader_, GL_COMPILE_STATUS, &compiled);
    if(compiled == GL_FALSE)
    {
        GLint len = 0;
        glGetShaderiv(self->shader_, GL_INFO_LOG_LENGTH, &len);

        std::string log(len, ' ');
        glGetShaderInfoLog(self->shader_, len, &len, log.data());
        fail("Unable to compile shader.\nLog: \n{}", log);
    }


    self->program_ = glCreateProgram();
    glProgramParameteri(self->program_, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glAttachShader(self->program_, self->shader_);
    glLinkProgram(self->program_);
    glDetachShader(self->program_, self->shader_);

    self->fbo_ = backbuffer();

    GLint linked = 0;
    glGetProgramiv(self->program_, GL_LINK_STATUS, (int *)&linked);
    if (linked == GL_FALSE)
    {
        GLint len = 0;
        glGetProgramiv(self->program_, GL_INFO_LOG_LENGTH, &len);

        std::string log(len, ' ');
        glGetProgramInfoLog(self->program_, len, &len, log.data());
        fail("Unable to link shader program.\nLog: \n{}", log);
    }

    self->query_uniforms_();
    self->query_input_();
    self->query_output_();
    self->query_ssbo_();

    return self;
}

shader_program::~shader_program()
{
    if (shader_) {
        glDeleteShader(shader_);
    }
    if (program_) {
        glDeleteProgram(program_);
    }
}

shader_program::shader_program() : shader_(0), program_(0) {}

std::shared_ptr<shader_program>
shader_program::load_binary(const std::string& shader_file,
                            GLuint shader_type,
                            const std::string& entry_point)
{
    std::shared_ptr<shader_program> self(new shader_program());
    self->shader_type_ = shader_type;

    if (std::ifstream in(shader_file,
                         std::ios::ate | std::ios::binary);
        in.good()) {
        std::streamsize size = in.tellg();
        in.seekg(0, std::ios::beg);
        std::vector<char> content(size);
        in.read(content.data(), size);

        self->shader_ = glCreateShader(shader_type);
        glShaderBinary(1, &self->shader_, GL_SHADER_BINARY_FORMAT_SPIR_V,
                       content.data(), content.size());
        glSpecializeShaderARB(self->shader_, entry_point.c_str(), 0, nullptr, nullptr);
        self->program_ = glCreateProgram();
        glProgramParameteri(self->program_, GL_PROGRAM_SEPARABLE, GL_TRUE);
        glAttachShader(self->program_, self->shader_);
        glLinkProgram(self->program_);
        glDetachShader(self->program_, self->shader_);

        self->fbo_ = backbuffer();

        self->query_uniforms_();
        self->query_input_();
        self->query_output_();
        self->query_ssbo_();
    } else {
        fail("Unable to read shader file \"{}\"", shader_file);
    }

    return self;
}

std::shared_ptr<shader_program>
shader_program::load_code(const std::string& shader_file,
                          GLuint shader_type,
                          std::vector<std::string> const& include_dirs)
{
    std::string code = preprocess_shaders(shader_file, include_dirs);
    // fmt::print("final code: \n{}\n", code);
    return shader_program::from_code(code, shader_type);
}

void
shader_program::query_output_()
{
    GLint count = 0;
    glGetProgramInterfaceiv(program_, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES,
                            &count);
    std::array<GLenum, 3> properties = {GL_TYPE, GL_NAME_LENGTH, GL_LOCATION};

    output_.clear();
    for (GLint idx = 0; idx < count; ++idx) {
        std::array<GLint, 3> values;
        glGetProgramResourceiv(program_, GL_PROGRAM_OUTPUT, idx,
                               properties.size(), properties.data(), 255, NULL,
                               values.data());
        auto&& [type, name_len, loc] = values;
        std::string name(name_len - 1, ' ');
        glGetProgramResourceName(program_, GL_PROGRAM_OUTPUT, idx, name_len,
                                 &name_len, name.data());

        output_[name] = {.type = static_cast<GLenum>(type), .location = loc, .program = weak_from_this()};
    }
}

void
shader_program::query_input_()
{
    GLint count = 0;
    glGetProgramInterfaceiv(program_, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES,
                            &count);
    std::array<GLenum, 3> properties = {GL_TYPE, GL_NAME_LENGTH, GL_LOCATION};

    input_.clear();
    for (GLint idx = 0; idx < count; ++idx) {
        std::array<GLint, 3> values;
        glGetProgramResourceiv(program_, GL_PROGRAM_INPUT, idx,
                               properties.size(), properties.data(), 255, NULL,
                               values.data());
        auto&& [type, name_len, loc] = values;
        std::string name(name_len - 1, ' ');
        glGetProgramResourceName(program_, GL_PROGRAM_INPUT, idx, name_len,
                                 &name_len, name.data());

        input_[name] = {.type = static_cast<GLenum>(type), .location = loc};
    }
}

void
shader_program::query_uniforms_()
{
    GLint count = 0;
    glGetProgramInterfaceiv(program_, GL_UNIFORM, GL_ACTIVE_RESOURCES, &count);
    std::array<GLenum, 4> properties = {GL_TYPE, GL_ARRAY_SIZE, GL_NAME_LENGTH,
                                        GL_LOCATION};

    GLuint tex_unit = 0, atomic_counter = 0;
    uniforms_.clear();
    for (GLint idx = 0; idx < count; ++idx) {
        std::array<GLint, 4> values;
        glGetProgramResourceiv(program_, GL_UNIFORM, idx, properties.size(),
                               properties.data(), 255, NULL, values.data());
        auto&& [type, element_count, name_len, loc] = values;
        std::string name(name_len - 1, ' ');
        glGetProgramResourceName(program_, GL_UNIFORM, idx, name_len, &name_len,
                                 name.data());

        GLuint binding_point = loc;
        if (is_sampler(type)) {
            samplers_[name] = {.unit = tex_unit++,
                               .location = loc,
                               .program = weak_from_this()};
            continue;
        } else if (is_image(type)) {
            GLint unit = -1;
            glGetUniformiv(program_, loc, &unit);
            if (unit >= 0) {
                images_[name] = {.unit = unit };
            }
            continue;
        } else if (is_atomic_counter(type)) {
            GLint bp;
            glGetActiveAtomicCounterBufferiv(program_, atomic_counter,
                                             GL_ATOMIC_COUNTER_BUFFER_BINDING,
                                             &bp);
            binding_point = static_cast<GLuint>(bp);
            ++atomic_counter;
        }

        uniforms_[name] = {.type = static_cast<GLenum>(type),
                           .count = static_cast<GLuint>(element_count),
                           .location = loc,
                           .binding_point = binding_point,
                           .program = weak_from_this()};
    }
}

void
shader_program::query_ssbo_()
{
    GLint count = 0;
    glGetProgramInterfaceiv(program_, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &count);
    std::array<GLenum, 2> properties = {GL_BUFFER_BINDING, GL_NAME_LENGTH};

    //GLuint tex_unit = 0, img_unit = 0, atomic_counter = 0;
    //uniforms_.clear();
    for (GLint idx = 0; idx < count; ++idx) {
        std::array<GLint, 2> values;
        glGetProgramResourceiv(program_, GL_SHADER_STORAGE_BLOCK, idx, properties.size(),
                               properties.data(), 255, NULL, values.data());
        auto&& [binding, name_len] = values;
        std::string name(name_len - 1, ' ');
        glGetProgramResourceName(program_, GL_SHADER_STORAGE_BLOCK, idx, name_len, &name_len,
                                 name.data());

        ssbo_[name] = shader_ssbo{.binding_point = static_cast<GLuint>(binding)};
    }
}

void
shader_program::attach_texture_(GLenum attachment, const texture& tex, GLint level) {
    if (shader_type_ != GL_FRAGMENT_SHADER) return;
    if (!fbo_->handle()) {
        // still backbuffer bound - create real FBO
        fbo_ = std::make_shared<framebuffer>(GL_FRAMEBUFFER);
    }
    fbo_->attach(attachment, tex, level);
}

}  // namespace baldr
