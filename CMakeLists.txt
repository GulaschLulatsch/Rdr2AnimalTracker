cmake_minimum_required(VERSION 4.0.3)

project(AnimalTracker LANGUAGES CXX C)

set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY	${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY	${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY	${CMAKE_BINARY_DIR}/bin)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
if(MSVC)
	add_compile_options(/W4 /wd4250)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion)
endif()


find_package(spdlog REQUIRED)
find_package(ICU REQUIRED)
find_package(ScriptHookRDR2 REQUIRED PATHS external/ScriptHookRDR2)
find_package(SimpleIni REQUIRED PATHS external/SimpleIni)
find_package(better-enums REQUIRED PATHS external/better-enums)

add_subdirectory(src)