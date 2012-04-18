LDFLAGS 	:= -lpthread
all: mthread_stack  

OBJS  :=    \
    simple_stack.o \
    mid_stack.o \

mthread_stack : $(OBJS) stack_header.h  


.PHONY: clean  
clean:  
	rm -rf mthread_stack $(OBJS)  
