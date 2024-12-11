CXX = g++
CXXFLAGS = -Wall -Iinclude

LDFLAGS = -lfltk -lfltk_images -ljsoncpp -lpthread -lSDL2 -lSDL2_mixer

SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/main
SERVER_TARGET = $(BUILDDIR)/back_server

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCFILES))

SERVERDIR = server
SERVER_SRC = $(wildcard $(SERVERDIR)/*.cpp)
SERVER_OBJ = $(patsubst $(SERVERDIR)/%.cpp, $(BUILDDIR)/%.o, $(SERVER_SRC))

all: $(TARGET) $(SERVER_TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(SERVERDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(BUILDDIR)/*.o $(TARGET) $(SERVER_TARGET)

make_server:
	$(CXX) $(SERVERDIR)/server.cpp -o $(BUILDDIR)/back_server -ljsoncpp -lpthread

run_server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run run_server make_server
