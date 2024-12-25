CC = g++
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -limgui -lglfw -lGL -lGLEW
SRCDIR = src
OBJDIR = obj
TARGETDIR = bin
SRCS = $(wildcard $(SRCDIR)/*)
OBJS = $(patsubst $(SRCDIR)/*.cpp, $(OBJDIR)/%.o, $(SRCS))
TARGETMAIN = $(TARGETDIR)/main

all: $(TARGETMAIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETDIR):
	mkdir -p $(TARGETDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGETMAIN): $(OBJS) | $(TARGETDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(TARGETDIR)

run:
	./bin/main

