CFLAGS = -Wall -O2
BIN = tokyobash
BINDIR = bin
SRC = src/tokyobash.c src/lib/tokyobashlib.c
OBJ = tokyobash.o tokyobashlib.o
CONFIG_FILE = config
CONFIGDIR = ~/.config/tokyobash
prefix =

all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p $(BINDIR)
	@gcc $(CFLAGS) $(OBJ) -o $(BINDIR)/$(BIN)
	@$(RM) $(OBJ)

tokyobash.o: src/tokyobash.c
	@gcc $(CFLAGS) -c src/tokyobash.c

tokyobashlib.o: src/lib/tokyobashlib.c
	@gcc $(CFLAGS) -c src/lib/tokyobashlib.c

install: $(BIN) $(CONFIG_FILE)

ifeq ($(prefix),)
	@mkdir -p $(CONFIGDIR)
	@cp -n $(CONFIG_FILE) $(CONFIGDIR)/$(CONFIG_FILE)
	@echo "$(CONFIG_FILE) successfully placed at $(CONFIGDIR)"
	@echo "$(BIN) binary located at $(PWD)/$(BINDIR)/"
else
	@mkdir -p $(CONFIGDIR)
	@cp -n $(CONFIG_FILE) $(CONFIGDIR)/$(CONFIG_FILE)
	@cp -r $(BINDIR) $(prefix)
	@echo "$(CONFIG_FILE) successfully placed at $(CONFIGDIR)"
	@echo "$(BIN) successfully placed at $(prefix)/$(BINDIR)"
endif

clean:
	$(RM)r $(BINDIR) $(OBJ)

.PHONY: all install clean
