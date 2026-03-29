# HTTP Library

A lightweight C++ HTTP library for creating and handling HTTP requests and responses.  
> [!NOTE]  
> The library only works on POSIX-compliant systems.

---

## Features

- Create and handle HTTP requests and responses
- Can be used as both a server and a client
- Uses [this](https://github.com/bokshi-gh/http-codec.git) library to encode and decode HTTP requests and responses
- Uses [this](https://github.com/bokshi-gh/dotenv-cpp.git) utility to directly load environment variables from a file

---

## Installation

- Install remotely via installer script:

```bash
curl -fsSL https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh | sudo bash
```

> Requirements: [cURL](https://curl.se/), [GNU Make](https://www.gnu.org/software/make/), and [GCC](https://gcc.gnu.org/).

> Want to check source code of the installer script? It is [here](https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh).

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

---

## License

This project is licensed under the [GPL-3.0 license](./LICENSE).  
