#include <baldr/framebuffer.hpp>

namespace baldr {

framebuffer::framebuffer(GLenum target) : target_(target)
{
    glCreateFramebuffers(1, &handle_);
}

framebuffer::~framebuffer() {
    if (handle_) {
        glDeleteFramebuffers(1, &handle_);
    }
}

GLuint
framebuffer::handle() const
{
    return handle_;
}

void
framebuffer::bind() {
    glBindFramebuffer(target_, handle_);
}

void
framebuffer::draw_into(GLenum buffer) {
    glNamedFramebufferDrawBuffer(handle_, buffer);
}

void
framebuffer::attach(GLenum attachment, const texture& tex, GLint level)
{
    glNamedFramebufferTexture(handle_, attachment, tex.handle(), level);
    if (attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT && attachment != GL_DEPTH_STENCIL_ATTACHMENT) {
        attachments_.insert(attachment);
        std::vector<GLenum> buffers(attachments_.begin(), attachments_.end());
        glNamedFramebufferDrawBuffers(handle_, buffers.size(), buffers.data());
    }
}

void
framebuffer::detach(GLenum attachment)
{
    auto it = attachments_.find(attachment);
    if (it == attachments_.end()) {
        fail("Trying to detach texture from framebuffer that was not attached before.");
    }
    glNamedFramebufferTexture(handle_, attachment, 0, 0);
    attachments_.erase(it);
    if (!attachments_.empty()) {
        std::vector<GLenum> buffers(attachments_.begin(), attachments_.end());
        glNamedFramebufferDrawBuffers(handle_, buffers.size(), buffers.data());
    }
}

bool
framebuffer::check() {
    // backbuffer is always complete
    return !handle_ || GL_FRAMEBUFFER_COMPLETE == glCheckNamedFramebufferStatus(handle_, target_);
}

void
framebuffer::clear_color(const vec4f_t& rgba, GLint draw_buffer) {
    float col[4];
    col[0] = rgba[0];
    col[1] = rgba[1];
    col[2] = rgba[2];
    col[3] = rgba[3];
    glClearNamedFramebufferfv(handle_, GL_COLOR, draw_buffer, col);
}

void
framebuffer::clear_depth(float depth) {
    glClearNamedFramebufferfv(handle_, GL_DEPTH, 0, &depth);
}

framebuffer::framebuffer() : target_(GL_FRAMEBUFFER), handle_(0u) {}

std::shared_ptr<framebuffer>
backbuffer() {
    return std::shared_ptr<framebuffer>(new framebuffer());
}

}  // namespace baldr
