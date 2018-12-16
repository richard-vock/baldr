#include <offscreen_context.hpp>

#include <condition_variable>
#include <thread>
#include <mutex>

#include <GLFW/glfw3.h>

namespace {

static void
gl_error_callback(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei,
                  const GLchar* message, const void*)
{
    bool error = false;
    if (type == GL_DEBUG_TYPE_ERROR ||
        type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR ||
        severity == GL_DEBUG_SEVERITY_HIGH) {
        error = true;
    }
    bool is_verbose = severity == GL_DEBUG_SEVERITY_LOW ||
                      severity == GL_DEBUG_SEVERITY_NOTIFICATION;
    std::string src;
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            src = "OpenGL API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            src = "Window system";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            src = "Shader compilation";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            src = "GL application";
            break;
        default:
            src = "Misc";
            break;
    }

    if (error) {
        baldr::perror("{} error:\n{}", src, message);
        std::terminate();
    } else {
        if (is_verbose) {
            if constexpr (baldr::debug_verbose) {
                baldr::pverbose("{} notification:\n{}", src, message);
            }
        } else {
            baldr::pdebug("{} info:\n{}", src, message);
        }
    }
}

}

namespace baldr {

struct offscreen_context::impl
{
    impl(bool oneshot);

    ~impl();

    void
    init(std::function<void()> f_init, std::function <void ()> f_finish);

    void
    render(std::function<void()> f);

    bool
    loop_finished();

    void
    terminate();

    bool oneshot_;
    bool quit_;

    std::mutex init_mutex_;
    std::condition_variable init_cv_;
    bool start_init_;

    std::mutex render_mutex_;
    std::condition_variable render_cv_;
    bool start_render_;

    std::mutex finish_mutex_;

    std::function<void()> init_func_;
    std::function<void()> finish_func_;
    std::function<void()> render_func_;

    std::unique_ptr<std::thread> main_loop_;
    GLFWwindow* window_;
};

offscreen_context::offscreen_context(bool oneshot)
{
    impl_ = std::make_unique<impl>(oneshot);
}

offscreen_context::~offscreen_context() {
}

void
offscreen_context::init(std::function<void()> f_init, std::function<void()> f_finish)
{
    impl_->init(std::move(f_init), std::move(f_finish));
}

void
offscreen_context::render(std::function<void()> f)
{
    impl_->render(std::move(f));
}

void
offscreen_context::terminate()
{
    impl_->terminate();
}

offscreen_context::impl::impl(bool oneshot) : oneshot_(oneshot), quit_(false) {
    main_loop_ = std::make_unique<std::thread>([&]() {
        glfwSetErrorCallback([](int error, const char* description) {
            perror("GLFW Error {}: {}", error, description);
        });

        terminate_unless(glfwInit(), "Unable to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

        window_ = glfwCreateWindow(640, 480, "", NULL, NULL);
        terminate_unless(window_ != NULL, "Unable to create window");
        glfwMakeContextCurrent(window_);

        terminate_unless(GLEW_OK == glewInit(), "Unable to initialize GLEW");

        if constexpr (debug) {
            if (GLFW_TRUE ==
                glfwGetWindowAttrib(window_, GLFW_OPENGL_DEBUG_CONTEXT)) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(&gl_error_callback, nullptr);
            }
        }

        std::unique_lock<std::mutex> lk_init(init_mutex_);
        while (!start_init_) {
            init_cv_.wait(lk_init);
        }
        init_func_();


        std::unique_lock<std::mutex> lk_render(render_mutex_);
        while (!start_render_) {
            render_cv_.wait(lk_render);
        }

        while (!loop_finished()) {
            glfwPollEvents();
            render_func_();
            glfwSwapBuffers(window_);

            if (oneshot_) this->terminate();
        }

        finish_func_();

        glfwDestroyWindow(window_);
        glfwTerminate();
    });
}

offscreen_context::impl::~impl() {
    if (main_loop_) {
        main_loop_->join();
    }
}

void
offscreen_context::impl::init(std::function<void()> f_init, std::function<void()> f_finish)
{
    init_func_ = std::move(f_init);
    finish_func_ = std::move(f_finish);
    std::unique_lock lk(init_mutex_);
    start_init_ = true;
    init_cv_.notify_one();
}

void
offscreen_context::impl::render(std::function<void()> f)
{
    render_func_ = std::move(f);
    std::unique_lock lk(render_mutex_);
    start_render_ = true;
    render_cv_.notify_one();
}

bool
offscreen_context::impl::loop_finished()
{
    std::lock_guard<std::mutex> lg(finish_mutex_);
    return quit_;
}

void
offscreen_context::impl::terminate()
{
    std::lock_guard<std::mutex> lg(finish_mutex_);
    quit_ = true;
}

}  // namespace baldr
