# HTTP Library

A lightweight C++ HTTP library for creating and handling HTTP requests and responses.
<br>
This library is designed to be simple, extensible, and easy to integrate into your C++ projects.

## Features

- Handle HTTP requests and responses
- Encode and decode HTTP requests and responses
- Dynamic routing
- Directly load environment variables from a environment variable file
- Easy to integrate as a static or shared library

## Installation

- Run the command:
```bash
curl -fsSL https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh | bash
```
>**REQUIREMENTS:** bash, curl, git, cmake, make

## Usage

- Example code is available in [**`src/main.cpp`**](https://github.com/bokshi-gh/http-library/blob/main/src/main.cpp) file demonstrating how to use the library.

- Add the header in your C++ project:
```c++
#include <http_library.hpp>
```

- Link library while compiling your project:
```bash
g++ main.cpp -lhttplibrary -o main
```
