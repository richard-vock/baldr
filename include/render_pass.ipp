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

    if (opts.blend) {
        glEnable(GL_BLEND);
        GLenum blend_src = GL_ZERO, blend_tgt = GL_ZERO;
        switch (opts.blend->first) {
            case blend_mode::zero: blend_src = GL_ZERO; break;
            case blend_mode::one: blend_src = GL_ONE; break;
            case blend_mode::src_color: blend_src = GL_SRC_COLOR; break;
            case blend_mode::dst_color: blend_src = GL_DST_COLOR; break;
            case blend_mode::src_alpha: blend_src = GL_SRC_ALPHA; break;
            case blend_mode::dst_alpha: blend_src = GL_DST_ALPHA; break;
            case blend_mode::constant_color: blend_src = GL_CONSTANT_COLOR; break;
            case blend_mode::constant_alpha: blend_src = GL_CONSTANT_ALPHA; break;
            case blend_mode::one_minus_src_color: blend_src = GL_ONE_MINUS_SRC_COLOR; break;
            case blend_mode::one_minus_dst_color: blend_src = GL_ONE_MINUS_DST_COLOR; break;
            case blend_mode::one_minus_src_alpha: blend_src = GL_ONE_MINUS_SRC_ALPHA; break;
            case blend_mode::one_minus_dst_alpha: blend_src = GL_ONE_MINUS_DST_ALPHA; break;
            case blend_mode::one_minus_constant_color: blend_src = GL_ONE_MINUS_CONSTANT_COLOR; break;
            case blend_mode::one_minus_constant_alpha: blend_src = GL_ONE_MINUS_CONSTANT_ALPHA; break;
        };
        switch (opts.blend->second) {
            case blend_mode::zero: blend_src = GL_ZERO; break;
            case blend_mode::one: blend_src = GL_ONE; break;
            case blend_mode::src_color: blend_src = GL_SRC_COLOR; break;
            case blend_mode::dst_color: blend_src = GL_DST_COLOR; break;
            case blend_mode::src_alpha: blend_src = GL_SRC_ALPHA; break;
            case blend_mode::dst_alpha: blend_src = GL_DST_ALPHA; break;
            case blend_mode::constant_color: blend_src = GL_CONSTANT_COLOR; break;
            case blend_mode::constant_alpha: blend_src = GL_CONSTANT_ALPHA; break;
            case blend_mode::one_minus_src_color: blend_src = GL_ONE_MINUS_SRC_COLOR; break;
            case blend_mode::one_minus_dst_color: blend_src = GL_ONE_MINUS_DST_COLOR; break;
            case blend_mode::one_minus_src_alpha: blend_src = GL_ONE_MINUS_SRC_ALPHA; break;
            case blend_mode::one_minus_dst_alpha: blend_src = GL_ONE_MINUS_DST_ALPHA; break;
            case blend_mode::one_minus_constant_color: blend_src = GL_ONE_MINUS_CONSTANT_COLOR; break;
            case blend_mode::one_minus_constant_alpha: blend_src = GL_ONE_MINUS_CONSTANT_ALPHA; break;
        };
        glBlendFunc(blend_src, blend_tgt);
    }

    pipeline_->bind();

    for (auto && [var, tex] : opts.input) {
        fs_->sampler(var) = *tex;
    }

    for (auto && [var, tex] : opts.output) {
        std::visit(overloaded {
            [&, var=var](std::shared_ptr<texture> t) { fs_->output(var) = *t; },
            [&, var=var](texture_image img) { fs_->output(var).bind_image(*img.tex, img.level); }
        }, tex);
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

    if (opts.blend) {
        glDisable(GL_BLEND);
    }
}

} // baldr
