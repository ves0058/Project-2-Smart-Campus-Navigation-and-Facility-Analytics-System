project2Milestone.exe: Facility.o HashTable.o Manager.o campusgraph.o DepartmentTree.o Main.o 
	g++ Facility.o HashTable.o Manager.o campusgraph.o DepartmentTree.o Main.o -o project2Milestone.exe
Facility.o: src/Facility.cpp include/Facility.h
	g++ -Wall -Iinclude -c src/Facility.cpp
HashTable.o: src/HashTable.cpp include/HashTable.h include/Facility.h
	g++ -Wall -Iinclude -c src/HashTable.cpp
Manager.o: src/Manager.cpp include/Manager.h include/Facility.h include/HashTable.h
	g++ -Wall -Iinclude -c src/Manager.cpp
campusgraph.o: src/campusgraph.cpp include/campusgraph.h
	g++ -Wall -Iinclude -c src/campusgraph.cpp
DepartmentTree.o: src/DepartmentTree.cpp include/DepartmentTree.h
	g++ -Wall -Iinclude -c src/DepartmentTree.cpp
Main.o: src/Main.cpp include/Manager.h include/Facility.h include/HashTable.h include/campusgraph.h include/DepartmentTree.h
	g++ -Wall -Iinclude -c src/Main.cpp
clean:
	rm *.o project2Milestone.exe