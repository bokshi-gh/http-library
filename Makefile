CXX      = g++
AR       = ar
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude
SRC      = src/http_library.cpp src/router.cpp src/router_helpers.cpp
OBJ      = $(SRC:.cpp=.o)
LIB_CODEC= lib/libhttp_codec.a
LIB_NAME = libhttp_library.a
INSTALL_INCLUDE_DIR = /usr/local/include/bokshi

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
	@echo "Installing http_library headers to $(INSTALL_INCLUDE_DIR)..."
	sudo mkdir -p $(INSTALL_INCLUDE_DIR)
	sudo cp include/*.hpp $(INSTALL_INCLUDE_DIR)/
	@echo "Installing $(LIB_NAME) to /usr/local/lib..."
	sudo cp $(LIB_NAME) /usr/local/lib/
	sudo ldconfig

uninstall:
	@echo "Removing http_library headers from $(INSTALL_INCLUDE_DIR)..."
	sudo rm -f $(INSTALL_INCLUDE_DIR)/*.hpp
	@echo "Removing $(LIB_NAME) from /usr/local/lib..."
	sudo rm -f /usr/local/lib/$(LIB_NAME)
	sudo ldconfig
