run:
	./code message.txt code.txt

built:
	g++ code.cpp -Wall -O -o code
	
clean:
	rm code