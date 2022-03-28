# Target library
lib := libuthread.a

all: $(lib)
CC = gcc
FLAGS = -Wall -Wextra -Werror
OBJS = queue.o uthread.o context.o

ifneq ($(V),1)
Q = @
endif

deps := $(patsubst %.o, %d, $(OBJS))
-include $(deps)

libuthread.a: $(OBJS)
	@echo "CC  @"
	$(Q) ar rcs $@ $^

uthread.o: uthread.c uthread.h private.h queue.h
	@echo "CC  $@"
	$(Q) $(CC) $(FLAGS) -c -o $@ $<

context.o: context.c private.h uthread.h
	@echo "CC  $@"
	$(Q) $(CC) $(FLAGS) -c -o $@ $<

queue.o: queue.c queue.h
	@echo "CC  $@"
	$(Q) $(CC) $(FLAGS) -c -o $@ $<

clean:
	@echo "clean"
	$(Q)rm -f $(lib) $(OBJS)

	# Advanced Makefile for Project 2
	# Following Discussion 4 slides format for Dependency Tracking
