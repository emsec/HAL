#include "hal_core/python_bindings/python_bindings.h"

namespace hal
{
    void path_init(py::module& m)
    {
        py::class_<std::experimental::filesystem::path> py_path(m, "HalPath");

        py_path.def(py::init<>());

        py_path.def(py::init<const std::experimental::filesystem::path&>());

        py_path.def(py::init<const std::string&>()).def("__str__", [](std::experimental::filesystem::path& p) -> std::string { return std::string(p.c_str()); });

        py::implicitly_convertible<std::string, std::experimental::filesystem::path>();
    }
}    // namespace hal
