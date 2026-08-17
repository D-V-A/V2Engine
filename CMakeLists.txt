cmake_minimum_required(VERSION 3.20)

project(V2Engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)

FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG release-3.2.20
)

FetchContent_MakeAvailable(SDL3)

file(GLOB_RECURSE V2ENGINE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/headers/types/*.h"
)

add_executable(V2Engine ${V2ENGINE_SOURCES})

target_include_directories(V2Engine PRIVATE
	headers
)

add_custom_command(TARGET V2Engine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:SDL3::SDL3>
		$<TARGET_FILE_DIR:V2Engine>
)

add_custom_command(TARGET V2Engine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		${CMAKE_SOURCE_DIR}/assets
		$<TARGET_FILE_DIR:V2Engine>/assets
)

target_link_libraries(V2Engine PRIVATE SDL3::SDL3)