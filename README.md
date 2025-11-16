# HTTP Library

A lightweight C++ HTTP library for creating and handling HTTP requests and responses.

This library is designed to be simple, extensible, and easy to integrate into your C++ projects.

## Features

- Handle HTTP requests and responses.
- Encode and decode HTTP requests and responses.
- Easy to integrate as a static or shared library

## Installation

Run the command:
```bash
bash <(curl -s https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh)
```
>**REQUIREMENTS:** bash, curl, git, cmake, make

## Usage

Add the header in your C++ project:
```c++
#include <http_library.hpp>
```

Link library while compiling your project:
```bash
g++ main.cpp -lhttplibrary -o main
```
