lmmdb.exe: db.o memTable.o immutableTable.o log.o
	g++ -std=c++11 db.o memTable.o immutableTable.o log.o -o lmmdb.exe
db.o: db.cpp
	g++ -std=c++11 -g -c db.cpp -o db.o
memTable.o: memTable/MemTable.cpp
	g++ -std=c++11 -g -c memTable/MemTable.cpp -o memTable.o

immutableTable.o: immutableTable/ImmutableTable.cpp
	g++ -std=c++11 -g -c immutableTable/ImmutableTable.cpp -o immutableTable.o
log.o: log/Log.cpp
	g++ -std=c++11 -g -c log/Log.cpp -o log.o
clean:
	rm  lmmdb.exe *.o
