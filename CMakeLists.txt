cmake_minimum_required(VERSION 4.1.2)
project("HTTP Library" CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_library(http_library STATIC src/http_library.cpp src/http_codec.cpp)

install(TARGETS http_library ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
