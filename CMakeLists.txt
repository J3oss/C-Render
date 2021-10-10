cmake_minimum_required(VERSION 3.10)

project(C-Render)

add_subdirectory(ext/SDL)
add_subdirectory(ext/assimp)
add_subdirectory(ext/glm)

add_executable(C-Render
	src/main.cpp
	src/window.cpp
	src/renderer.cpp
	src/scene.cpp
	src/scene_manager.cpp
	src/camera.cpp
	src/node.cpp
	src/image.cpp)

target_link_libraries(C-Render
	SDL2
	assimp
	glm)

target_include_directories(C-Render PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include)
