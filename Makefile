CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

TARGET = project2Milestone.exe

OBJECTS = Facility.o HashTable.o Manager.o DepartmentTree.o CampusGraph.o Main.o

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

Facility.o: src/Facility.cpp include/Facility.h
	$(CXX) $(CXXFLAGS) -c src/Facility.cpp -o Facility.o

HashTable.o: src/HashTable.cpp include/HashTable.h include/Facility.h
	$(CXX) $(CXXFLAGS) -c src/HashTable.cpp -o HashTable.o

Manager.o: src/Manager.cpp include/Manager.h include/Facility.h include/HashTable.h
	$(CXX) $(CXXFLAGS) -c src/Manager.cpp -o Manager.o

DepartmentTree.o: src/DepartmentTree.cpp include/DepartmentTree.h
	$(CXX) $(CXXFLAGS) -c src/DepartmentTree.cpp -o DepartmentTree.o

CampusGraph.o: src/CampusGraph.cpp include/CampusGraph.h
	$(CXX) $(CXXFLAGS) -c src/CampusGraph.cpp -o CampusGraph.o

Main.o: src/Main.cpp include/Manager.h include/Facility.h include/HashTable.h include/DepartmentTree.h include/CampusGraph.h
	$(CXX) $(CXXFLAGS) -c src/Main.cpp -o Main.o

clean:
	rm -f *.o $(TARGET)

.PHONY: clean