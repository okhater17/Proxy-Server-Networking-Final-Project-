cmake_minimum_required(VERSION 3.1)
project(final_project)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CLIENT_FILES
  src/client.cpp
)

set(SERVER_FILES
  src/proxy.cpp
)

add_executable(client ${CLIENT_FILES})

target_include_directories(client PRIVATE third_party/boost/include)
target_include_directories(client PRIVATE third_party/curlpp-0.8.1/include)
target_link_libraries(client PRIVATE Threads::Threads)

add_executable(proxy ${SERVER_FILES})

target_include_directories(proxy PRIVATE third_party/boost/include)
target_include_directories(proxy PRIVATE third_party/curlpp-0.8.1/include)
target_link_libraries(proxy PRIVATE Threads::Threads)
