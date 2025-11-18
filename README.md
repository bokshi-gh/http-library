# HTTP Library

A lightweight C++ HTTP library for creating and handling HTTP requests and responses.
<br>
This library is designed to be simple, extensible, and easy to integrate into your C++ projects.

## Features

- Handle HTTP requests and responses
- Encode and decode HTTP requests and responses
- Directly load environment variables from a .env file
- Easy to integrate as a static or shared library

## Installation

- Run the command:
```bash
bash <(curl -s https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh)
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

> [!WARNING]
> If you are thinking of using this library(i know no one will). Don't use it in the production.
>
> Library is bug prone and unstable due to the lack of error handling in edge cases.
>
> Just consider using this in small codebases or testing.
>
> I think i will make it stable in upcoming releases.
