all: asteroids


asteroids: asteroids.o tardis.o asteroid.o dalek.o draw.o model.o explosion.o shot.o
	gcc -lGL -lglut -lGLU -std=c++0x asteroids.o tardis.o asteroid.o dalek.o draw.o model.o explosion.o shot.o -o asteroid

asteroids.o: asteroids.cpp asteroids.h tardis.h asteroid.h model.h
	gcc -lGL -lglut -lGLU -std=c++0x asteroids.cpp -c
	
asteroid.o: asteroid.cpp asteroid.h model.h
	gcc -lGL -lglut -lGLU -std=c++0x asteroid.cpp -c	
	
tardis.o: tardis.cpp tardis.h model.h
	gcc -lGL -lglut -lGLU -std=c++0x tardis.cpp -c

dalek.o: dalek.cpp dalek.h model.h
	gcc -lGL -lglut -lGLU -std=c++0x dalek.cpp -c
	
draw.o: draw.cpp draw.h
	gcc -lGL -lglut -lGLU -std=c++0x draw.cpp -c
	
model.o: model.cpp model.h
	gcc -lGL -lglut -lGLU -std=c++0x model.cpp -c
	
explosion.o: explosion.cpp explosion.h
	gcc -lGL -lglut -lGLU -std=c++0x explosion.cpp -c
	
shot.o: shot.cpp shot.h
	gcc -lGL -lglut -lGLU -std=c++0x shot.cpp -c
    
clean: 
	rm *.o

