cmake_minimum_required(VERSION 3.10)

project(NetworkApp)

set(CMAKE_CXX_STANDARD 23)

# Include the src directory for headers
include_directories(${CMAKE_SOURCE_DIR}/src)

# Automatically include all source files from src/common
file(GLOB_RECURSE  COMMON_SOURCES ${CMAKE_SOURCE_DIR}/src/common/*.cpp)
file(GLOB_RECURSE  SERVER_SOURCES ${CMAKE_SOURCE_DIR}/src/server/*.cpp)
file(GLOB_RECURSE  CLIENT_SOURCES ${CMAKE_SOURCE_DIR}/src/client/*.cpp)


# Set output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

message(STATUS "COMMON_SOURCES: ${COMMON_SOURCES}")


# Add the client executable
add_executable(client
    
    ${COMMON_SOURCES}
    ${CLIENT_SOURCES}
)

# Add the server executable
add_executable(server
    
    ${COMMON_SOURCES}
    ${SERVER_SOURCES}
)

# Link libraries
target_link_libraries(client PRIVATE ws2_32)
target_link_libraries(server PRIVATE ws2_32)

# Optionally, you could add the headers to target's include directories if needed
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
