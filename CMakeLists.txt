cmake_minimum_required(VERSION 4.1.2)
project("HTTP Library" CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_library(httplibrary STATIC src/http_library.cpp src/http_codec.cpp)

install(TARGETS httplibrary ARCHIVE DESTINATION lib)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.hpp")
install(FILES README.md DESTINATION docs)
