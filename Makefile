SRCS = $(wildcard sourse/*.cpp)

OBJS = $(SRCS:.cpp=.o)

all: hello 

hello: $(OBJS) main.o
	g++ main.o $(OBJS) -o hell.exe

main.o: main.cpp
	g++ -I ./headers main.cpp -c

%.o: %.cpp
	g++ -I ./headers -c $< -o $@

clean:
	DEL *.o
	DEL *.exe