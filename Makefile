CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

LDFLAGS = -lfltk -lfltk_images

SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/main

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCFILES))

all: $(TARGET)
$(TARGET): $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(BUILDDIR)/*.o $(TARGET)

run_server:
	python3 server/server.py

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run run_server
