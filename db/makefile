# lmmdb.exe: db.o immutableTable.o memTable.o log.o
# 	g++ -std=c++11 db.o memTable.o immutableTable.o log.o -o lmmdb.exe
# db.o: db.cpp
# 	g++ -std=c++11 -g -c db.cpp -o db.o

# immutableTable.o: immutableTable/ImmutableTable.cpp
# 	g++ -std=c++11 -g -c immutableTable/ImmutableTable.cpp -o immutableTable.o

# log.o: log/Log.cpp
# 	g++ -std=c++11 -g -c log/Log.cpp -o log.o

# memTable.o: memTable/MemTable.cpp log.o immutableTable.o 
# 	g++ -std=c++11 -g -c memTable/MemTable.cpp  log.o immutableTable.o  -o memTable.o

# 单例类，单独只编译不链接，getInstance() 返回的地址值为0x00，可以调用函数，但是不可以使用对象变量

lmmdb.exe: db.cpp immutableTable/ImmutableTable.cpp log/Log.cpp  memTable/MemTable.cpp
	g++ -std=c++11 -g db.cpp immutableTable/ImmutableTable.cpp log/Log.cpp  memTable/MemTable.cpp -o lmmdb.exe

clean:
	rm  lmmdb.exe *.o
