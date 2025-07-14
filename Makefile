.PHONY: all install clean
CFLAGS = -O3 -Wall -Wextra -pedantic -fstack-protector-strong -std=gnu99 -Wshadow -Wconversion -Wdouble-promotion -Wmissing-noreturn -Wmissing-format-attribute -Wmissing-prototypes -fsigned-char -Wno-conversion -fno-common -Wno-unused-result -Wimplicit-fallthrough -fdiagnostics-color=always
BIN = tokyobash
BINDIR = bin
OBJ = tokyobash.o tokyobashlib.o gitlib.o fetchlib.o
CONFIG_FILE = config
CONFIGDIR = $(shell [ -n "$$XDG_CONFIG_HOME" ] && echo "$$XDG_CONFIG_HOME" || echo "$$HOME/.config")/$(BIN)
prefix =
CXX =
COMP_VERSION := $(shell gcc --version)

ifneq ($(findstring gcc,$(COMP_VERSION)),)
	CXX = gcc
else
	COMP_VERSION = $(shell clang --version)
	ifneq ($(findstring clang,$(COMP_VERSION)),)
		CXX = clang
	else
		@echo "No compiler found, exiting."
		exit 1
	endif
endif

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p $(BINDIR)
	@$(CXX) $(CFLAGS) $(OBJ) -o $(BINDIR)/$(BIN)

tokyobash.o: src/tokyobash.c
	@$(CXX) $(CFLAGS) -c src/tokyobash.c

tokyobashlib.o: src/lib/tokyobashlib.c
	@$(CXX) $(CFLAGS) -c src/lib/tokyobashlib.c

gitlib.o: src/lib/gitlib.c
	@$(CXX) $(CFLAGS) -c src/lib/gitlib.c

fetchlib.o: src/lib/fetchlib.c
	@$(CXX) $(CFLAGS) -c src/lib/fetchlib.c

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
	@printf 'theme      = tokyonight\n#theme     = catppuccin\n#theme     = kanagawa\n\ngit        = 1\nbranchname = 1\nstatusbar  = 0\ntime       = 1\nfetchtimer = 1d\n' > $(CONFIG_FILE)

clean:
	$(RM)r $(BINDIR) $(OBJ)

