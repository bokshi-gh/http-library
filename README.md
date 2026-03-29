# HTTP Library

A lightweight C++ HTTP library for creating and handling HTTP requests and responses.

## Features

- Handle HTTP requests and responses
- Encode and decode HTTP requests and responses
- Dynamic routing
- Directly load environment variables from a environment variable file
- Easy to integrate as a static or shared library

## Installation

- Install remotely via installer script:

```bash
curl -fsSL https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh | sudo bash
```

> Check source code of installer code [here](https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh).

## Integrate with your project

- Example code is available in [**`examples/http_library_demo.cpp`**](examples/http_library_demo.cpp) file demonstrating how to use the library.

- Add the header in your C++ project:

```c++
#include <bokshi/http_library.hpp>
```

- Link library while compiling your project:

```bash
g++ main.cpp -lhttp_library -pthread -o my_server
```

## Uninstallation

```sh
sudo rm -f /usr/local/include/bokshi/*.hpp
sudo rm -f /usr/local/lib/libhttp_library.a
sudo ldconfig
```
