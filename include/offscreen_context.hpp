#pragma once

#include "common.hpp"

namespace baldr {

class offscreen_context
{
public:
    offscreen_context(bool oneshot);

    virtual ~offscreen_context();

    void
    init(std::function<void()> f_init, std::function<void()> f_finish);

    void
    render(std::function<void()> f);

    void
    terminate();

protected:
    struct impl;
    std::unique_ptr<impl> impl_;
};

}  // namespace baldr
