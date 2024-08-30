#pragma once

#include <pybind11/pybind11.h>

#include "GFE_API_CPy.h"

namespace py = pybind11;

PYBIND11_MODULE(Post, GFE_Post_mod)
{
    py::class_<GFE_Post_CPy::DB_CPy>(GFE_Post_mod, "DB_CPy")
        .def(py::init<std::shared_ptr<GFE::DB>>());
    GFE_Post_mod.def("open", &GFE_Post_CPy::open, py::return_value_policy::reference_internal);
    GFE_Post_mod.def("getNodeNum", &GFE_Post_CPy::getNodeNum);
    GFE_Post_mod.def("getQuantityDCRP", &GFE_Post_CPy::getQuantityDCRP);
    GFE_Post_mod.def("add", &GFE_Post_CPy::add);
}
