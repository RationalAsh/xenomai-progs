#
# TODO: Move `libmongoclient.a` to /usr/local/lib so this can work on production servers
#

EXECUTABLE=cyclictest 

CC := gcc # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := bin/${EXECUTABLE}
 
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -std=c99
LIB := -L/usr/lib -lm
INC := -Iinclude -I/usr/include

#Xenomai compiler and linker flags
XENO_CFLAGS := $(shell xeno-config --alchemy --rtdm --cflags)
XENO_LDFLAGS := $(shell xeno-config --alchemy --rtdm --ldflags)


$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(XENO_LDFLAGS) $(LIB)"; $(CC) $^ -o $(TARGET) $(XENO_LDFLAGS) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(XENO_CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(XENO_CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
ticket:
	$(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean

