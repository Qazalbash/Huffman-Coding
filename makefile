CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -O3 -g
SRCS = code
 
$(SRCS) : $(SRCS).cpp
	$(CXX) $(CXXFLAGS) -o $(SRCS) $(SRCS).cpp

run: $(SRCS)
	./$(SRCS) message.txt code.txt

clean:
	rm -f $(SRCS)
