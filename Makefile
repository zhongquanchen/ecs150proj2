# Target library
lib 	:= libuthread.a
objs	:= queue.o

CC 			:= gcc
CFLAGS	:= -Wall -Werror
CGLAGS	+= -g

AR			:= ar
RCS			:= rcs

ifneq ($(V), 1)
Q = @
endif

all: $(lib)

DEPFLAGS = -MMD -MF $(@:.o =.d)

libuthread: $(objs)
		@echo "CC $@"
		$(Q) $(AR) $(RCS) $@ $<

%.o %.c
		@echo "CC $@"
		$(Q) $(CC) $(CFLAGS) -o $@ $<

clean:
		@echo "clean"
		$(Q) rm -f $(lib) $(objs)
