cmake_minimum_required(VERSION 3.10)

project(C-Render)

add_subdirectory(ext/SDL)
add_subdirectory(ext/assimp)

add_executable(C-Render
	src/main.cpp
	src/window.cpp
	src/renderer.cpp
	src/point.cpp
	src/scene.cpp)

target_link_libraries(C-Render 
	SDL2
	assimp)
	
target_include_directories(C-Render PUBLIC
	${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/include)
