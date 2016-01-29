# static library
ifeq ($(MAKECMDGOALS), static)
BUILDPATH=build/static/
CC=g++
CFLAGS=-Wextra -Og -g3 -ggdb -c -std=c++11 -I../ -DPNT_EXPORT_LIBRARY
LNK=ar rcs
LNKFLAGS=
OUTPREFIX=
BUILDNAME=libpalnatoki.a
SOURCEEXT=cpp
SOURCEPATH=src/

# shared library
else ifeq ($(MAKECMDGOALS), shared)
BUILDPATH=build/shared/
CC=g++
CFLAGS=-Wextra -Og -g3 -ggdb -c -std=c++11 -I../ -DPNT_EXPORT_LIBRARY
OUTPREFIX=-o
LNK=g++
LNKFLAGS=-shared -I../
BUILDNAME=libpalnatoki.so
SOURCEEXT=cpp
SOURCEPATH=src/

# static test executable
else ifeq ($(MAKECMDGOALS), test-static)
CC=g++
CFLAGS=-Wextra -Og -g3 -ggdb -c -std=c++11 -I../ -static
LNK=g++
LNKFLAGS=-Lbuild/static/ -lpalnatoki -static
OUTPREFIX=-o
BUILDPATH=test/build/
BUILDNAME=test.exe
SOURCEEXT=cpp
SOURCEPATH=test/

# shared test executable
else ifeq ($(MAKECMDGOALS), test-shared)
CC=g++
CFLAGS=-Wextra -Og -g3 -ggdb -c -std=c++11 -I../
LNK=g++
LNKFLAGS=-Lbuild/shared/ -lpalnatoki
OUTPREFIX=-o
BUILDPATH=test/build/
BUILDNAME=test.exe
SOURCEEXT=cpp
SOURCEPATH=test/

# cleaning
else ifeq ($(MAKECMDGOALS), clean)
else
$(error No target specified, available: clean/shared/static/test-static/test-shared)
endif


ifeq ($(OS), WIN32)
CFLAGS += -DWIN32
endif


# files
SOURCE=$(shell find $(SOURCEPATH) -name *.$(SOURCEEXT))
OBJECTS=$(subst $(SOURCEPATH),$(BUILDPATH),\
	$(subst .$(SOURCEEXT),.o,$(SOURCE)))
DEPENDENCIES=$(OBJECTS:.o=.d)
-include $(DEPENDENCIES)


$(OBJECTS): $(BUILDPATH)%.o: $(SOURCEPATH)%.$(SOURCEEXT)
	$(CC) $(CFLAGS) -MP -MMD -o $@ $<

target: $(OBJECTS)
	$(LNK) $(OUTPREFIX) $(BUILDPATH)$(BUILDNAME) $^ $(LNKFLAGS)

shared: target

static: target

test-static: target

test-shared: target

clean:
	-rm $(shell find . -name *.o)
	-rm $(shell find . -name *.d)
	-rm $(shell find . -name libpalnatoki*.so*)
	-rm $(shell find . -name libpalnatoki*.a)
