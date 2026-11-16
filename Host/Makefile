CXX = g++ 
CXXFLAGS = -std=c++20 -Wall -Wextra -g -pedantic 

all: main.o UART_Driver.o
	$(CXX) $(CXXFLAGS) main.o UART_Driver.o -o App 

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

UART_Driver.o: UART_Driver.cpp
	$(CXX) $(CXXFLAGS) -c UART_Driver.cpp

.PHONY: clean

clean:
	rm -f App main.o UART_driver.o
