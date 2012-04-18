all    : penguin

HEADER := b_ball.h  b_hard.h  b_soft.h  b_spin.h  b_wall.h
OBJS   := b_grid.o  b_main.o

penguin: $(OBJS) $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $^


.PHONY : clean
clean  :
	rm -rf penguin $(OBJS)
