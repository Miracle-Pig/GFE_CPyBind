#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Post/GFE_Post.h"

namespace py = pybind11;

PYBIND11_MODULE(GFE, GFE_MOD)
{
    /* Post */
    auto POST_MOD = GFE_MOD.def_submodule("Post");
    py::class_<Post::DB>(POST_MOD, "DB")
        .def(py::init<std::shared_ptr<GFE::DB>>());
    POST_MOD.def("open", &Post::open, py::return_value_policy::reference_internal);
    POST_MOD.def("getNodeNum", &Post::getNodeNum);

    /*  Post.FO */
    auto POST_FO_MOD = POST_MOD.def_submodule("FO");
    // class
    py::class_<GFE::GridData>(POST_FO_MOD, "GridData")
        .def(py::init<>())
        .def("GetGridSize", &GFE::GridData::GetGridSize);
    // func
    POST_FO_MOD.def("GetData", &Post::FO::GetData);
    POST_FO_MOD.def("GetData_Node", &Post::FO::GetData_Node);
}