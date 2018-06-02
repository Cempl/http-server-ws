cmake_minimum_required(VERSION 2.8.3)

project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtCore library
find_package(Qt5Core)

# Add the include directories for the Qt 5 Core module to
# the compile lines.
include_directories(${Qt5Core_INCLUDE_DIRS})

#
add_subdirectory(engine)
add_subdirectory(database)
add_subdirectory(web)

# Use the compile definitions defined in the Qt 5 Core module
add_definitions(${Qt5Core_DEFINITIONS})

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

#qt5_generate_moc(main.cpp main.moc)

# Tell CMake to create the executable
add_executable(server main.cpp)

#Link the executable to the Qt 5 core library.
target_link_libraries(server Qt5::Core)
