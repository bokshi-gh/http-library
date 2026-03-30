CXX      = g++
AR       = ar
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRC = \
	src/http-server/http_server.cpp \
	src/http-server/http_server_helpers.cpp \
	src/http-server/router/router.cpp \
	src/http-server/router/router_helpers.cpp

OBJ = $(SRC:.cpp=.o)

LIB_CODEC = lib/libhttp_codec.a

LIB_NAME = libhttp_library.a

INSTALL_INCLUDE_DIR = /usr/local/include/bokshi
INSTALL_LIB_DIR     = /usr/local/lib

.PHONY: all clean install uninstall

all: $(LIB_NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(LIB_NAME): $(OBJ)
	mkdir -p tmp_objs
	cp $(OBJ) tmp_objs/
	cd tmp_objs && ar x ../$(LIB_CODEC)
	ar rcs $(LIB_NAME) tmp_objs/*.o
	rm -rf tmp_objs

clean:
	rm -f $(OBJ) $(LIB_NAME)

install: $(LIB_NAME)
	@echo "Installing headers to $(INSTALL_INCLUDE_DIR)..."
	sudo mkdir -p $(INSTALL_INCLUDE_DIR)
	sudo cp -r include/* $(INSTALL_INCLUDE_DIR)/

	@echo "Installing library to $(INSTALL_LIB_DIR)..."
	sudo cp $(LIB_NAME) $(INSTALL_LIB_DIR)/
	sudo ldconfig

uninstall:
	@echo "Removing headers from $(INSTALL_INCLUDE_DIR)..."
	sudo rm -rf $(INSTALL_INCLUDE_DIR)

	@echo "Removing library from $(INSTALL_LIB_DIR)..."
	sudo rm -f $(INSTALL_LIB_DIR)/$(LIB_NAME)
	sudo ldconfig
