cmake_minimum_required(VERSION 3.5)

project(Post)

file(GLOB HEADER
    "*.h"
)

file(GLOB SRC
    "*.cpp"
)

set(PYTHON EXECUTABLE Extern/python3.8/python.exe)
add_subdirectory(Extern/pybind11)

Pybind11_add_module(${PROJECT_NAME} SHARED
    ${HEADER}
    ${SRC}
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}
    PRIVATE
        $ENV{TRD_ROOT}/GFE_API/include
)
target_link_directories(${PROJECT_NAME}
    PRIVATE
        $ENV{TRD_ROOT}/GFE_API/lib
)

