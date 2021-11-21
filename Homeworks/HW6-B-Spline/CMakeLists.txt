cmake_minimum_required(VERSION 3.5)
project(HW3-Bezier-Bernstein)

# -- glfw --
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)

# -- opengl --
find_package(OpenGL REQUIRED)


# -- imgui --
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
set(IMGUI_FILES
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
    "${IMGUI_DIR}/imconfig.h"
    "${IMGUI_DIR}/imgui.h"
    "${IMGUI_DIR}/imgui_internal.h"
    "${IMGUI_DIR}/imstb_rectpack.h"
    "${IMGUI_DIR}/imstb_textedit.h"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imstb_truetype.h"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
add_library(imgui STATIC ${IMGUI_FILES})
target_link_libraries(imgui PUBLIC glfw)
target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# -- implot --
add_library(implot STATIC
    "implot/implot_demo.cpp"
    "implot/implot_items.cpp"
    "implot/implot.cpp"
    "implot/implot.h"
    "implot/implot_internal.h"
)
target_include_directories(implot PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/implot)
target_link_libraries(implot PUBLIC imgui)

# -- eigen --
add_library(eigen3 INTERFACE)
target_include_directories(eigen3 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/eigen)

# -- Homework --
add_executable(hw-main main.cpp BSplineApp.cpp BSpline.cpp)
target_link_libraries(hw-main imgui implot glfw OpenGL::GL eigen3)
