cmake_minimum_required (VERSION 3.8)

project (Sandbox VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_subdirectory(libs)

include_directories(AFTER
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB_RECURSE ENGINE_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Sandbox/*.cpp)
file(GLOB_RECURSE ENGINE_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/Sandbox/*.*)

file(GLOB_RECURSE EXAMPLE_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Example/*.cpp)
file(GLOB_RECURSE EXAMPLE_HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/Example/*.h)

file(GLOB_RECURSE OBJECT_RESOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/*.obj)


add_subdirectory(resources)
configure_file(resourceConfig.h.in config/resourceConfig.h)
set(CONFIGS_DIR ${PROJECT_BINARY_DIR}/config)


add_library(SandboxEngine STATIC ${ENGINE_CPP_FILES} ${ENGINE_HEADER_FILES})
add_executable(SandboxEngineExample ${EXAMPLE_CPP_FILES} ${EXAMPLE_HEADER_FILES})

target_include_directories(SandboxEngine
	PUBLIC 
		${CONFIGS_DIR}
)

add_compile_definitions(GLEW_STATIC)

target_link_libraries(SandboxEngine ${OPENGL_LIBRARY})
target_link_libraries(SandboxEngine glfw)
target_link_libraries(SandboxEngine glm)
target_link_libraries(SandboxEngine ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib/glew32s.lib)
target_link_libraries(SandboxEngine ${CMAKE_CURRENT_SOURCE_DIR}/libs/soil/lib/soil.lib)

target_link_libraries(SandboxEngineExample ${OPENGL_LIBRARY})
target_link_libraries(SandboxEngineExample glfw)
target_link_libraries(SandboxEngineExample glm)
target_link_libraries(SandboxEngineExample ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/lib/glew32s.lib)
target_link_libraries(SandboxEngineExample ${CMAKE_CURRENT_SOURCE_DIR}/libs/soil/lib/soil.lib)
target_link_libraries(SandboxEngineExample SandboxEngine)

target_include_directories(SandboxEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include/)
target_include_directories(SandboxEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/soil/include/)
target_include_directories(SandboxEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinygltf/include/)
target_include_directories(SandboxEngineExample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/glew/include/)
target_include_directories(SandboxEngineExample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/soil/include/)
target_include_directories(SandboxEngineExample PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinygltf/include/)

set_property(TARGET SandboxEngine SandboxEngineExample PROPERTY CXX_STANDARD 17)

add_custom_command(TARGET SandboxEngineExample POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/resources"
		"$<TARGET_FILE_DIR:SandboxEngineExample>/resources"
)
add_custom_command(TARGET SandboxEngineExample POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_SOURCE_DIR}/resources"
		"$<TARGET_FILE_DIR:SandboxEngineExample>/../resources" # Copy to IDE project root, so it will find resources properly when executed from IDE
)
