cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

cmake_policy(SET CMP0067 NEW)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 23)

project(bn-test VERSION 0.1 LANGUAGES CXX)

if(WIN32)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Wish we could properly find_package the api...
FetchContent_Declare(binaryninja-api
  GIT_REPOSITORY https://github.com/Vector35/binaryninja-api.git
  GIT_TAG dev
  SYSTEM # So many warnings...
)
FetchContent_MakeAvailable(binaryninja-api)

set(INSTALL_GTEST OFF)
FetchContent_Declare(googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG main
	SYSTEM
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(bntest STATIC)

target_sources(bntest
	PUBLIC
		# HEADERS
		"${CMAKE_CURRENT_LIST_DIR}/src/bntest.h"

	PRIVATE
		# SOURCE FILES
		"${CMAKE_CURRENT_LIST_DIR}/src/bntest.cc")

target_link_libraries(bntest
	PRIVATE binaryninjaapi
	PUBLIC GTest::gtest_main)

target_include_directories(bntest
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

install(TARGETS bntest
	ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
	RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)
install(FILES "src/bntest.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(DEFINED ENABLE_TESTING)
	message(STATUS "Enabling bntest plugin for testing...")
	enable_testing()
	add_library(bntest-tests SHARED
		"examples/test-plugin.cc"
		"examples/mlil_tests.cc")

	target_link_libraries(bntest-tests
		PRIVATE bntest
		PRIVATE binaryninjaapi)

	install(TARGETS bntest-tests DESTINATION "$ENV{APPDATA}\\Binary Ninja\\plugins")
endif()