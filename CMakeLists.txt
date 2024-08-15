cmake_minimum_required(VERSION 3.12)
project(SecureUnderwaterComm VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/homebrew/opt/openssl@3 /opt/homebrew/opt/python@3.12)


# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Find Python
if(APPLE)
    # On macOS, prefer Homebrew's Python if available
    execute_process(
        COMMAND brew --prefix python
        OUTPUT_VARIABLE HOMEBREW_PYTHON_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(HOMEBREW_PYTHON_PREFIX)
        set(Python3_ROOT_DIR "${HOMEBREW_PYTHON_PREFIX}")
    endif()
endif()

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Add OpenSSL and Python include directories
include_directories(${OPENSSL_INCLUDE_DIR} ${Python3_INCLUDE_DIRS})

# Define source files
set(SOURCES
    src/MessageComposer.cpp
    src/FracturedMessage.cpp
    src/CommunicationChannel.cpp
    src/Submarine.cpp
    src/PythonIntegration.cpp
    src/ZeroKnowledgeAuth.cpp
    src/KeyExchange.cpp
)

# Create the main executable
add_executable(SecureUnderwaterComm ${SOURCES} src/main.cpp)

# Include directories
target_include_directories(SecureUnderwaterComm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link OpenSSL, pthread, and Python
target_link_libraries(SecureUnderwaterComm 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    pthread 
    Python3::Python
)

# Add tests
enable_testing()
add_executable(RunTests tests/test.cpp ${SOURCES})
target_include_directories(RunTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(RunTests 
    OpenSSL::SSL 
    OpenSSL::Crypto 
    pthread 
    Python3::Python
)
add_test(NAME RunTests COMMAND RunTests)

# Copy Python script to build directory
configure_file(${CMAKE_SOURCE_DIR}/scripts/adaptive_frequency_hopping.py
               ${CMAKE_BINARY_DIR}/scripts/adaptive_frequency_hopping.py COPYONLY)

# Set Python path for runtime
set_target_properties(SecureUnderwaterComm PROPERTIES
    BUILD_WITH_INSTALL_RPATH TRUE
    INSTALL_RPATH "${Python3_LIBRARY_DIRS}"
)