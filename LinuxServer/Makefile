 
EXE=server_03
SUBDIR=src jsoncpp afos
CFLAGS = -g

LDFLAGS += -lpthread

CXX_SOURCES =$(foreach dir,$(SUBDIR), $(wildcard $(dir)/*.cpp))
CXX_OBJECTS=$(patsubst  %.cpp, %.o, $(CXX_SOURCES))
DEP_FILES  =$(patsubst  %.o,  %.d, $(CXX_OBJECTS))

$(EXE): $(CXX_OBJECTS)
	g++  $(CFLAGS) $(CXX_OBJECTS) -o $(EXE) $(LDFLAGS)
	
%.o: %.cpp
	g++  $(CFLAGS) -c  -MMD $<  -o  $@

-include $(DEP_FILES)

clean: 
	rm  -rf  $(CXX_OBJECTS)  $(DEP_FILES)  $(EXE)

test:
	echo $(CXX_OBJECTS)

	
