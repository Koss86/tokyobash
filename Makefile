.PHONY: all install clean
CFLAGS = -O3 -Wall -Wextra -pedantic -fstack-protector-strong -std=gnu99 -Wshadow -Wconversion -Wdouble-promotion -Wmissing-noreturn -Wmissing-format-attribute -Wmissing-prototypes -fsigned-char -Wno-conversion -fno-common -Wno-unused-result -Wimplicit-fallthrough -fdiagnostics-color=always
BINDIR = bin
BIN = tokyobash
LIB1 = tokyobashlib
LIB2 = gitlib
LIB3 = shouldFetchlib
LIB4 = printlib
OBJ = $(BIN).o $(LIB1).o $(LIB2).o $(LIB3).o $(LIB4).o
CONFIG_FILE = config
CONFIGDIR = $(shell [ -n "$$XDG_CONFIG_HOME" ] && echo "$$XDG_CONFIG_HOME" || echo "$$HOME/.config")/$(BIN)
COMP_VERSION := $(shell gcc --version)
prefix =
CC =

ifneq ($(findstring gcc,$(COMP_VERSION)),)
	CC = gcc
else
	COMP_VERSION = $(shell clang --version)
	ifneq ($(findstring clang,$(COMP_VERSION)),)
		CC = clang
	endif
endif

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p $(BINDIR)
	@$(CC) $(CFLAGS) $(OBJ) -o $(BINDIR)/$(BIN)

$(BIN).o: src/$(BIN).c
	@$(CC) $(CFLAGS) -c src/$(BIN).c

$(LIB1).o: src/lib/$(LIB1).c
	@$(CC) $(CFLAGS) -c src/lib/$(LIB1).c

$(LIB2).o: src/lib/$(LIB2).c
	@$(CC) $(CFLAGS) -c src/lib/$(LIB2).c

$(LIB3).o: src/lib/$(LIB3).c
	@$(CC) $(CFLAGS) -c src/lib/$(LIB3).c

$(LIB4).o: src/lib/$(LIB4).c
	@$(CC) $(CFLAGS) -c src/lib/$(LIB4).c

install: $(BIN) $(CONFIG_FILE)
	@mkdir -p $(CONFIGDIR)
	@cp -n $(CONFIG_FILE) $(CONFIGDIR)/$(CONFIG_FILE)
	@echo "$(CONFIG_FILE) file created and copied to '$(CONFIGDIR)'."
	@echo "  -will not overwrite pre-existing config file."

ifeq ($(prefix),)
	@echo "$(BIN) binary successfully built and located in '$(PWD)/$(BINDIR)'."
else
	@cp -r $(BINDIR) $(prefix)
	@echo "$(BIN) successfully built and moved to '$(prefix)/$(BINDIR)'."
	@$(RM)r $(BINDIR)
endif

	@$(RM) $(OBJ) $(CONFIG_FILE)

$(CONFIG_FILE):
	@{ \
		printf 'theme      = tokyonight\n'; \
		printf '#theme     = catppuccin\n'; \
		printf '#theme     = kanagawa\n\n'; \
		printf 'background = 0\n'; \
		printf 'time       = 1\n'; \
		printf 'abvpath    = 1\n'; \
		printf 'branchname = 1\n'; \
		printf 'statusbar  = 1\n'; \
		printf 'fetch      = 0\n'; \
		printf 'fetchtimer = 1d\n'; \
		printf 'icon       = \n'; \
	} >$(CONFIG_FILE)

clean:
	$(RM)r $(BINDIR) $(OBJ)
