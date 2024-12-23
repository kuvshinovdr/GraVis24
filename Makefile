

# Определение компилятора (gcc для Linux, g++ для Windows)
CXX=gcc

# Определение команды удаления и расширение файла в зависимости от ОС
ifeq ($(OS),Windows_NT)
	RM=del
	EXT=.exe
else
	RM=rm
	EXT=

endif

#Правило сборки исполняемого файла из объектных.

#CXX - переменная для команды компилятора. (По умолчанию g++.)
objects=m.o kva.o 

m.exe: $(objects)
	$(CXX) -o m.exe $(objects)

#Правило компиляции исходного файла с завизимостью от заголовочного файла в объектный
m.o: m.c kva.h
	$(CXX) -c m.c

#Правило компиляции исходного файла в объектный
kva.o: kva.c
	$(CXX) -c kva.c

#Фиктивное правило для очистки каталога от созданных в процессе сборки файлов.
#Для его вызова нужно указать clean после команды make.

#Пример теста листинга 1.1 со стр. 22
l101022o=L101_022.o L101_022test.o
L101022.exe: $(l101022o)
	$(CXX) -o L101022.exe $(l101022o)

L101_022.o: L101_022.c
	$(CXX) -c L101_022.c

L101_022test.o: L101_022test.c L101_022.h
	$(CXX) -c L101_022test.c

# Объекты для L604_217.exe
l604217o=L604_217.cpp graph.cpp

# Правило сборки L604_217.exe
L604_217.exe: $(l604217o)
	$(CXX) -o L604_217.exe $(l604217o)

# Компиляция L604_217.o с зависимостью от заголовочного файла graph.h
L604_217.o: L604_217.o graph.o
	$(CXX) -c L604_217.cpp

# Компиляция graph.o
graph.o: graph.cpp
	$(CXX) -c graph.cpp

l610229o=L610_229.o graph.o
L610_229.exe: $(l610229o)
	$(CXX) -o L610_229.exe $(l610229o)

# Компиляция L610_229.o с зависимостью от заголовочного файла graph.h
L610_229.o: L610_229.cpp graph.h
	$(CXX) -c L610_229.cpp

# Компиляция graph.o
graph.o: graph.cpp
	$(CXX) -c graph.cpp


l606219o=L606_219.o graph.o set_union.o
L606_219.exe: $(l606219o)
	$(CXX) -o L606_219.exe $(l606219o)
L606_219.o: L606_219.cpp graph.cpp ./L608_221/set_union.cpp
	$(CXX) -c L606_219.cpp

# Компиляция graph.cpp в объектный файл
graph.o: graph.cpp
	$(CXX) -c graph.cpp

# Компиляция set_union.cpp в объектный файл
set_union.o: ./L608_221/set_union.cpp
	$(CXX) -c ./L608_221/set_union.cpp


l509187o=L509_187.o graph.o
L509_187.exe: $(l509187o)
	$(CXX) -o L509_187.exe $(l509187o)

# Компиляция L558_189.o с зависимостью от заголовочного файла graph.h
L509_187.o: L509_187.cpp graph.h
	$(CXX) -c L509_187.cpp

# Компиляция graph.o
graph.o: graph.cpp
	$(CXX) -c graph.cpp




#L111_022

l111022o=L111_022.o insertion_sort.o
L111_022.exe: $(l111022o)
	$(CXX) -o L111_022.exe $(l111022o)
L111_022.o: L111_022.cpp insertion_sort.cpp
	$(CXX) -c L111_022.cpp 

# Компиляция insertion_sort.cpp в объектный файл
insertion_sort.o: insertion_sort.cpp
	$(CXX) -c insertion_sort.cpp

# L110_034
l110034o=L110_034.o increment.o
L110_034.exe: $(l110034o)
	$(CXX) -o L110_034.exe $(l110034o) 
L110_034.o: L110_034.cpp increment.cpp	
	$(CXX) -c L110_034.cpp 

# Компиляция insertion_sort.cpp в объектный файл
increment.o: increment.cpp
	$(CXX) -c increment.cpp 

# L221_059
l221059o=L221_059.o selection_sort.o
L221_059.exe: $(l221059o)
	$(CXX) -o L221_059.exe $(l221059o) 
L221_059.o: L221_059.cpp selection_sort.cpp	
	$(CXX) -c L221_059.cpp 

# Компиляция selection_sort.cpp в объектный файл
selection_sort.o: selection_sort.cpp
	$(CXX) -c selection_sort.cpp 





l513189o=L513_189.o L509_187.o graph.o
L513_189.exe: $(l513189o)
	$(CXX) -o L513_189.exe $(l513189o)

L513_189.o: L513_189.cpp L509_187.cpp
	$(CXX) -c L513_189.cpp

# Компиляция L509_187.o с зависимостью от заголовочного файла graph.h
L509_187.o: L509_187.cpp graph.h
	$(CXX) -c L509_187.cpp

# Компиляция graph.o
graph.o: graph.cpp
	$(CXX) -c graph.cpp


TARGET = Cyc.exe

# Объектные файлы
objects = Cyc.o

# Правило сборки исполняемого файла
$(TARGET): $(objects)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(objects)

# Компиляция объекта
Cyc.o: Cyc.cpp
	$(CXX) $(CXXFLAGS) -c Cyc.cpp


.PHONY: clean
clean:
	$(RM) $(objects) $(l101022o) $(l604217o) ${l606219o} ${l610229o} m.exe L101022.exe L604_217.exe L606_219.exe L610_229.exe


