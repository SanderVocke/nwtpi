PGM=testtriangle


SOURCES= $(wildcard *.cpp)
HEADERS= $(wildcard *.h)
OBJDIR=obj
OBJS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(SOURCES))

GPP=g++
CFLAGS+=-Wall -g
LDFLAGS+=-L/opt/vc/lib -lbcm_host -lGLESv2 -lEGL 
INCLUDES+=-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I./

all: showme $(PGM)

$(OBJDIR)/%.o: %.cpp $(HEADERS)
	$(GPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(PGM): $(OBJS)
	$(GPP) -Wl $(LDFLAGS) $(OBJS) -o $@

clean:
	@for i in $(OBJS); do (if test -e "$$i"; then ( echo "==> removing $$i" && rm $$i ); fi ); done
	@echo "==> removing $(PGM)" && rm -f $(PGM)

showme:
	@uname -a

# /usr/lib/arm-linux-gnueabihf/libstdc++.so.6.0.17
# /usr/lib/gcc/arm-linux-gnueabihf/4.6/libstdc++.a
