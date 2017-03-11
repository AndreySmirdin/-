
CC      := g++
EXT     := cpp
FG      := -std=c++03 -g -Wall -Wshadow -Wextra
PROGNAME:= main

INCDIR  := include
OBJDIR  := bin
SRCDIR  := src
DEPDIR  := dep


#Don't change this, if you don't know what is it
SOURCES := $(shell find $(SRCDIR) -name '*.$(EXT)')
OBJECTS := $(subst $(SRCDIR), $(OBJDIR), $(SOURCES:.$(EXT)=.o))
DEPS    := $(subst $(SRCDIR), $(DEPDIR), $(SOURCES:.$(EXT)=.d))

.PHONY: dirs all clean

all: dirs $(PROGNAME) 

dirs:
	mkdir -p $(OBJDIR) $(DEPDIR)

$(PROGNAME): $(OBJECTS)
	$(CC) $(FG) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.$(EXT)
	$(CC) $(FG) -MMD -I$(INCDIR) $< -c -o $@
	mv $(OBJDIR)/*.d $(DEPDIR)/
-include $(DEPS)

clean:
	rm -rf $(PROGNAME) $(OBJDIR) $(DEPDIR)
