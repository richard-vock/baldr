#pragma once

#include "common.hpp"
#include "texture.hpp"
#include <set>

namespace baldr {

class framebuffer;

std::shared_ptr<framebuffer>
backbuffer();

class framebuffer
{
protected:
    friend std::shared_ptr<framebuffer>
    backbuffer();

public:
    framebuffer(GLenum target);

    virtual ~framebuffer();

    GLuint
    handle() const;

    void
    bind();

    void
    draw_into(GLenum buffer);

    void
    attach(GLenum attachment, const texture& tex, GLint level = 0);

    void
    detach(GLenum attachment);

    [[nodiscard]]
    bool
    check();

    void
    clear_color(const vec4f_t& rgba, GLint draw_buffer = 0);

    void
    clear_depth(float depth);

protected:
    framebuffer();

protected:
    GLenum target_;
    GLuint handle_;
    std::set<GLenum> attachments_;
};

}  // namespace baldr
