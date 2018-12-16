#include <baldr.hpp>
using namespace baldr;

#include <png++/png.hpp>

constexpr int width = 800;
constexpr int height = 600;

struct state_t {
    std::shared_ptr<shader_program> fs;
    std::shared_ptr<texture> gradient_tex;
    std::shared_ptr<texture> output_tex;
    std::shared_ptr<fullscreen_pass> pass;
};
std::unique_ptr<state_t> state;

constexpr auto init = [&] () {
    state = std::make_unique<state_t>();

    // fragment shader
    std::string fs_code = R"shader(
        #version 460

        layout(location=0) in vec2 uv;
        layout(location=0) out vec4 color;
        layout(location=0) uniform sampler2D tex;

        void main() {
            color = vec4(texture(tex, uv).rgb, 1.0);
        }
    )shader";
    state->fs = shader_program::from_code(fs_code, GL_FRAGMENT_SHADER);

    // gradient texture
    std::vector<float> image(10*10*4);
    for (uint32_t i = 0; i < 10; ++i) {
        for (uint32_t j = 0; j < 10; ++j) {
            image[j*10*4+i*4+0] = static_cast<float>(i)/10.f;
            image[j*10*4+i*4+1] = static_cast<float>(j)/10.f;
            image[j*10*4+i*4+2] = 0.f;
            image[j*10*4+i*4+3] = 1.f;
        }
    }
    state->gradient_tex = texture::rgba32f(10, 10);
    state->gradient_tex->set(image.data());
    state->fs->sampler("tex") = *state->gradient_tex;

    // draw texture
    state->output_tex = texture::rgba32f(width, height);
    state->fs->output("color") = *state->output_tex;

    // render pass
    state->pass = std::make_shared<fullscreen_pass>(state->fs);
};

constexpr auto finish = [&] () {
    state.reset();
};

constexpr auto render = [&] () {
    glViewport(0, 0, width, height);

    state->fs->current_framebuffer()->clear_color(vec4f_t(0.2f, 0.2f, 0.2f, 1.f), 0);
    state->fs->current_framebuffer()->bind();
    state->pass->render(vec4i_t(0, 0, width, height));

    std::vector<float> img_data(4*width*height, 0.f);
    state->output_tex->get(0, img_data.data());
    png::image<png::rgb_pixel> image(width, height);
    for (uint32_t j = 0; j < image.get_height(); ++j) {
        for (uint32_t i = 0; i < image.get_width(); ++i) {
            image[image.get_height() - j - 1][i] = png::rgb_pixel(
                static_cast<uint8_t>(255.f * img_data[j*width*4 + i*4 + 0]),
                static_cast<uint8_t>(255.f * img_data[j*width*4 + i*4 + 1]),
                static_cast<uint8_t>(255.f * img_data[j*width*4 + i*4 + 2])
            );
        }
    }
    image.write("output.png");
};


int main (int, char const**) {
    offscreen_context context(true);
    context.init(init, finish);
    context.render(render);
}
