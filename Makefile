CXX      = g++
AR       = ar
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC      = $(wildcard src/*.cpp)
OBJ      = $(SRC:.cpp=.o)

LIB      = libhttp_library.a

DEPLIB   = lib/libhttp_codec.a

PREFIX    ?= /usr/local
INCDIR    = $(PREFIX)/include/http_library
LIBDIR    = $(PREFIX)/lib

.PHONY: all clean install uninstall

all: $(LIB)

$(LIB): $(OBJ) $(DEPLIB)
	$(AR) rcs $@ $^

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

install: all
	@echo "Installing headers to $(INCDIR) and library to $(LIBDIR)..."
	@mkdir -p $(INCDIR)
	@cp -r include/* $(INCDIR)
	@mkdir -p $(LIBDIR)
	@cp $(LIB) $(LIBDIR)
	@echo "Installation complete."

uninstall:
	@echo "Removing headers from $(INCDIR) and library from $(LIBDIR)..."
	@rm -rf $(INCDIR)
	@rm -f $(LIBDIR)/$(LIB)
	@echo "Uninstallation complete."

clean:
	rm -f src/*.o $(LIB)
