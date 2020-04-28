cmake_minimum_required (VERSION 3.8)

project (Sandbox VERSION 0.1 LANGUAGES CXX)
set(SXX_STANDART 17)

add_subdirectory(libs)

include_directories(AFTER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE ENGINE_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Sandbox/*.cpp)
file(GLOB_RECURSE ENGINE_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/Sandbox/*.*)

file(GLOB_RECURSE EXAMPLE_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Example/*.cpp)
file(GLOB_RECURSE EXAMPLE_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Example/*.h)

add_library(SandboxEngine ${ENGINE_CPP_FILES} ${ENGINE_HEADER_FILES})
add_executable(SandboxEngineExample ${EXAMPLE_CPP_FILES} ${EXAMPLE_HEADER_FILES})

target_link_libraries(SandboxEngine glfw)
target_link_libraries(SandboxEngine glm)
target_link_libraries(SandboxEngine ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib/glew32.lib)
target_link_libraries(SandboxEngineExample SandboxEngine)

target_include_directories(SandboxEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include/)
