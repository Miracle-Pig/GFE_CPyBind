#include <pybind11/pybind11.h>

#include "Post/GFE_Post.h"

namespace py = pybind11;

PYBIND11_MODULE(GFE, GFE_MOD)
{
    /* Post */
    auto POST_MOD = GFE_MOD.def_submodule("Post");
    py::class_<Post_CPy::DB>(POST_MOD, "DB")
        .def(py::init<std::shared_ptr<GFE::DB>>());
    POST_MOD.def("open", &Post_CPy::open, py::return_value_policy::reference_internal);
    POST_MOD.def("getNodeNum", &Post_CPy::getNodeNum);
}