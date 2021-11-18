CXX = clang++


CFLAGS = -g -O2 -std=c++17 -Wall
SRCS = main.cc chip8em.cc
OBJS = $(SRCS:.c=.o)

TARGET = chip8em


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.cc.o:
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) *.o *~ $(TARGET)