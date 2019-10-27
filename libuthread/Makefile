## Target library
#lib 	:= libuthread.a
#objs	:= queue.o
#
#CC 			:= gcc
#CFLAGS	:= -Wall -Werror
#CGLAGS	+= -g
#
#AR			:= ar
#RCS			:= rcs
#
#ifneq ($(V), 1)
#Q = @
#endif
#
#all: $(lib)
#
#DEPFLAGS = -MMD -MF $(@:.o =.d)
#
#libuthread.a: $(objs)
#		ar rcs $@ $<
#
#queue.o: queue.c queue.h
#	 	$(CC) -c -o $@ $<
#
#uthread.o: uthread.c uthread.h
#		$(CC) -c -o $@ $<
#
## %.o: %.c
##		@echo "CC $@"
##		$(Q) -o $@ $<
#
#clean:
#		@echo "clean"
#		$(Q) rm -f $(lib) $(objs)

#Target library

lib := libuthread.a
obj := queue.o

CC := gcc
CFLAGS := -Wall -Werror -g

ifneq ($(V),1)
Q = @
endif

libuthread.a: queue.o uthread.o context.o preempt.o
	@echo "AR libuthread.a"
	$(Q)ar rcs $@ $^

preempt.o: preempt.c preempt.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

queue.o: queue.c queue.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

context.o: context.c context.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

uthread.o: uthread.c uthread.h context.h
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "CLEAN"
	$(Q)rm -f $(lib) *.o *.a *.out

#
# test: uthread_yield.c uthread.o context.o preempt.o queue.o
# 	gcc -o test uthread_yield.c uthread.o context.o preempt.o queue.o
#
# uthread.o: uthread.c uthread.h
# 	gcc -c uthread.c
#
# context.o: context.c context.h
# 	gcc -c context.c
#
# preempt.o: preempt.c preempt.h
# 	gcc -c preempt.c
#
# queue.o: queue.c queue.h
# 	gcc -c queue.c
#
# clean:
# 	@echo "CLEAN"
# 	$(Q)rm -f $(lib) *.o *.a *.out test
