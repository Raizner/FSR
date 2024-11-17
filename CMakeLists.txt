cmake_minimum_required(VERSION 3.5.0)
project(first VERSION 0.1.0 LANGUAGES C CXX)

# Try to set C++23, and fallback to C++17 or C++11 if not available
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add all source files
set(SOURCES
    main.cpp
    task.cpp
    six_dof.cpp
    messages.cpp
)

# Add all header files for clarity (optional in CMake)
set(HEADERS
    task.h
    six_dof.h
    messages.h
)

# Add executable
add_executable(first ${SOURCES} ${HEADERS})

include(CTest)
enable_testing()