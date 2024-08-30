cmake_minimum_required(VERSION 3.5)

project(Post)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
add_compile_options(-bigobj)
endif()

set(TRD_PARTY E:/SHW/WorkSpace/3rdParty-src-win/install)

include_directories(     
    ${TRD_PARTY}/GFE_API/include 
    ${TRD_PARTY}/GFE_API/include/Private
    ${TRD_PARTY}/GFE_Utility/include
    $ENV{TRD_ROOT}/sqlite_orm
    $ENV{TRD_ROOT}/sqlite3/include
)  
link_directories(  
    ${TRD_PARTY}/GFE_API/lib
    ${TRD_PARTY}/GFE_Utility/lib
    $ENV{TRD_ROOT}/sqlite3/lib
)  

file(GLOB HEADER
    "*.h"
)

file(GLOB SRC
    "*.cpp"
)

set(PYTHON EXECUTABLE Extern/python3.8/python.exe)
add_subdirectory(Extern/pybind11)

Pybind11_add_module(${PROJECT_NAME}
    ${HEADER}
    ${SRC}
)
target_link_libraries(${PROJECT_NAME} 
PRIVATE 
    sqlite3
    GFE_Utility
    GFE_API
)  

