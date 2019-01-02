namespace baldr {

template <typename Func>
inline void
render_pass::render(const render_options& opts, Func&& render_call) {
    if (opts.depth_test) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    glDepthMask(opts.depth_write);
    glColorMask(std::get<0>(opts.color_write), std::get<1>(opts.color_write), std::get<2>(opts.color_write), std::get<3>(opts.color_write));

    if (opts.cull_face) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }

    pipeline_->bind();

    for (auto && [var, tex] : opts.input) {
        fs_->sampler(var) = *tex;
    }

    for (auto && [var, tex] : opts.output) {
        fs_->output(var) = *tex;
    }

    if (opts.depth_attachment) {
        fs_->depth_attachment() = *opts.depth_attachment;
    }

    terminate_unless(fs_->current_framebuffer()->check(), "Incomplete framebuffer");
    fs_->current_framebuffer()->bind();
    if (opts.output.empty()) {
        GLenum clear_bits = 0;
        if (opts.clear_depth) {
            glClearDepthf(*opts.clear_depth);
            clear_bits |= GL_DEPTH_BUFFER_BIT;
        }
        if (opts.clear_color) {
            glClearColor((*opts.clear_color)[0], (*opts.clear_color)[1], (*opts.clear_color)[2], (*opts.clear_color)[3]);
            clear_bits |= GL_COLOR_BUFFER_BIT;
        }
        glClear(clear_bits);
    } else {
        if (opts.clear_depth) {
            fs_->current_framebuffer()->clear_depth(*opts.clear_depth);
        }
        if (opts.clear_color) {
            fs_->current_framebuffer()->clear_color(*opts.clear_color);
        }
    }

    render_call(vs_, fs_);
}

} // baldr
