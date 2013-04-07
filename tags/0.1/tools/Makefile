CC=g++
CFLAGS=-c -Wall -O3 -I/usr/local/include -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -Iinclude/ngl -Isrc/ngl -Isrc/shaders -DNGL_DEBUG
LDFLAGS=-L/opt/vc/lib -lEGL -lGLESv2 
SOURCES=$(shell find ./ -name *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
EXECUTABLE=EGLgetConfig

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean :
	rm -f *.o $(EXECUTABLE)
