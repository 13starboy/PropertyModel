cmake_minimum_required(VERSION 3.26)

project(pm VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
message(STATUS "Downloading GoogleTest (v1.17.0)…")
FetchContent_Declare(
  googletest
  URL         https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  USES_TERMINAL_DOWNLOAD TRUE
  GIT_PROGRESS TRUE
  DOWNLOAD_NO_EXTRACT FALSE
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(lib)
add_subdirectory(tests)
