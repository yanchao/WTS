all: cpp_demo

OBJS  := \
    main.o \
    allocator.o \
    array.o \
    exception.o \
    list.o \
    multipleInhert.o \
    static.o \
    string.o \
    virtualBase.o \

cpp_demo : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

allocator.o : allocator.h

array.o : array.h

list.o : list.h

string.o : string.cpp string.h allocator.h


.PHONY: clean  
clean:  
	rm -rf cpp_demo $(OBJS)  
