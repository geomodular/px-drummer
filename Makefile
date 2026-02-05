CC     = cc
BINDIR = ./bin
BLDDIR = ./build
SRCDIR = ./src
SRCS   = $(wildcard $(SRCDIR)/*.c)
OBJS   = $(addprefix $(BLDDIR)/, $(notdir $(SRCS:.c=.o)))
DEPS   = $(OBJS:.o=.d)
MODE   =

LIBS     = -lSDL2 -lSDL2_mixer -lSDL2_image
CFLAGS   = -std=c11 -Wall -Wextra -pedantic -Werror -Wno-unused-parameter
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
		$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS): | $(BLDDIR)
$(BLDDIR)/%.o: $(SRCDIR)/%.c
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

$(DEPS): | $(BLDDIR)
$(BLDDIR)/%.d: $(SRCDIR)/%.c
		$(CC) $(CFLAGS) $(CPPFLAGS) -MM $< -MT $(@:.d=.o) > $@

clean:
		@rm -rf $(BINDIR)
		@rm -rf $(BLDDIR)
		@echo cleaning done

include $(BLDDIR)/.depend

