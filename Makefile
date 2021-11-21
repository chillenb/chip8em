CXX = clang++
LIBS = -lSDL2

CFLAGS = -g -O2 -std=c++20 -Wall
SRCS = main.cc chip8em.cc chip8sdl.cc
OBJS = $(SRCS:.cc=.o)

TARGET = chip8em

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.cc.o:
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: depend clean

clean:
	$(RM) *.o *~ $(TARGET)

depend: $(SRCS)
	makedepend $^