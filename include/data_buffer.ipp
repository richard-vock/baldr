namespace baldr {

template <typename T>
data_buffer::data_buffer(const std::vector<T>& data, GLenum usage_hint)
    : data_buffer(data.size() * sizeof(T), usage_hint,
                  reinterpret_cast<const void*>(data.data()))
{}

template <typename Derived>
data_buffer::data_buffer(const Eigen::PlainObjectBase<Derived>& data,
                         GLenum usage_hint)
    : data_buffer(data.size() *
                      sizeof(typename Eigen::PlainObjectBase<Derived>::Scalar),
                  usage_hint, reinterpret_cast<const void*>(data.data()))
{}

template <typename T>
void
data_buffer::set_data(const std::vector<T>& data)
{
    set_data(reinterpret_cast<const void*>(data.data()));
}

template <typename T>
void
data_buffer::get_data(std::vector<T>& data) const
{
    get_data(reinterpret_cast<void*>(data.data()));
}

}  // namespace baldr
