project2Milestone.exe: Facility.o HashTable.o Manager.o tmpMain.o 
	g++ Facility.o HashTable.o Manager.o tmpMain.o -o project2Milestone.exe
Facility.o: src/Facility.cpp include/Facility.h
	g++ -Wall -Iinclude -c src/Facility.cpp
HashTable.o: src/HashTable.cpp include/HashTable.h include/Facility.h
	g++ -Wall -Iinclude -c src/HashTable.cpp
Manager.o: src/Manager.cpp include/Manager.h include/Facility.h include/HashTable.h
	g++ -Wall -Iinclude -c src/Manager.cpp		
tmpMain.o: src/tmpMain.cpp include/Manager.h include/Facility.h include/HashTable.h
	g++ -Wall -Iinclude -c src/tmpMain.cpp
clean:
	rm *.o project2Milestone.exe