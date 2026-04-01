# HTTP Library

A lightweight C++ HTTP library for creating and handling HTTP requests and responses.  
> [!NOTE]  
> The library only works on POSIX-compliant systems.

---

## Features

- Create and handle HTTP requests and responses
- Can be used as both an HTTP server and an HTTP client
- Uses [bokshi-gh/http-codec](https://github.com/bokshi-gh/http-codec) library to encode and decode HTTP requests and responses
- Includes [nlohmann/json](https://github.com/nlohmann/json) for parsing and serializing JSON data
- Includes [bokshi-gh/dotenv-cpp](https://github.com/bokshi-gh/dotenv-cpp) utility for loading environment variables directly from a file

---

## Installation

- Install via the installer script:

```bash
curl -fsSL https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh | sudo bash
```

> Requirements: [cURL](https://curl.se/), [GNU Make](https://www.gnu.org/software/make/), and [GCC](https://gcc.gnu.org/)

> You can check the source code of the installer script [here](https://raw.githubusercontent.com/bokshi-gh/http-library/main/scripts/install.sh).

---

## Integrate Into Your Project

- Example code demonstrating how to use the library is available in [**`examples/http_library_server_demo.cpp`**](examples/http_library_server_demo.cpp) and [**`examples/http_library_client_demo.cpp`**](examples/http_library_client_demo.cpp)

- Include the header in your C++ project:

```c++
#include <bokshi/http_library.hpp>
```

- Link the library when compiling your project:

  - If you need server, compile like:

    ```bash
    g++ main.cpp -lhttp_library -pthread -o my_server
    ```

  - If you only need client, compile like:

    ```bash
    g++ main.cpp -lhttp_library -o my_client
    ```

---

## Uninstallation

- Remove the installed headers and library files from the system:

```sh
sudo rm -f /usr/local/include/bokshi/
sudo rm -f /usr/local/lib/libhttp_library.a
sudo ldconfig
```

---

## License

This project is licensed under the [GPL-3.0 license](./LICENSE).  
