CC = g++
CFLAGS = -g -c -Wall
LDFLAGS =
SOURCES = host.cpp tcpstream.cpp tcpacceptor.cpp thread.cpp
INCLUDES = -I. 
LPTHREAD = -lpthread
OBJS = $(SOURCES:.cpp=.o)
TARGET = host

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LPTHREAD) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

clean:
	rm -rf $(OBJS) $(TARGET)
