PGM=testtriangle

PACKAGES= Native OEGL

NWTPI_SOURCES= $(foreach P,$(PACKAGES),$(wildcard $(P)/*.cpp))
NWTPI_INCLUDES= $(foreach I,$(PACKAGES), -I./$(I))

OBJDIR=obj
NWTPI_OBJS=$(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(NWTPI_SOURCES)))

DEBUG_ON=1

GPP=g++
CFLAGS+=-std=c++0x -Wall -O

ifdef DEBUG_ON
CFLAGS+=-ggdb -DDEBUG_ON
endif

INCLUDES+=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I. $(NWTPI_INCLUDES)
LDFLAGS+=-L/opt/vc/lib -lbcm_host -lGLESv2 -lEGL 

all: $(OBJDIR) showme $(PGM)

#$(PGM): $(OBJS)
#	$(GPP) -Wl $(LDFLAGS) $(OBJS) -o $(@)


testtriangle: $(NWTPI_OBJS) $(OBJDIR)/testtriangle.o $(OBJDIR)/Triangle.o
	$(GPP) -Wl $(LDFLAGS) $(^) -o $(@)


$(OBJDIR):
	@if ! [ -d $(@) ]; then mkdir -p $(@); fi

# ./
$(OBJDIR)/%.o: %.cpp
	$(GPP) $(CFLAGS) $(INCLUDES) -c $< -o $(@)

# ./Native
$(OBJDIR)/%.o: Native/%.cpp
	$(GPP) $(CFLAGS) $(INCLUDES) -c $< -o $(@)
	
# ./OEGL
$(OBJDIR)/%.o: OEGL/%.cpp
	$(GPP) $(CFLAGS) $(INCLUDES) -c $< -o $(@)

clean:
	@for i in $(OBJDIR)/*; do (if test -e "$$i"; then ( echo "==> removing $$i" && rm $$i ); fi ); done
	@for i in $(PGM);  do (if test -e "$$i"; then ( echo "==> removing $$i" && rm $$i ); fi ); done

showme:
	@uname -a

# /usr/lib/arm-linux-gnueabihf/libstdc++.so.6.0.17
# /usr/lib/gcc/arm-linux-gnueabihf/4.6/libstdc++.a