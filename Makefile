.PHONY: all install clean
CFLAGS = -O3
BIN = tokyobash
BINDIR = bin
OBJ = tokyobash.o tokyobashlib.o
CONFIG_FILE = config
CONFIGDIR = ~/.config/tokyobash
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

install: $(BIN) $(CONFIG_FILE)

	@mkdir -p $(CONFIGDIR)
	@cp -n $(CONFIG_FILE) $(CONFIGDIR)/$(CONFIG_FILE)
	@echo "$(CONFIG_FILE) file placed in '$(CONFIGDIR)'."
	@echo "  -will not overwrite an existing config file."

ifeq ($(prefix),)
	@echo "$(BIN) binary successfully built and located in '$(PWD)/$(BINDIR)'."
else
	@cp -r $(BINDIR) $(prefix)
	@echo "$(BIN) successfully built and copied to '$(prefix)/$(BINDIR)'."
endif

clean:
	$(RM)r $(BINDIR) $(OBJ)

