cmake_minimum_required(VERSION 3.12)
project(SecureUnderwaterComm VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Add OpenSSL include directory
include_directories(${OPENSSL_INCLUDE_DIR})

# Define source files
set(SOURCES
    src/MessageComposer.cpp
    src/FracturedMessage.cpp
    src/CommunicationChannel.cpp
    src/Submarine.cpp
)

# Create the main executable
add_executable(SecureUnderwaterComm ${SOURCES} src/main.cpp)

# Include directories
target_include_directories(SecureUnderwaterComm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link OpenSSL and pthread
target_link_libraries(SecureUnderwaterComm OpenSSL::SSL OpenSSL::Crypto pthread)

# Add tests
enable_testing()
add_executable(RunTests tests/test.cpp ${SOURCES})
target_include_directories(RunTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(RunTests OpenSSL::SSL OpenSSL::Crypto pthread)
add_test(NAME RunTests COMMAND RunTests)