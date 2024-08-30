cmake_minimum_required(VERSION 3.5)

project(GFE_CPyBind)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
add_compile_options(-bigobj)
endif()

set(TRD_PARTY E:/SHW/WorkSpace/3rdParty-src-win/install)
set(MOD_NAME GFE)
set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/install)

include_directories(
    ${PROJECT_SOURCE_DIR}     
    ${TRD_PARTY}/GFE_API/include 
    ${TRD_PARTY}/GFE_API/include/Private
    ${TRD_PARTY}/GFE_API/include/GFE_Struct
    ${TRD_PARTY}/GFE_Utility/include
    $ENV{TRD_ROOT}/sqlite_orm
    $ENV{TRD_ROOT}/sqlite3/include
)  
link_directories(  
    ${TRD_PARTY}/GFE_API/lib
)  

file(GLOB HEADER
    "*.h"
    "Post/*.h"
    "Post/Private/*.h"
    "Post/GFE_Struct/*.h"
)

file(GLOB SRC
    "*.cpp"
    "Post/*.cpp"
    "Post/Private/*.cpp"
    "Post/GFE_Struct/*.cpp"
)

set(PYTHON EXECUTABLE Extern/python3.8/python.exe)
add_subdirectory(Extern/pybind11)

Pybind11_add_module(${MOD_NAME}
    ${HEADER}
    ${SRC}
)
target_link_libraries(${MOD_NAME} 
PRIVATE 
    GFE_API
)  
install(TARGETS ${MOD_NAME}
    DESTINATION ${INSTALL_DIR}
)