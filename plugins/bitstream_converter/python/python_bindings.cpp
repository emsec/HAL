#include "hal_core/python_bindings/python_bindings.h"

#include "pybind11/operators.h"
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include "pybind11/stl_bind.h"

#include "bitstream_converter/plugin_bitstream_converter.h"

namespace py = pybind11;

namespace hal
{

    // the name in PYBIND11_MODULE/PYBIND11_PLUGIN *MUST* match the filename of the output library (without extension),
    // otherwise you will get "ImportError: dynamic module does not define module export function" when importing the module

    #ifdef PYBIND11_MODULE
    PYBIND11_MODULE(bitstream_converter, m)
    {
        m.doc() = "hal BitstreamConverterPlugin python bindings";
    #else
    PYBIND11_PLUGIN(bitstream_
    converter)
    {
        py::module m("bitstream_converter", "hal BitstreamConverterPlugin python bindings");
    #endif    // ifdef PYBIND11_MODULE

        py::class_<BitstreamConverterPlugin, RawPtrWrapper<BitstreamConverterPlugin>, BasePluginInterface>(m, "BitstreamConverterPlugin")
            .def_property_readonly("name", &BitstreamConverterPlugin::get_name)
            .def("get_name", &BitstreamConverterPlugin::get_name)
            .def_property_readonly("version", &BitstreamConverterPlugin::get_version)
            .def("get_version", &BitstreamConverterPlugin::get_version)
            ;

    #ifndef PYBIND11_MODULE
        return m.ptr();
    #endif    // PYBIND11_MODULE
    }
}
