cmake_minimum_required(VERSION 2.8.3)

project(server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_AUTOMOC ON)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the Qt library
find_package(Qt5 REQUIRED
	Core
	Network
)

# Add the include directories for the Qt 5 Core module to
# the compile lines.
include_directories(${Qt5Core_INCLUDE_DIRS})

# dirs
add_subdirectory(engine)
add_subdirectory(database)
add_subdirectory(web)

# Use the compile definitions defined in the Qt 5 Core module
add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")

# Tell CMake to create the executable
add_executable(server main.cpp)

#Link the executable to the Qt 5 core library.
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Network)

#Link the executable to the custom libraries
target_link_libraries(${PROJECT_NAME} SSL_SERVER)
