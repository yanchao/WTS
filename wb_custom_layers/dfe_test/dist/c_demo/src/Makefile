all: c_demo

OBJS  := \
    main.o \
    date.o \
    linklist.o \
    math.o \
    strutils.o \
    calendar.o \
    engineer.o \

c_demo : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^


.PHONY: clean  
clean:  
	rm -rf c_demo $(OBJS)  
