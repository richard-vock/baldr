namespace baldr {

template <typename... Stages>
inline
shader_pipeline::shader_pipeline(Stages&&... stages) {
    glGenProgramPipelines(1, &handle_);
    std::apply([&](auto&&... stage) { (add_stage(stage), ...); },
               std::make_tuple(std::forward<Stages>(stages)...));
}

} // baldr
