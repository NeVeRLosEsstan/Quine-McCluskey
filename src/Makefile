CC:=g++
main:=main
QMK:=Quine_McCluskey
PET:=Petrick
head:=imp


$(main).o:$(main).cpp $(QMK).o $(PET).o
	$(CC) -o $(main).o $(main).cpp $(QMK).o $(PET).o

$(QMK).o:$(head).h $(QMK).h $(QMK).cpp
	$(CC) -c $(QMK).cpp
	
$(PET).o:$(head).h $(PET).h $(PET).cpp
	$(CC) -c $(PET).cpp

clean:
	rm *.o
