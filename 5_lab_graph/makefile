CC      = C:/msys64/ucrt64/bin/gcc.exe
CXX     = C:/msys64/ucrt64/bin/g++.exe
CFLAGS  = -Werror -Wall -std=c11
GTEST   = C:/msys64/ucrt64
BENCH   = C:/msys64/ucrt64

build: graph.o main.o
	$(CC) $(CFLAGS) graph.o main.o -o lab5.exe

test: graph.o test_graph.o
	$(CXX) -std=c++17 graph.o test_graph.o \
		-L$(GTEST)/lib -lgtest -lgtest_main -lpthread \
		-o test_lab5.exe
	cmd /c "set PATH=C:\msys64\ucrt64\bin;%PATH% && .\test_lab5.exe"

bench: graph.o bench_graph.o
	$(CXX) -std=c++17 graph.o bench_graph.o \
		-L$(BENCH)/lib -lbenchmark -lbenchmark_main -lpthread -lshlwapi \
		-o bench_lab5.exe
	cmd /c "set PATH=C:\msys64\ucrt64\bin;%PATH% && .\bench_lab5.exe"

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c graph.c -o graph.o

main.o: main.c graph.h
	$(CC) $(CFLAGS) -c main.c -o main.o

test_graph.o: test_graph.cpp graph.h
	$(CXX) -std=c++17 -Wall -I$(GTEST)/include -c test_graph.cpp -o test_graph.o

bench_graph.o: bench_graph.cpp graph.h
	$(CXX) -std=c++17 -Wall -I$(BENCH)/include -c bench_graph.cpp -o bench_graph.o

run: build
	lab5.exe --load sample.txt --result result.txt

clean:
	if exist *.o del /q *.o
	if exist *.exe del /q *.exe
	if exist result.txt del /q result.txt
	if exist lab5_test_tmp.txt del /q lab5_test_tmp.txt
