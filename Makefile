
all.o : NFA.o NFA_SE.o frontEnd.o main.o
	g++ -o all.o NFA.o NFA_SE.o frontEnd.o main.o 

run : all.o
	(./all.o < examples.txt) > result.txt

NFA.o : NFA.cpp NFA.h
	g++ -o NFA.o NFA.cpp -c

NFA_SE.o : NFA_SE.cpp NFA_SE.h
	g++ -o NFA_SE.o NFA_SE.cpp -c

frontEnd.o : frontEnd.cpp frontEnd.h
	g++ -o frontEnd.o frontEnd.cpp -c

main.o : main.cpp
	g++ -o main.o main.cpp -c
	
clean : 
	rm *.o