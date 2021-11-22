project(MeowGL)

add_subdirectory(gl3w)

add_library(MeowGL STATIC 
    MeowGLRenderer.cpp
)
target_link_libraries(MeowGL gl3w glfw)
target_include_directories(MeowGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})