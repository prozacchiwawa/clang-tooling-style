cmake_minimum_required(VERSION 3.13)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

set(LIBCLANG_INSTALL "libclang-copied")
ExternalProject_Add(libclang-static-build
	DOWNLOAD_DIR "libclang-static-build"
	INSTALL_DIR "${LIBCLANG_INSTALL}"
	GIT_REPOSITORY "https://github.com/deech/libclang-static-build"
	GIT_TAG "0cae8e85ef1ad951e1bb560e1eadcd64b2f0828e"
	CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${LIBCLANG_INSTALL}"
	)

project(noglob)
add_compile_options("-fno-rtti")

add_executable(noglob
  DisallowGlobals.cpp
  DisallowNewDelete.cpp
  DisallowNonAbstract.cpp
  DisallowCoupling.cpp
  tool.cpp
  )

ExternalProject_Get_Property(libclang-static-build binary_dir)

include_directories(
	${binary_dir}/_deps/clang_sources-src/include
	${binary_dir}/_deps/libclang_prebuilt-src/include
	)

add_dependencies(noglob libclang-static-build)
target_link_libraries(noglob "-L${binary_dir}" "-lclang_static_bundled" -lpthread)
