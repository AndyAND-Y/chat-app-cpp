cmake_minimum_required(VERSION 3.10)

# Set project name
project(NetworkApp)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)

# Add include directories for all sources
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add the common source files that are shared by both the client and server
set(COMMON_SOURCES
    src/common/socket_utils.cpp
)

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Add executable for client
add_executable(client
    src/client/client.cpp
    ${COMMON_SOURCES}
)

# Add executable for server
add_executable(server
    src/server/server.cpp
    ${COMMON_SOURCES}
)

# Link with the necessary library (in this case ws2_32)
target_link_libraries(client PRIVATE ws2_32)
target_link_libraries(server PRIVATE ws2_32)
