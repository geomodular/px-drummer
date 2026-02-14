CC     = clang
BINDIR = ./bin
BLDDIR = ./build
SRCDIR = ./src
SRCS   = $(wildcard $(SRCDIR)/*.c)
OBJS   = $(addprefix $(BLDDIR)/, $(notdir $(SRCS:.c=.o)))
DEPS   = $(OBJS:.o=.d)
MODE   =

LIBS     = -lSDL3
CFLAGS   = -std=c23 -Wall -Wextra -Wpedantic -Wno-unused-parameter --embed-dir=./assets/
MAIN     = $(BINDIR)/pxdrummer

ifeq ($(MODE),debug)
		CFLAGS += -g
else
		CFLAGS += -O2
endif

.PHONY: all clean

all: $(MAIN)

$(BLDDIR)/.depend: $(DEPS)
		@cat $(BLDDIR)/*.d > $(BLDDIR)/.depend

$(BINDIR):
		@mkdir -p $(BINDIR)

$(BLDDIR):
		@mkdir -p $(BLDDIR)

$(MAIN): | $(BINDIR)
$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS): | $(BLDDIR)
$(BLDDIR)/%.o: $(SRCDIR)/%.c
		$(CC) -c $(CFLAGS) $< -o $@

$(DEPS): | $(BLDDIR)
$(BLDDIR)/%.d: $(SRCDIR)/%.c
		$(CC) --embed-dir=./assets/ -MM $< -MT $(@:.d=.o) > $@

clean:
		@rm -rf $(BINDIR)
		@rm -rf $(BLDDIR)
		@echo cleaning done

include $(BLDDIR)/.depend

