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

libuthread.a: $(objs)
		ar rcs $@ $<

queue.o: queue.c queue.h
	gcc -c queue.c 

# %.o: %.c
#		@echo "CC $@"
#		$(Q) -o $@ $<

clean:
		@echo "clean"
		$(Q) rm -f $(lib) $(objs)


