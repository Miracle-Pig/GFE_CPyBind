#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int a, int b){
    return a + b;
}

PYBIND11_MODULE(Post, m)
{
    m.def("add", &add);
}