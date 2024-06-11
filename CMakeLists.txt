cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

cmake_policy(SET CMP0067 NEW)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 23)

project(bn-test VERSION 0.1)

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

include_directories(".")

add_library(bntest STATIC
	# HEADERS
	"bntest.h"

	# SOURCE FILES
	"bntest.cc")

target_link_libraries(bntest
	PUBLIC binaryninjaapi
	PUBLIC GTest::gtest_main)

if(DEFINED ENABLE_TESTING)
	message(STATUS "Enabling bntest plugin for testing...")
	enable_testing()
	add_library(bntest-tests SHARED
		"tests.cc"
		"examples/mlil_tests.cc")

	#include(GoogleTest)
	#gtest_discover_tests(bntest-tests)

	target_link_libraries(bntest-tests
		PRIVATE bntest)

	install(TARGETS bntest-tests DESTINATION "$ENV{APPDATA}\\Binary Ninja\\plugins")
endif()
