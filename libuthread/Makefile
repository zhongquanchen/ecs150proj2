#Target library

lib := libuthread.a
objs := queue.o uthread.o context.o preempt.o

CC := gcc
CFLAGS := -Wall -Werror -g
CFLAGS += -g

ifneq ($(V),1)
Q = @
endif

deps := $(patsubst %.o, %.d, $(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

libuthread.a: $(objs)
	@echo "AR libuthread.a"
	$(Q)ar rcs $@ $^

%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $< $(DEPFLAGS)

clean:
	@echo "CLEAN"
	$(Q)rm -f $(lib) $(objs) $(deps)
